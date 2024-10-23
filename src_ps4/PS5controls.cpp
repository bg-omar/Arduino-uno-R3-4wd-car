//
// Created by mr on 10/21/2024.
//

#include "PS5controls.h"
/*
 Example sketch for the PS5 Bluetooth library - developed by Kristian Sloth Lauszus
 For more information visit the Github repository: github.com/felis/USB_Host_Shield_2.0 or
 send me an e-mail: lauszus@gmail.com
 */

#include <PS5BT.h>
#include <usbhub.h>
#include "motor.h"
#include "main.h"
// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb5;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd5(&Usb5); // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS5BT class in two ways */
// This will start an inquiry and then pair with the PS5 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS5 controller will then start to blink rapidly indicating that it is in pairing mode
PS5BT PS5(&Btd5, PAIR);

// After that you can simply create the instance like so and then press the PS button on the device
//PS5BT PS5(&Btd);

bool printAngle = false, printTouch = false;
uint16_t lastMessageCounter = -1;
uint8_t player_led_mask = 0;
bool microphone_led = false;
uint32_t ps_timer;

void PS5controls::PS5setup() {
#if !defined(__MIPSEL__)
	while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
	if (Usb5.Init() == -1) {
		Serial.print(F("\r\nOSC did not start"));
		while (1); // Halt
	}
	Serial.print(F("\r\nPS5 Bluetooth Library Started"));
}

