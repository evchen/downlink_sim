/*
 * I2C_configuration.h
 *
 * Authors: Johan Sjöblom & John Wikman
 *
 * Description:
 * Contains all the configurations for the I2C bus.
 *
 * - I2C_BIT_RATE: Determines the bit rate of the I2C bus in Hz.
 * - I2C_BUFFER_SIZE: The size of the TX and RX bufferinos for the slave.
 */

#ifndef I2C_CONFIGURATION_H
#define I2C_CONFIGURATION_H

#define I2C_BIT_RATE 400000
#define I2C_TIMEOUT_RATE 100000
#define I2C_BUFFER_SIZE 512

#endif


