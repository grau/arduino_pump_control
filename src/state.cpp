#include "state.h"

enum SYSTEM_STATES systemState = STATE_PUMP_OFF;

void applyLogic() {
  int deltaT = SENSOR_ROOF - SENSOR_BUFFER_LOW;
  if (SENSOR_BUFFER_TOP >= TMAX 
    || (systemState == STATE_BUFFER_TOO_HOT && SENSOR_BUFFER_TOP >= TMAX - HYSTERESIS)) {
    systemState = STATE_BUFFER_TOO_HOT;
    setOutput(0, false);
    setOutput(1, false);
  } else if (SENSOR_ROOF <= TFROST
    || (systemState == STATE_ROOF_FROST && SENSOR_ROOF <= TFROST + HYSTERESIS)) {
    systemState = STATE_ROOF_FROST;
    setOutput(0, true);
    setOutput(1, true);
  } else if (SENSOR_ROOF <= TMIN
    || (systemState == STATE_ROOF_TOO_COLD && SENSOR_ROOF <= TMIN + HYSTERESIS)) {
    systemState = STATE_ROOF_TOO_COLD;
    setOutput(0, false);
    setOutput(1, false);
  } else if (deltaT > DELTA_T_ON) {
    systemState = STATE_PUMP_ON;
    setOutput(0, true);
    setOutput(1, true);
  } else if (deltaT < DELTA_T_ON - HYSTERESIS) {
    // Difference between roof and buffer too small - turn of the pumps and wait.
    systemState = STATE_PUMP_OFF;
    setOutput(0, false);
    setOutput(1, false);
  }
}