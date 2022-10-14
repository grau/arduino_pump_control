/**
 * @file State logic. Processing sensor input and potentially changing outputs
 */

#ifndef state_h
#define state_h

#include "inputOutput.h"

/**
 * @file State logic. Active state and state-change logic
 */

/** Temperature difference to activate pump on output 0 an 1  */
#define DELTA_T_ON 10

/** Hysteresis to deactivate Pumps on Output 0 and 1 */
#define HYSTERESIS 5

/** Minimum temperature of solarpanel to activate pump */
#define TMIN 60

/** Minimum temperature before liquid begins to freeze. Activate pump to defrost */
#define TFROST -25

/** Maximum temperature of water tank to activate pump */
#define TMAX 90

/** All possible states the system may be in */
enum SYSTEM_STATES {
    STATE_ROOF_TOO_COLD,
    STATE_BUFFER_TOO_HOT,
    STATE_ROOF_FROST,
    STATE_PUMP_ON,
    STATE_PUMP_OFF,
};

/** The active state. Initial is pump off - somewhat save */
extern enum SYSTEM_STATES systemState;

/**
 * Heater logic.
 */
void applyLogic();

#endif