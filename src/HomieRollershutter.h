/*
 * HomieRollershutter.h
 *
 *  Created on: 10.01.2019
 *      Author: ian
 */

#ifndef SRC_HOMIEROLLERSHUTTER_H_
#define SRC_HOMIEROLLERSHUTTER_H_

#include <HomieNode.hpp>
#include <Atm_Rollershutter.h>

class HomieRollershutter: public HomieNode {
public:
	HomieRollershutter(const char* id, const char* name, uint8_t _pinOpen=13, uint8_t _pinClose=12, bool _invert=true);

	virtual void setup() override;
//	virtual void loop() override;
//	virtual void onReadyToOperate() override;
	virtual bool handleInput(const HomieRange& range, const String  &property, const String &value) override;


private:
	static const String propState;
	static const String propMoving;
	static const String propAutolearn;

	Atm_Rollershutter shutter;
};

#endif /* SRC_HOMIEROLLERSHUTTER_H_ */
