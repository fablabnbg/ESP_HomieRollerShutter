/*
 * main.cpp
 *
 *  Created on: 10.01.2019
 *      Author: ian
 */


#include <Homie.hpp>
#include <Automaton.h>
#include <LoggerNode.h>
#include <HomieRollershutter.h>


#define FW_NAME "Rollershutter"
#define FW_MAJOR "0"
#define COMMIT_COUNTER "1"
#define BUILD_NUMBER "1"

#define FW_VERSION FW_MAJOR "." COMMIT_COUNTER "." BUILD_NUMBER

/* Magic sequence for Autodetectable Binary Upload */
const char *__FLAGGED_FW_NAME = "\xbf\x84\xe4\x13\x54" FW_NAME "\x93\x44\x6b\xa7\x75";
const char *__FLAGGED_FW_VERSION = "\x6a\x3f\x3e\x0e\xe1" FW_VERSION "\xb0\x30\x48\xd4\x1a";
/* End of magic sequence for Autodetectable Binary Upload */

LoggerNode LN;
//
HomieRollershutter shutter1("shutter1", "Fenster 3");
HomieRollershutter shutter2("shutter2", "Fenster 4", 14, 16);

void setup() {
  Homie_setFirmware(FW_NAME, FW_VERSION);
  Serial.begin(74880);  // Baudrate of ESP8266 Boot loader. Set it here to same value, so you can see both the boot loader's messages and your's.
  Serial.println("Start");
  Serial.flush();
  Homie.disableLedFeedback();
  Homie.disableResetTrigger();
  Homie.setLoggingPrinter(&Serial);

  Homie.setup();

}


void loop() {
  Homie.loop();
  automaton.run();
}

