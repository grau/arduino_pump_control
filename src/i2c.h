/**
 * @file Logic to use arduino as I2C slave. Master can be an raspberry pi.
 */

#ifndef i2cSlave_h
#define i2cSlave_h

// Needed for ... everything arduino-ish
#include <Arduino.h>

// I2C Communication 
#include <Wire.h>

// Access to sensor/output values
#include "inputOutput.h"

/** I2C slave address */
#define I2C_ADDRESS 0x20

/** On a receive event of the index to read from is set. */
extern volatile int i2cIndex;

/** Array for storing 16 bit number for transfer as 8bit blocks */
extern uint8_t i2cTransferArray[];

/**
 * Initialize arduino as i2c slave
 */
void initI2c();

/**
 * Request handler for incoming I2C read requests
 */
void i2cRequest();

/**
 * Request handler for incoming I2C write requests.
 * Only sets the index of the next read operation
 * 
 * @param numBytes number of bytes.
 */
void i2cReceive(int numBytes);

#endif