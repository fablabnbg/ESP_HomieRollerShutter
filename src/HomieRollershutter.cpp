/*
 * HomieRollershutter.cpp
 *
 *  Created on: 10.01.2019
 *      Author: ian
 */

#include <HomieRollershutter.h>
#include <LoggerNode.h>

const String HomieRollershutter::propState("state");
const String HomieRollershutter::propMoving("moving");
const String HomieRollershutter::propAutolearn("auto-learn");

HomieRollershutter::HomieRollershutter(const char* id, const char* name, uint8_t _pinOpen, uint8_t _pinClose, bool invert):
	HomieNode(id, name, "RollerShutter"),
	pinOpen(_pinOpen), pinClose(_pinClose), on(!invert),
	estimatedState(0),  // TODO: Reload from SPIFFS/EEPROM
	destState(0),
	fullMoveMillisUp(15000),
	fullMoveMillisDown(15000),
	movingInfo(NOMOVE)
{
	// Unfortunately it is not possible to use the static string variables here (propState.c_str() etc.) because it is not guaranteed that they are initialized yet
	advertise("state").settable().setName("Rollershutter State").setDatatype("integer").setUnit("%").setFormat("0:100");
	advertise("moving").setName("moving info").setDatatype("enum").setFormat(getMovingInfoStrings().c_str());
	advertise("auto-learn").setName("start auto-learning").setDatatype("boolean");

}


const char* const HomieRollershutter::mi_string[MI_COUNT] = {"no", "DOWN", "UP", "DOWN_EXTRA", "UP_EXTRA"};
const String& HomieRollershutter::getMovingInfoStrings() {
	static String* rc = NULL;
	if (!rc) {
		rc = new String();
		for (int_fast8_t iInfo = NOMOVE; iInfo < MI_COUNT; iInfo++) {
			rc->concat(mi_string[iInfo]);
			if (iInfo < MI_COUNT - 1) rc->concat(',');
		}
	}
	return *rc;
}

void HomieRollershutter::setup() {
	pinMode(pinOpen, OUTPUT);
	pinMode(pinClose, OUTPUT);
	digitalWrite(pinOpen, !on);   // if invert is true, true is the off state
	digitalWrite(pinClose, !on);
}

void HomieRollershutter::loop() {
	MOVING_INFO newMI = movingInfo;
	if (destState > estimatedState) {
		digitalWrite(pinOpen, on);
		digitalWrite(pinClose,!on);
		estimatedState++;
		delay(40);
		newMI = UP;
	} else if (destState < estimatedState) {
		digitalWrite(pinClose, on);
		digitalWrite(pinOpen, !on);
		estimatedState--;
		delay(40);
		newMI = DOWN;
	} else {
		digitalWrite(pinOpen, !on);   // if invert is true, true is the off state
		digitalWrite(pinClose, !on);
		newMI = NOMOVE;
	}
	if (movingInfo != newMI) {
		movingInfo = newMI;
		updateState();
	}


}

void HomieRollershutter::onReadyToOperate() {
	updateState();

}

bool HomieRollershutter::handleInput(const HomieRange& range,
		const String& property, const String& value) {
	destState = estimatedState;
	LN.logf(__PRETTY_FUNCTION__, LoggerNode::DEBUG, "Received Input for topic %s with message %s.", property.c_str(), value.c_str());

	if (property.equals(propState)) {
		if (isDigit(value.charAt(0))) {
			uint8_t perValue = value.toInt();
			if (perValue > 100) return false;
			destState = perValue;
		} else if (value.equalsIgnoreCase("UP")) {
			destState = 100;
		} else if (value.equalsIgnoreCase("DOWN")) {
			destState = 0;
		}
	} else if (property.equals(propAutolearn)) {
		// TODO: not yet implemented
		return true;
	}

	return false; // unhandled
}

void HomieRollershutter::updateState() {
	setProperty(propState).send(String(estimatedState));
	setProperty(propMoving).send(String(mi_string[movingInfo]));
}
