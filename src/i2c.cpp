#include "i2c.h"

volatile int i2cIndex = 0;
uint8_t i2cTransferArray[2];

void initI2c() {
  Wire.begin(I2C_ADDRESS);
  Wire.onRequest(i2cRequest);
  Wire.onReceive(i2cReceive);
}

void i2cRequest() {
  if (i2cIndex < 2*24) { // Reading sensors
    int numIndex = i2cIndex / 2;
    int subIndex = i2cIndex % 2;
    i2cTransferArray[0] = (sensorVals[numIndex] >> 8) & 0xFF;
    i2cTransferArray[1] = sensorVals[numIndex] & 0xFF;
    
    if (sensorVals[numIndex] > 0) {
      i2cTransferArray[1] = 0;
      i2cTransferArray[0] = sensorVals[numIndex];
    } else {
      i2cTransferArray[1] = 1;
      i2cTransferArray[0] = -1 * sensorVals[numIndex];
    }

    Wire.write(i2cTransferArray[subIndex]);
  } else { // Reading outputs
    Serial.print("Request for id ");
    Serial.print(i2cIndex);
    Serial.print(". Sending id ");
    Serial.print(i2cIndex-2*24);
    Serial.print(". value ");
    Serial.println(outputStates[i2cIndex-2*24]);
    Wire.write(outputStates[i2cIndex-2*24]);
  }
  
}

void i2cReceive(int numBytes) {
  int inIndex = Wire.read();
  if (inIndex >= 0 && inIndex <= 2*24 + 4) {
    i2cIndex = inIndex;
  }
}