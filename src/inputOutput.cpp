#include "inputOutput.h"

int sensorVals[24];
int outputStates[4];

/** Buffer used for median calculation */
int medianBuffer[BUFFER_SIZE];

/** Magic calibration value. Updated on every readValues iteration */
int calibrationVal = 2211;

/** Resistance values for a kty81 220 sensor. First value is for -50°C - last for 150°C with a 10°C step per value. */
const int kty81_220TempVals[] = { 1030, 1135, 1247, 1367, 1495, 1630, 1772, 1922, 2080, 2245, 2417, 2597, 2785, 2980, 3182, 3392, 3607, 3817, 4008, 4166, 4280 };

/** Lookup table to match internal port numbers to external ones */
const int PORT_LUT[] = {6, 0, 4, 2, 1, 3, 7, 5, 15, 13, 14, 8, 12, 10, 9, 11, 22, 16, 20, 18, 17, 19, 23, 21};

/** Types of sensors */
const int SENSOR_TYPES[24] = { SENSOR_TYPE_PT1000, SENSOR_TYPE_PT1000, SENSOR_TYPE_PT1000, SENSOR_TYPE_PT1000, SENSOR_TYPE_KTY81_220 };

void initInputOutput() {
  pinMode(ADDR_S0, OUTPUT);
  pinMode(ADDR_S1, OUTPUT);
  pinMode(ADDR_S2, OUTPUT);

  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(OUT_1, OUTPUT);
  pinMode(OUT_2, OUTPUT);
  pinMode(OUT_3, OUTPUT);
  pinMode(OUT_4, OUTPUT);
}

int readVal(uint8_t in_id) {
  int id = PORT_LUT[in_id];
  digitalWrite(ADDR_S0, ((id & 0b001) > 0) ? HIGH : LOW);
  digitalWrite(ADDR_S1, ((id & 0b010) > 0) ? HIGH : LOW);
  digitalWrite(ADDR_S2, ((id & 0b100) > 0) ? HIGH : LOW);
  const uint8_t readPin = id < 8 ? A0 : (id < 16 ? A1 : A2);
  delay(5);

  int val;
  int tmp;
  for (uint8_t i = 0; i < BUFFER_SIZE; i++) {
    val = analogRead(readPin);
    for (uint8_t j = 0; j <= i; j++) {
      if (j == i) {
        medianBuffer[i] = val;
      } else if (val < medianBuffer[j]) {
        tmp = medianBuffer[j];
        medianBuffer[j] = val;
        val = tmp;
      }
    }
    delay(1);
  }
  return medianBuffer[BUFFER_MEDIAN];
}

int getR(int val) {
  long inVal = val > 1022 ? 1022 : val;
  // 2211 is a magic constant found by approximation.
  return  (inVal * calibrationVal) / (1023  - inVal);
}

int getTempPt1000(int r) {
  return (r - 1000) / 3.85;
}

int getTempKty81_220(int r) {
  if (r < kty81_220TempVals[0]) {
    return -55; // Below lower limit. Less than -55 °C
  }
  for (int i = 1; i < 20; i++) {
    // perfect match - return exact value
    if (r == kty81_220TempVals[i]) {
      return i * 10 - 50;
    // Value is lower than table value. Linear approximation of in-between value.
    } else if (r < kty81_220TempVals[i]) {
      int t1 = (i - 1) * 10 - 50;
      int t2 = i * 10 - 50;
      int r1 = kty81_220TempVals[i-1];
      int r2 = kty81_220TempVals[i];
      double m = (0.0 + t2 - t1) / (0.0 + r2 - r1);
      int b = t1 - m * r1;
      return m * r + b;
    }
    // Value is bigger than table value. Try next one or return upper limit.
  }
  return 150; // Beyond upper limit. More than 150 °C
}

void readSensors() {
  digitalWrite(BUILTIN_LED, HIGH);
  calibrate();
  for (int i = 0; i < 23; i++) {
    switch (SENSOR_TYPES[i]) {
      case SENSOR_TYPE_RAW:
        sensorVals[i] = readVal(i);
        break;
      case SENSOR_TYPE_R:
        sensorVals[i] = getR(readVal(i));
        break;
      case SENSOR_TYPE_PT1000:
        sensorVals[i] = getTempPt1000(getR(readVal(i)));
        break;
      case SENSOR_TYPE_KTY81_220:
        sensorVals[i] = getTempKty81_220(getR(readVal(i)));
        break;
      case SENSOR_TYPE_OFF:
        // Sensor deactivated. Skip.
        break;
      default:
        Serial.print("Unknown sensor type: ");
        Serial.println(SENSOR_TYPES[i]);
    }
    delay(10);
  }
  digitalWrite(BUILTIN_LED, LOW);
}

void calibrate() {
  long val = readVal(23);
  if (val < 100 || val > 1000) {
    Serial.print("Calibation failed. We have a HUGE PROBLEM! Read: ");
    Serial.println(val);
    calibrationVal = 2211; // Educated guess
  } else {
    calibrationVal = ((1023 - val) * CALIBRATION_RESISTOR) / val;
  }
}

void setOutput(int id, bool high) {
  outputStates[id] = high;
  switch(id) {
    case 0:
      digitalWrite(OUT_1, high ? HIGH : LOW);
      return;
    case 1:
      digitalWrite(OUT_2, high ? HIGH : LOW);
      return;
    case 2:
      digitalWrite(OUT_3, high ? HIGH : LOW);
      return;
    case 3:
      digitalWrite(OUT_4, high ? HIGH : LOW);
      return;
  }
}