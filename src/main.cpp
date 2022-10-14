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
  Serial.begin(115200);

  initInputOutput();
  initLcd();
  // initI2c();
}

void loop() {
  readSensors();
  applyLogic();
  serialOutput();
  lcdOutput();
  delay(1000);
}
