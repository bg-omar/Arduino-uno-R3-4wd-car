#include <Arduino.h>
#include "main.h"
#include "Wire.h"
#include "motor.h"
#include "PS4controls.h"

/*
 *   PS4 controller is currently connected to USB-shield
 * 	00:1a:7d:da:71:13
 */

void setup() {
	//Serial.begin(115200);
	Serial.println("UNO 4wd car + ps4 BLUE");

	PS4controls::PS4setup();

	Motor::motor_setup();
}

void loop() {

	PS4controls::PS4loop();

}







