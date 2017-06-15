/*
 * I2C.h 
 *
 * These prototypes are identical to those in ISIS HAL. Ported to Arduino.
 */

#ifdef I2C_SLAVE_H
	#error "Can't be master and slave at the same time"
#endif

#ifndef I2C_H
#define I2C_H

/*!
 * Initializes the I2C driver.
 * @param i2cBusSpeed_Hz Speed of the I2C bus in Hertz. (Minimum: 10000, Maximum 400000)
 * @param i2cTransferTimeout Timeout for a byte to be transferred in 1/10th of a tick.
 * @note When debugging the code use portMAX_DELAY to make this timeout infinite.
 * Otherwise, stepping through code can trigger the I2C timeout.
 * If this value is less than 1, the driver uses a timeout of 1.
 * @return -3 if creating queues to handle transfers fails,
 * -2 if Initializing the TWI peripheral fails,
 * -1 if creating the task that consumes I2C transfer requests failed,
 * 0 on success.
 */
int I2C_start(uint32_t i2cBusSpeed_Hz, uint32_t i2cTransferTimeout);

/*!
 * Sets the timeout for a read or write transaction to complete.
 * @param i2cTransferTimeout Timeout for a read or write transaction to complete in 1/10th of a tick.
 * If this value is less than 1, the driver uses a timeout of 1.
 * @note When debugging the code, use portMAX_DELAY to make this timeout infinite.
 * Otherwise, stepping through code can trigger the I2C timeout.
 */
void I2C_setTransferTimeout(uint32_t i2cTransferTimeout);

/*!
 * De-initializes the I2C driver.
 */
void I2C_stop();

/*!
 * Writes data to a slave on the I2C bus using DMA.
 * This is a blocking call. The task will be swapped out of context while the transfer is in progress.
 * @param slaveAddress Address of the slave where to make the transfer.
 * @param data Memory location of the data to be written to the I2C slave.
 * @param size Number of bytes to be written to the I2C slave.
 * @return -3 if creating a semaphore to wait for the transfer fails,
 * -2 if the input transfer parameters are wrong or the driver is not initialized,
 * -1 if queuing the transfer fails,
 * 0 on success,
 * 4 on write error,
 * 6 on timeout error,
 * 7 on general error.
 * @note Negative values imply there was an initialization error,
 * so the transfer was never attempted on the I2C bus,
 * positive values mean a transfer was attempted on the I2C bus but failed.
 * @note Positive error values correspond in value to the members of I2CtransferStatus.
 */
int I2C_write(uint32_t slaveAddress, uint8_t *data, uint32_t size);

/*!
 * Reads data from a slave on the I2C bus using DMA.
 * This is a blocking call. The task will be swapped out of context while the transfer is in progress.
 * @param slaveAddress Address of the slave where to make the transfer.
 * @param data Memory location to store the data read from the I2C slave. Must be able to store size bytes.
 * @param size Number of bytes to be read from the I2C slave.
 * @return -3 if creating a semaphore to wait for the transfer fails,
 * -2 if the input transfer parameters are wrong,
 * -1 if queuing the transfer fails,
 * 0 on success,
 * 5 on read error,
 * 6 on timeout error,
 * 7 on general error.
 * @note Negative values imply there was an initialization error,
 * so the transfer was never attempted on the I2C bus,
 * positive values mean a transfer was attempted on the I2C bus but failed.
 * @note Positive error values correspond in value to the members of I2CtransferStatus.
 */
int I2C_read(uint32_t slaveAddress, uint8_t *data, uint32_t size);

#endif
