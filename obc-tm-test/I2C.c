/*
 * I2C.c
 *
 * Author: John Wikman
 *
 * Description:
 * An I2C implementation based on the Wire.cpp implementation.
 */

#include <inttypes.h>
#include <include/twi.h> // Atmel CMSIS Driver

#include "variant.h" // Included in the original source code

#define ENABLE_DEBUG
#include "debug.h"

#include "I2C.h"

#define I2C_READY 0
#define I2C_MRX   1
#define I2C_MTX   2
#define I2C_SRX   3
#define I2C_STX   4

#if WIRE_INTERFACES_COUNT < 1
  #error No TWI found
#endif

/* Pointer to TWI0 */
static Twi *twi = WIRE_INTERFACE;

static uint32_t i2cTimeoutPeriod;

static int I2C_WaitByteSent() {
  volatile uint32_t timeout = i2cTimeoutPeriod;
  uint32_t status = 0;
  while ((status & TWI_SR_TXRDY) != TWI_SR_TXRDY) {
    status = TWI_GetStatus(twi);

    if (status & TWI_SR_NACK)
      return 4; // Write Error

    if (--timeout == 0)
      return 6; // Timeout Error
  }

  return 0; // OK
}

static int I2C_WaitByteReceived() {
  volatile uint32_t timeout = i2cTimeoutPeriod;
  uint32_t status = 0;
  while ((status & TWI_SR_RXRDY) != TWI_SR_RXRDY) {
    status = TWI_GetStatus(twi);

    if (status & TWI_SR_NACK)
      return 5; // Read Error

    if (--timeout == 0)
      return 6; // Timeout Error
  }

  return 0; // OK
}

static int TWI_WaitTransferComplete() {
  volatile uint32_t timeout = i2cTimeoutPeriod;
  uint32_t status = 0;
  while ((status & TWI_SR_TXCOMP) != TWI_SR_TXCOMP) {
    status = TWI_GetStatus(twi);

    if (status & TWI_SR_NACK)
      return 7; // General Error

    if (--timeout == 0)
      return 6; // Timeout Error
  }

  return 0; // OK
}


int I2C_start(uint32_t i2cBusSpeed_Hz, uint32_t i2cTransferTimeout)
{
  //** BELOW IS COPIED FROM THE WIRE.CPP IMPLEMENTATION **//
  pmc_enable_periph_clk(WIRE_INTERFACE_ID);
  PIO_Configure(
      g_APinDescription[PIN_WIRE_SDA].pPort,
      g_APinDescription[PIN_WIRE_SDA].ulPinType,
      g_APinDescription[PIN_WIRE_SDA].ulPin,
      g_APinDescription[PIN_WIRE_SDA].ulPinConfiguration);
 PIO_Configure(
      g_APinDescription[PIN_WIRE_SCL].pPort,
      g_APinDescription[PIN_WIRE_SCL].ulPinType,
      g_APinDescription[PIN_WIRE_SCL].ulPin,
      g_APinDescription[PIN_WIRE_SCL].ulPinConfiguration);

  NVIC_DisableIRQ(WIRE_ISR_ID);
  NVIC_ClearPendingIRQ(WIRE_ISR_ID);
  NVIC_SetPriority(WIRE_ISR_ID, 0);
  NVIC_EnableIRQ(WIRE_ISR_ID);
  //******************************************************//

  // Disable PDC channel
  twi->TWI_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

  TWI_ConfigureMaster(twi, i2cBusSpeed_Hz, VARIANT_MCK);

  I2C_setTransferTimeout(i2cTransferTimeout);

  return 0;
}

void I2C_setTransferTimeout(uint32_t i2cTransferTimeout)
{
  i2cTimeoutPeriod = i2cTransferTimeout;
}

void I2C_stop()
{
  TWI_Disable(twi);

  // Enable PDC channel
  twi->TWI_PTCR &= ~(UART_PTCR_RXTDIS | UART_PTCR_TXTDIS);

  //** BELOW IS COPIED FROM THE WIRE.CPP IMPLEMENTATION **//
  NVIC_DisableIRQ(WIRE_ISR_ID);
  NVIC_ClearPendingIRQ(WIRE_ISR_ID);

  pmc_disable_periph_clk(WIRE_INTERFACE_ID);

  // no need to undo PIO_Configure, 
  // as Peripheral A was enable by default before,
  // and pullups were not enabled
  //******************************************************//
}

/*
 * Writes to a slave.
 * 
 * Contains debug prints. If errors are ocurring on the I2C line,
 * consider removing ENABLE_DEBUG from the top of the file.
 */
int I2C_write(uint32_t slaveAddress, uint8_t *data, uint32_t size)
{
  if (slaveAddress > 0x70)
    return -2;

  if (size < 1)
    return -2;

  int I2C_error = 0;

  debug("S");
  TWI_StartWrite(twi, (uint8_t) slaveAddress, 0, 0, data[0]);

  debug("P");
  I2C_error = I2C_WaitByteSent();

  uint32_t currentByte = 1;
  while (currentByte < size && I2C_error == 0) {
    TWI_WriteByte(twi, data[currentByte++]);
    I2C_error = I2C_WaitByteSent();
    debug("P");
	}

  if (I2C_error == 0) {
    debug("St");
    TWI_Stop(twi);
    I2C_error = TWI_WaitTransferComplete();
	}

  return I2C_error;
}

/*
 * Reads from a slave.
 * 
 * Contains debug prints. If errors are ocurring on the I2C line,
 * consider removing ENABLE_DEBUG from the top of the file.
 */
int I2C_read(uint32_t slaveAddress, uint8_t *data, uint32_t size)
{
  if (slaveAddress > 0x70)
    return -2;

  if (size < 1)
    return -2;

  int I2C_error = 0;

  debug("S");
  TWI_StartRead(twi, (uint8_t) slaveAddress, 0, 0);

  uint32_t currentByte = 0;
  while (currentByte < size && I2C_error == 0) {
    debug("P");
    // Send stop condition at last byte
    if (currentByte + 1 == size) {
      debug("St");
      TWI_SendSTOPCondition(twi);
    }

    I2C_error = I2C_WaitByteReceived();
    if (I2C_error == 0)
      data[currentByte++] = TWI_ReadByte(twi);
  }

  I2C_error = TWI_WaitTransferComplete();

  return I2C_error;
}

/**
 * ISR Handler for wire.
 */
void WIRE_ISR_HANDLER(void) {
	// This is not used by master.
}
