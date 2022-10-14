#include "main.h"

void serialOutput() {
  Serial.print("INPUT --> ");
  for (int i = 0; i < 5; i++) {
    Serial.print("ID: ");
    Serial.print(i);
    Serial.print(". Value: ");
    Serial.print(sensorVals[i]);
    Serial.print(" || \t");
  }
  Serial.print("\nOUTPUT --> ");
  for (int i = 0; i < 4; i++) {
    Serial.print("ID: ");
    Serial.print(i);
    Serial.print(". Value: ");
    Serial.print(outputStates[i]);
    Serial.print(" || \t");
  }
  Serial.println("");
}

void setup() {
  wdt_reset();
  // wdt_enable is broken --> @see https://forum.arduino.cc/t/arduino-nano-wdt-problem/492906
  WDTCSR = (1 << WDCE) | (1 << WDE);
  WDTCSR = (1 << WDIE) | (1 << WDE) | (1 << WDP3) | (1 << WDP0);

  Serial.begin(115200);

  initInputOutput();
  initLcd();

  // Can't use LCD and I2C together
  // initI2c();
}

void loop() {
  wdt_reset();
  readSensors();
  applyLogic();
  serialOutput();
  lcdOutput();
  wdt_reset();
  delay(1000);
}
