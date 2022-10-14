/**
 * @file LCD data output. Before use initLcd() must be called once
 */

#ifndef lcd_h
#define lcd_h

// Needed for ... everything arduino-ish
#include <Arduino.h>

// I2C Communication 
#include <Wire.h>

// I2C LCD library
#include <LiquidCrystal_I2C.h>

// Access to sensor/output values
#include "inputOutput.h"

// State logic. Holds the active state
#include "state.h"

/**
 * Initialize lcd. I2C, custom chars, backlight, ...
 */
void initLcd();

/**
 * Outputs state on LCD
 */
void lcdOutput();

#define LCD_ROOF 0
#define LCD_BUF_TOP 1
#define LCD_BUF_MID 2
#define LCD_BUF_LOW 3
#define LCD_DEG 4

#endif