/**
 * @file Input output logic. Reading sensor data, calculating temperatures and providing outputs.
 */

#ifndef inputoutput_h
#define inputoutput_h

// Needed for ... everything arduino-ish
#include <Arduino.h>

/** Multiplexer ports */
#define ADDR_S2 10      // Address select, S2 Pin @ 74HC4051
#define ADDR_S1 11      // Address select, S1 Pin @ 74HC4051
#define ADDR_S0 12      // Address select, S0 Pin @ 74HC4051

/** Output ports */
#define BUILTIN_LED 13  // Builtin LED. Good for debugging.
#define OUT_1 2         // Output port 1
#define OUT_2 3         // Output port 2
#define OUT_3 4         // Output port 3
#define OUT_4 5         // Output port 4

/** Size of buffer used for median calculation */
#define BUFFER_SIZE 129
/** Center position of buffer */
#define BUFFER_MEDIAN 64

/** Sensor types */
#define SENSOR_TYPE_RAW 1           // Read raw value and store
#define SENSOR_TYPE_R 2             // Calculate resistance value and store
#define SENSOR_TYPE_PT1000 3        // Calculate temperature based on PT1000 rule and store
#define SENSOR_TYPE_KTY81_220 4     // Calculate temperature based on KTY81_220 rule and store
#define SENSOR_TYPE_OFF 0           // Sensor not connected. Skip

#define CALIBRATION_RESISTOR 3873l   // Resistor on Port 23 used for calibration

#define SENSOR_BUFFER_LOW sensorVals[0]
#define SENSOR_BUFFER_MID sensorVals[3]
#define SENSOR_BUFFER_TOP sensorVals[2]
#define SENSOR_ROOF sensorVals[1]

/** Values of sensors. */
extern int sensorVals[];

/** State of outputs */
extern int outputStates[];

/**
 * Initializes the sensor logic
 */
void initInputOutput();

/**
 * Puts the right configuration on the 74HC4051E multiplexers and reads the
 * value from correct adc.
 * Contains wait intervalls and averaging to retrieve a good result.
 * Therefore reading a value is rather slow (~25ms).
 *
 * @param id Input id to read value from
 * @returns Raw analog read value
 */
int readVal(uint8_t id);

/**
 * Maps given raw-AD value to resistance value
 *
 * @param val Raw AD value (0-1023)
 * @returns Resistance value in ohms
 */
int getR(int val);

/**
 * Maps resistance value to temperature for a pt1000 temperature sensor
 *
 * @param r Resistance value in ohms
 * @returns Temperature in °C
 */
int getTempPt1000(int r);

/**
 * Maps resistance value to temperature for a KTY81 220 temperature sensor
 *
 * @param r Resistance value from getR
 * @returns Temperature in °C
 * @see Datasheets/KTY81_SER.pdf
 */
int getTempKty81_220(int r);

/**
 * Reads all sensors and stores their values for later use.
 */
void readSensors();

/**
 * There should be a 220 Ohms resistor on port 23.
 * Reads the value as raw and updates the calibrationVal
 */
void calibrate();

/**
 * Sets a output channel to the given value
 * @param id of output channel (0-3)
 * @param high if value should be set to high (true) or low (false)
 */
void setOutput(int id, bool high);
#endif