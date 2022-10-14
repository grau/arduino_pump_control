/**
 * @file Main application entry point. Reads values, applies output logic, shows data on lcd
 */

#ifndef temp_main
#define temp_main

// Needed for ... everything arduino-ish
#include <Arduino.h>

// I2C Communication (LCD or data)
#include <Wire.h>

// Watchdog timer
#include <avr/wdt.h>

// State logic
#include "state.h"

// LCD Custom characters
#include "lcd.h"

// State logic
#include "state.h"

// Access to sensor/output values
#include "inputOutput.h"

// For using arduino as I2C slave.
// !!! DEACTIVATED! LCD NEEDS ARDUINO AS I2C MASTER !!!
// #include "i2c.h"

#endif