void PS5controls::PS5loop() {
	Usb5.Task();

	if (PS5.connected() && lastMessageCounter != PS5.getMessageCounter()) {
		lastMessageCounter = PS5.getMessageCounter();
		int FL_velocity, BR_velocity, BL_velocity, FR_velocity;
		if (PS5.getAnalogHat(LeftHatX) > 137 || PS5.getAnalogHat(LeftHatX) < 117 || PS5.getAnalogHat(LeftHatY) > 137 || PS5.getAnalogHat(LeftHatY) < 117 || PS5.getAnalogHat(RightHatX) > 137 || PS5.getAnalogHat(RightHatX) < 117 || PS5.getAnalogHat(RightHatY) > 137 || PS5.getAnalogHat(RightHatY) < 117) {
			uint8_t Yright = PS5.getAnalogHat(RightHatY);
			uint8_t Xright = PS5.getAnalogHat(RightHatX);
			uint8_t Xleft = PS5.getAnalogHat(LeftHatX);
			uint8_t Yleft = PS5.getAnalogHat(LeftHatY);

			// Ensure values are within expected range
			if (Xleft > 255) Xleft = 255;
			if (Yleft > 255) Yleft = 255;
			if (Xright > 255) Xright = 255;
			if (Yright > 255) Yright = 255;

			int speedX = map(Xleft, 0, 255, -255, 255);
			int speedY = map(Yleft, 0, 255, -255, 255);
			int rotation = map(Xright, 0, 255, -255, 255);
			int drifting = map(Yright, 0, 255, -255, 255);

			// Dead zone to prevent small unintentional movements
			if (abs(speedX) < 10) speedX = 0;
			if (abs(speedY) < 10) speedY = 0;
			if (abs(rotation) < 10) rotation = 0;
			if (abs(drifting) < 10) drifting = 0;  // Adjust dead zone for "weird" if needed

			FR_velocity = speedY - speedX - rotation ;
			BR_velocity = speedY - speedX + rotation + drifting;
			BL_velocity = speedY + speedX - rotation + drifting;
			FL_velocity = speedY + speedX + rotation ;

		} else {
			FR_velocity = 0;
			BR_velocity = 0;
			BL_velocity = 0;
			FL_velocity = 0;
		}

		// Determine rotation for the motors and change neg to pos pwm value
		if (FR_velocity < 0) { Motor::pcf.digitalWrite(0, LOW); Motor::pcf.digitalWrite(1, HIGH); FR_velocity *= -2; } else{  Motor::pcf.digitalWrite(0, HIGH); Motor::pcf.digitalWrite(1, LOW); FR_velocity *= 2;}
		if (BR_velocity < 0) { Motor::pcf.digitalWrite(4, LOW); Motor::pcf.digitalWrite(5, HIGH); BR_velocity *= -2; } else{  Motor::pcf.digitalWrite(4, HIGH); Motor::pcf.digitalWrite(5, LOW); BR_velocity *= 2;}
		if (BL_velocity < 0) { Motor::pcf.digitalWrite(2, LOW); Motor::pcf.digitalWrite(3, HIGH); BL_velocity *= -2; } else{  Motor::pcf.digitalWrite(2, HIGH); Motor::pcf.digitalWrite(3, LOW); BL_velocity *= 2;}
		if (FL_velocity < 0) { Motor::pcf.digitalWrite(6, LOW); Motor::pcf.digitalWrite(7, HIGH); FL_velocity *= -2; } else{  Motor::pcf.digitalWrite(6, HIGH); Motor::pcf.digitalWrite(7, LOW); FL_velocity *= 2;}


		if (abs(FR_velocity) > 255) FR_velocity = 255;
		if (abs(BR_velocity) > 255) BR_velocity = 255;
		if (abs(BL_velocity) > 255) BL_velocity = 255;
		if (abs(FL_velocity) > 255) FL_velocity = 255;

		if (abs(FR_velocity) < 10) FR_velocity = 0;
		if (abs(BR_velocity) < 10) BR_velocity = 0;
		if (abs(BL_velocity) < 10) BL_velocity = 0;
		if (abs(FL_velocity) < 10) FL_velocity = 0;

		analogWrite(4, FR_velocity);
		analogWrite(5, BR_velocity);
		analogWrite(6, BL_velocity);
		analogWrite(7, FL_velocity);

		if (PS5.getAnalogButton(L2) || PS5.getAnalogButton(R2)) { // These are the only analog buttons on the PS5 controller
			Serial.print(F("\r\nL2: "));
			Serial.print(PS5.getAnalogButton(L2));
			Serial.print(F("\tR2: "));
			Serial.print(PS5.getAnalogButton(R2));
		}

		// Set the left trigger to resist at the right trigger's level
		static uint8_t oldR2Value = 0xFF;
		if (PS5.getAnalogButton(R2) != oldR2Value) {
			oldR2Value = PS5.getAnalogButton(R2);
			PS5.leftTrigger.setTriggerForce(oldR2Value, 255);
		}

		// Hold the PS button for 1 second to disconnect the controller
		// This prevents the controller from disconnecting when it is reconnected,
		// as the PS button is sent when it reconnects
		if (PS5.getButtonPress(PS)) {
			if (millis() - ps_timer > 1000)
				PS5.disconnect();
		} else
			ps_timer = millis();

		if (PS5.getButtonClick(PS))
			Serial.print(F("\r\nPS"));
		if (PS5.getButtonClick(TRIANGLE)) {
			Serial.print(F("\r\nTriangle"));
			PS5.setRumbleOn(RumbleLow);
		}
		if (PS5.getButtonClick(CIRCLE)) {
			Serial.print(F("\r\nCircle"));
			PS5.setRumbleOn(RumbleHigh);
		}
		if (PS5.getButtonClick(CROSS)) {
			Serial.print(F("\r\nCross"));

			// Set the player LEDs
			player_led_mask = (player_led_mask << 1) | 1;
			if (player_led_mask > 0x1F)
				player_led_mask = 0;
			PS5.setPlayerLed(player_led_mask); // The bottom 5 bits set player LEDs
		}
		if (PS5.getButtonClick(SQUARE)) {
			Serial.print(F("\r\nSquare"));
			PS5.setRumbleOff();
		}

		if (PS5.getButtonClick(UP)) {
			Serial.print(F("\r\nUp"));
			PS5.setLed(Red);
		} if (PS5.getButtonClick(RIGHT)) {
			Serial.print(F("\r\nRight"));
			PS5.setLed(Blue);
		} if (PS5.getButtonClick(DOWN)) {
			Serial.print(F("\r\nDown"));
			PS5.setLed(Yellow);
		} if (PS5.getButtonClick(LEFT)) {
			Serial.print(F("\r\nLeft"));
			PS5.setLed(Green);
		}

		if (PS5.getButtonClick(L1))
			Serial.print(F("\r\nL1"));
		if (PS5.getButtonClick(L3))
			Serial.print(F("\r\nL3"));
		if (PS5.getButtonClick(R1))
			Serial.print(F("\r\nR1"));
		if (PS5.getButtonClick(R3))
			Serial.print(F("\r\nR3"));

		if (PS5.getButtonClick(CREATE))
			Serial.print(F("\r\nCreate"));
		if (PS5.getButtonClick(OPTIONS)) {
			Serial.print(F("\r\nOptions"));
			printAngle = !printAngle;
		}
		if (PS5.getButtonClick(TOUCHPAD)) {
			Serial.print(F("\r\nTouchpad"));
			printTouch = !printTouch;
		}
		if (PS5.getButtonClick(MICROPHONE)) {
			Serial.print(F("\r\nMicrophone"));
			microphone_led = !microphone_led;
			PS5.setMicLed(microphone_led);
		}

		if (printAngle) { // Print angle calculated using the accelerometer only
			Serial.print(F("\r\nPitch: "));
			Serial.print(PS5.getAngle(Pitch));
			Serial.print(F("\tRoll: "));
			Serial.print(PS5.getAngle(Roll));
		}

		if (printTouch) { // Print the x, y coordinates of the touchpad
			if (PS5.isTouching(0) || PS5.isTouching(1)) // Print newline and carriage return if any of the fingers are touching the touchpad
				Serial.print(F("\r\n"));
			for (uint8_t i = 0; i < 2; i++) { // The touchpad track two fingers
				if (PS5.isTouching(i)) { // Print the position of the finger if it is touching the touchpad
					Serial.print(F("X")); Serial.print(i + 1); Serial.print(F(": "));
					Serial.print(PS5.getX(i));
					Serial.print(F("\tY")); Serial.print(i + 1); Serial.print(F(": "));
					Serial.print(PS5.getY(i));
					Serial.print(F("\t"));
				}
			}
		}
	}
}
