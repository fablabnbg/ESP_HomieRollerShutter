#include "Atm_Rollershutter.h"


/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

Atm_Rollershutter& Atm_Rollershutter::begin() {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                                   ON_ENTER         ON_LOOP                ON_EXIT     EVT_STEP    EVT_END_REACHED  EVT_EXTRA_TIMER_EXP       EVT_CMD_DOWN  EVT_CMD_HALT       EVT_CMD_UP  EVT_POS_REACHED  ELSE */
    /*              DOWN */              ENT_DOWN,             -1,              EXT_DOWN,          -1,                -1,                  -1, MOVING_DOWN_EXTRA,           -1,       MOVING_UP,              -1,   -1,
    /* MOVING_DOWN_EXTRA */ ENT_MOVING_DOWN_EXTRA,             -1, EXT_MOVING_DOWN_EXTRA,          -1,                -1,                DOWN,                -1,         DOWN,       MOVING_UP,              -1,   -1,
    /*       MOVING_DOWN */       ENT_MOVING_DOWN, LP_MOVING_DOWN,       EXT_MOVING_DOWN, MOVING_DOWN, MOVING_DOWN_EXTRA,                  -1,                -1,      STOPPED,       MOVING_UP,         STOPPED,   -1,
    /*           STOPPED */           ENT_STOPPED,             -1,                    -1,          -1,                -1,                  -1,       MOVING_DOWN,           -1,       MOVING_UP,              -1,   -1,
    /*         MOVING_UP */         ENT_MOVING_UP,   LP_MOVING_UP,         EXT_MOVING_UP,   MOVING_UP,   MOVING_UP_EXTRA,                  -1,       MOVING_DOWN,      STOPPED,              -1,         STOPPED,   -1,
    /*   MOVING_UP_EXTRA */   ENT_MOVING_UP_EXTRA,             -1,   EXT_MOVING_UP_EXTRA,          -1,                -1,                  UP,       MOVING_DOWN,           UP,              -1,              -1,   -1,
    /*                UP */                ENT_UP,             -1,                    -1,          -1,                -1,                  -1,       MOVING_DOWN,           -1, MOVING_UP_EXTRA,              -1,   -1,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  pinMode(pinOpen, OUTPUT);
  pinMode(pinClose, OUTPUT);
  digitalWrite(pinOpen, !on);
  digitalWrite(pinClose, !on);
  return *this;
}

/* Add C++ code for each internally handled event (input)
 * The code must return 1 to trigger the event
 */
int Atm_Rollershutter::event(int id) {
	switch (id) {
	case EVT_STEP:
		return timerStep.expired(this);

	case EVT_END_REACHED:
		if (counter_stepsToGo.expired()	&& (estimatedPosition == 0 || estimatedPosition == 100)) {
			Serial.printf("EndPos %d reached.\n", estimatedPosition);
			return true;
		}
		return 0;
	case EVT_EXTRA_TIMER_EXP:
		return timerExtra.expired(this);
	case EVT_POS_REACHED:
		if (counter_stepsToGo.expired()	&& !(estimatedPosition == 0 || estimatedPosition == 100)) {
			Serial.printf("Pos %d reached.\n", estimatedPosition);
			return true;
		} else {
			return false;
		}
	}
	return 0;
}

