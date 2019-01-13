/*
 * HomieRollershutter.cpp
 *
 *  Created on: 10.01.2019
 *      Author: ian
 */

#include <HomieRollershutter.h>
#include <LoggerNode.h>

const String HomieRollershutter::propState __attribute__((init_priority(101)))("state");
const String HomieRollershutter::propMoving __attribute__((init_priority(101)))("moving");
const String HomieRollershutter::propAutolearn __attribute__((init_priority(101)))("auto-learn");


HomieRollershutter::HomieRollershutter(const char* id, const char* name, uint8_t _pinOpen, uint8_t _pinClose, bool invert):
	HomieNode(id, name, "RollerShutter"),
	shutter(_pinOpen, _pinClose, !invert)
{
	advertise(propState.c_str()).settable().setName("Rollershutter State").setDatatype("integer").setUnit("%").setFormat("0:100");
	advertise(propMoving.c_str()).setName("moving info").setDatatype("enum"); //.setFormat(getMovingInfoStrings().c_str());
	advertise(propAutolearn.c_str()).setName("start auto-learning").setDatatype("boolean");
}



void HomieRollershutter::setup() {
	shutter.begin();
	shutter.trace(Serial);
	shutter.onChange([this]( int idx, int v, int up ) {setProperty(propMoving).send(String(shutter.mapstate(v)));},0);
	shutter.onPos([this]( int idx, int v, int up ) {setProperty(propState).send(String(v));},0);
}

bool HomieRollershutter::handleInput(const HomieRange& range,
		const String& property, const String& value) {
	LN.logf(__PRETTY_FUNCTION__, LoggerNode::DEBUG, "Received Input for topic %s with message %s.", property.c_str(), value.c_str());

	if (property.equals(propState)) {
		if (isDigit(value.charAt(0))) {
			uint8_t perValue = value.toInt();
			if (perValue > 100) return false;
			shutter.cmd_pos(perValue);
		} else if (value.equalsIgnoreCase("UP")) {
			shutter.cmd_up();
		} else if (value.equalsIgnoreCase("DOWN")) {
			shutter.cmd_down();
		} else if (value.equalsIgnoreCase("STOP")) {
			shutter.cmd_halt();
		} else {
			return false;
		}
		return true;
	} else if (property.equals(propAutolearn)) {
		// TODO: not yet implemented
		return true;
	}

	return false; // unhandled
}

