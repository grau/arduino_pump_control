#include "main.h"

bool bootFlag = true;

void serialOutput() {
  Serial.print("{state:");
  Serial.print(systemState);
  Serial.print(",boot:");
  Serial.print(bootFlag ? "true" : "false");
  Serial.print(",input:[");
  for (int i = 0; i < 4; i++) {
    Serial.print("{id:");
    Serial.print(i);
    Serial.print(",val:");
    Serial.print(sensorVals[i]);
    Serial.print("},");
  }
  Serial.print("],output: [");
  for (int i = 0; i < 2; i++) {
    Serial.print("{id:");
    Serial.print(i);
    Serial.print(",val:");
    Serial.print(outputStates[i]);
    Serial.print("},");
  }
  Serial.println("]}");
  bootFlag = false;
}

void setup() {
  wdt_reset();
  // wdt_enable is broken --> @see https://forum.arduino.cc/t/arduino-nano-wdt-problem/492906
  WDTCSR = (1 << WDCE) | (1 << WDE);
  WDTCSR = (1 << WDIE) | (1 << WDE) | (1 << WDP3) | (1 << WDP0);

  bootFlag = true;
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