Atm_Rollershutter& Atm_Rollershutter::cmd_pos(uint8_t destPos) {
	Serial.printf("New destination position: %d\n", destPos);
	int8_t delta = destPos - estimatedPosition;
	if (delta > 0 || destPos == 100) {
		Serial.printf("%d steps to go down\n", delta);
		counter_stepsToGo.set(delta);
		cmd_down();
	}
	if (delta < 0 || destPos == 0) {
		Serial.printf("%d steps to go up\n", -delta);
		counter_stepsToGo.set(-delta);
		cmd_up();
	}
	return *this;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 *
 * Available connectors:
 *   push( connectors, ON_CHANGE | ATM_BROADCAST, 1, <v>, <up> );
 *   push( connectors, ON_POS, 0, <v>, <up> );
 */

void Atm_Rollershutter::action( int id ) {
  char * stateString = 0;
  switch ( id ) {
    case ENT_DOWN:
    	moveStp();
        return;
    case EXT_DOWN:
      return;
    case ENT_MOVING_DOWN_EXTRA:
    	moveDn();
    	timerExtra.set(2000);
        return;
    case EXT_MOVING_DOWN_EXTRA:
      return;
    case ENT_MOVING_DOWN:
    	moveDn();
    	timerStep.set(500);
    	counter_stepsToGo.decrement();
		Serial.print('v');
		estimatedPosition++;
		if (estimatedPosition % 2 == 0) push( connectors, ON_POS, 0, estimatedPosition, 0);
    	return;
    case LP_MOVING_DOWN:
		return;
    case EXT_MOVING_DOWN:
      return;
    case ENT_STOPPED:
    	moveStp();
    	push( connectors, ON_POS, 0, estimatedPosition, 0);
    	push( connectors, ON_CHANGE, 0, 0, 0);
    	//mapSymbol(ELSE + STOPPED, stateString);
    	//Serial.printf("New state %s", stateString);
        return;
    case ENT_MOVING_UP:
    	moveUp();
    	timerStep.set(500);
    	counter_stepsToGo.decrement();
		Serial.print('^');
		estimatedPosition--;
    	if (estimatedPosition % 2 == 0) push( connectors, ON_POS, 0, estimatedPosition, 0);
    	return;
    case LP_MOVING_UP:
        return;
    case EXT_MOVING_UP:
      return;
    case ENT_MOVING_UP_EXTRA:
    	moveUp();
    	timerExtra.set(2400);
        return;
    case EXT_MOVING_UP_EXTRA:
      return;
    case ENT_UP:
      moveStp();
      return;
  }
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_Rollershutter& Atm_Rollershutter::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_Rollershutter::state( void ) {
  return Machine::state();
}

/* Nothing customizable below this line
 ************************************************************************************************
*/

/* Public event methods
 *
 */

Atm_Rollershutter& Atm_Rollershutter::cmd_down() {
  trigger( EVT_CMD_DOWN );
  return *this;
}

Atm_Rollershutter& Atm_Rollershutter::cmd_halt() {
  trigger( EVT_CMD_HALT );
  return *this;
}

Atm_Rollershutter& Atm_Rollershutter::cmd_up() {
  trigger( EVT_CMD_UP );
  return *this;
}

/*
 * onChange() push connector variants ( slots 1, autostore 0, broadcast 1 )
 */

Atm_Rollershutter& Atm_Rollershutter::onChange( Machine& machine, int event ) {
  onPush( connectors, ON_CHANGE, 0, 1, 1, machine, event );
  return *this;
}

Atm_Rollershutter& Atm_Rollershutter::onChange( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_CHANGE, 0, 1, 1, callback, idx );
  return *this;
}

Atm_Rollershutter& Atm_Rollershutter::onChange(atm_cb_lambda_t callback, int idx) {
	onPush(connectors, ON_CHANGE, 0, 1, 1, callback, idx);
	return *this;
}

/*
 * onPos() push connector variants ( slots 1, autostore 0, broadcast 0 )
 */

Atm_Rollershutter& Atm_Rollershutter::onPos( Machine& machine, int event ) {
  onPush( connectors, ON_POS, 0, 1, 1, machine, event );
  return *this;
}

Atm_Rollershutter& Atm_Rollershutter::onPos( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_POS, 0, 1, 1, callback, idx );
  return *this;
}

Atm_Rollershutter& Atm_Rollershutter::onPos( atm_cb_lambda_t callback, int idx ) {
  onPush( connectors, ON_POS, 0, 1, 1, callback, idx );
  return *this;
}


/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_Rollershutter& Atm_Rollershutter::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "ROLLERSHUTTER\0EVT_STEP\0EVT_END_REACHED\0EVT_EXTRA_TIMER_EXP\0EVT_CMD_DOWN\0EVT_CMD_HALT\0EVT_CMD_UP\0EVT_POS_REACHED\0ELSE\0DOWN\0MOVING_DOWN_EXTRA\0MOVING_DOWN\0STOPPED\0MOVING_UP\0MOVING_UP_EXTRA\0UP" );
  return *this;
}

