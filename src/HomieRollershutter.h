/*
 * HomieRollershutter.h
 *
 *  Created on: 10.01.2019
 *      Author: ian
 */

#ifndef SRC_HOMIEROLLERSHUTTER_H_
#define SRC_HOMIEROLLERSHUTTER_H_

#include <HomieNode.hpp>

class HomieRollershutter: public HomieNode {
public:
	HomieRollershutter(const char* id, const char* name, uint8_t _pinOpen=13, uint8_t _pinClose=12, bool _invert=true);

	virtual void setup() override;
	virtual void loop() override;
	virtual void onReadyToOperate() override;
	virtual bool handleInput(const HomieRange& range, const String  &property, const String &value) override;


private:
	uint8_t pinOpen, pinClose;
	bool on;
	uint8_t estimatedState, destState;
	uint16_t fullMoveMillisUp; // time (in millis) to move shutters completely
	uint16_t fullMoveMillisDown; // time (in millis) to move shutters completely

	enum MOVING_INFO {
		NOMOVE = 0, DOWN, UP, EXTRA_DOWN, EXTRA_UP,
		MI_COUNT  // array-size, leave at end of enumeration
	} movingInfo;

	static const char* const mi_string[MI_COUNT];

	void updateState();

	static const String propState;
	static const String propMoving;
	static const String propAutolearn;

	static const String& getMovingInfoStrings();



};

#endif /* SRC_HOMIEROLLERSHUTTER_H_ */
