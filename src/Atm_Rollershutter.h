#pragma once

#include <Automaton.h>

class Atm_Rollershutter: public Machine {

 public:
  enum { DOWN, MOVING_DOWN_EXTRA, MOVING_DOWN, STOPPED, MOVING_UP, MOVING_UP_EXTRA, UP }; // STATES
  enum { EVT_STEP, EVT_END_REACHED, EVT_EXTRA_TIMER_EXP, EVT_CMD_DOWN, EVT_CMD_HALT, EVT_CMD_UP, EVT_POS_REACHED, ELSE }; // EVENTS
  Atm_Rollershutter( uint8_t _pinOpen, uint8_t _pinClose, bool _on ) :
	  Machine(),
	  estimatedPosition(100),
	  pinOpen(_pinOpen), pinClose(_pinClose), on(_on),
	  timePerStepUp(800), timePerStepDn(800), extraTimeUp(1500), extraTimeDn(1500)
      {};

  Atm_Rollershutter& begin( void );
  Atm_Rollershutter& trace( Stream & stream );
  Atm_Rollershutter& trigger( int event );
  int state( void );
  Atm_Rollershutter& onChange( Machine& machine, int event = 0 );
  Atm_Rollershutter& onChange( atm_cb_push_t callback, int idx = 0 );
  Atm_Rollershutter& onChange( atm_cb_lambda_t callback, int idx = 0);
  Atm_Rollershutter& onPos( Machine& machine, int event = 0 );
  Atm_Rollershutter& onPos( atm_cb_push_t callback, int idx = 0 );
  Atm_Rollershutter& onPos( atm_cb_lambda_t callback, int idx = 0);

  Atm_Rollershutter& cmd_down( void );
  Atm_Rollershutter& cmd_halt( void );
  Atm_Rollershutter& cmd_up( void );
  Atm_Rollershutter& cmd_pos( uint8_t destPos );

  const char* mapstate(int state) { return mapSymbol(state + state_width - ATM_ON_EXIT, symbols);}

 private:
  enum { ENT_DOWN, EXT_DOWN, ENT_MOVING_DOWN_EXTRA, EXT_MOVING_DOWN_EXTRA, ENT_MOVING_DOWN, LP_MOVING_DOWN, EXT_MOVING_DOWN, ENT_STOPPED, ENT_MOVING_UP, LP_MOVING_UP, EXT_MOVING_UP, ENT_MOVING_UP_EXTRA, EXT_MOVING_UP_EXTRA, ENT_UP }; // ACTIONS
  enum { ON_CHANGE, ON_POS, CONN_MAX }; // CONNECTORS

  atm_connector connectors[CONN_MAX];
  int event( int id );
  void action( int id );

  atm_timer_millis timerExtra, timerStep;
  atm_counter counter_stepsToGo;

  uint8_t estimatedPosition;
  uint8_t pinOpen, pinClose;
  bool on;

  uint32_t timePerStepUp, timePerStepDn, extraTimeUp, extraTimeDn;

  void moveUp() {digitalWrite(pinClose,!on);	digitalWrite(pinOpen,on);}
  void moveDn() {digitalWrite(pinClose,on);	digitalWrite(pinOpen,!on);}
  void moveStp() {digitalWrite(pinClose,!on);	digitalWrite(pinOpen,!on);}

};
/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_Rollershutter">
    <states>
      <DOWN index="0" on_enter="ENT_DOWN" on_exit="EXT_DOWN">
        <EVT_CMD_DOWN>MOVING_DOWN_EXTRA</EVT_CMD_DOWN>
        <EVT_CMD_UP>MOVING_UP</EVT_CMD_UP>
      </DOWN>
      <MOVING_DOWN_EXTRA index="1" on_enter="ENT_MOVING_DOWN_EXTRA" on_exit="EXT_MOVING_DOWN_EXTRA">
        <EVT_EXTRA_TIMER_EXP>DOWN</EVT_EXTRA_TIMER_EXP>
        <EVT_CMD_HALT>DOWN</EVT_CMD_HALT>
        <EVT_CMD_UP>MOVING_UP</EVT_CMD_UP>
      </MOVING_DOWN_EXTRA>
      <MOVING_DOWN index="2" on_enter="ENT_MOVING_DOWN" on_loop="LP_MOVING_DOWN" on_exit="EXT_MOVING_DOWN">
        <EVT_STEP>MOVING_DOWN</EVT_STEP>
        <EVT_END_REACHED>MOVING_DOWN_EXTRA</EVT_END_REACHED>
        <EVT_CMD_HALT>STOPPED</EVT_CMD_HALT>
        <EVT_CMD_UP>MOVING_UP</EVT_CMD_UP>
        <EVT_POS_REACHED>STOPPED</EVT_POS_REACHED>
      </MOVING_DOWN>
      <STOPPED index="3" on_enter="ENT_STOPPED">
        <EVT_CMD_DOWN>MOVING_DOWN</EVT_CMD_DOWN>
        <EVT_CMD_UP>MOVING_UP</EVT_CMD_UP>
      </STOPPED>
      <MOVING_UP index="4" on_enter="ENT_MOVING_UP" on_loop="LP_MOVING_UP" on_exit="EXT_MOVING_UP">
        <EVT_STEP>MOVING_UP</EVT_STEP>
        <EVT_END_REACHED>MOVING_UP_EXTRA</EVT_END_REACHED>
        <EVT_CMD_DOWN>MOVING_DOWN</EVT_CMD_DOWN>
        <EVT_CMD_HALT>STOPPED</EVT_CMD_HALT>
        <EVT_POS_REACHED>STOPPED</EVT_POS_REACHED>
      </MOVING_UP>
      <MOVING_UP_EXTRA index="5" on_enter="ENT_MOVING_UP_EXTRA" on_exit="EXT_MOVING_UP_EXTRA">
        <EVT_EXTRA_TIMER_EXP>UP</EVT_EXTRA_TIMER_EXP>
        <EVT_CMD_DOWN>MOVING_DOWN</EVT_CMD_DOWN>
        <EVT_CMD_HALT>UP</EVT_CMD_HALT>
      </MOVING_UP_EXTRA>
      <UP index="6" on_enter="ENT_UP">
        <EVT_CMD_DOWN>MOVING_DOWN</EVT_CMD_DOWN>
        <EVT_CMD_UP>MOVING_UP_EXTRA</EVT_CMD_UP>
      </UP>
    </states>
    <events>
      <EVT_STEP index="0" access="PRIVATE"/>
      <EVT_END_REACHED index="1" access="PRIVATE"/>
      <EVT_EXTRA_TIMER_EXP index="2" access="PRIVATE"/>
      <EVT_CMD_DOWN index="3" access="PUBLIC"/>
      <EVT_CMD_HALT index="4" access="PUBLIC"/>
      <EVT_CMD_UP index="5" access="PUBLIC"/>
      <EVT_POS_REACHED index="6" access="PRIVATE"/>
    </events>
    <connectors>
      <CHANGE autostore="0" broadcast="1" dir="PUSH" slots="1"/>
      <POS autostore="0" broadcast="0" dir="PUSH" slots="1"/>
    </connectors>
    <methods>
    </methods>
  </machine>
</machines>

Automaton::ATML::end
*/

