//
// Created by mr on 6/18/2024.
//
#include "PS4controls.h"
#include "motor.h"

#include <PS4BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode
PS4BT PS4(&Btd, PAIR);

// After that you can simply create the instance like so and then press the PS button on the device
//PS4BT PS4(&Btd);

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

void PS4controls::PS4setup() {
	if (Usb.Init() == -1) {
		Serial.print(F("\r\nOSC did not start"));
		while (1); //halt
	}
	Serial.print(F("\r\nPS4 USB Library Started"));
}

void PS4controls::PS4loop() {
	Usb.Task();

	if (PS4.connected()) {
		int RB_velocity, LB_velocity, RF_velocity, LF_velocity;

		if ((PS4.getAnalogHat(LeftHatX)  > 135 || PS4.getAnalogHat(LeftHatX)  < 120) ||
			(PS4.getAnalogHat(LeftHatY)  > 135 || PS4.getAnalogHat(LeftHatY)  < 120) ||
			(PS4.getAnalogHat(RightHatX) > 135 || PS4.getAnalogHat(RightHatX) < 120) ||
			(PS4.getAnalogHat(RightHatY) > 135 || PS4.getAnalogHat(RightHatY) < 120) ) {
			uint8_t Yright, Xright, Xleft, Yleft;
			Xleft  = PS4.getAnalogHat(LeftHatX);
			Yleft  = PS4.getAnalogHat(LeftHatY);
			Xright = PS4.getAnalogHat(RightHatX);
			Yright = PS4.getAnalogHat(RightHatY);

			int speedX   = map(Xleft,  0, 255, -255, 255);
			int speedY   = map(Yleft,  0, 255, -255, 255);
			int rotation = map(Xright, 0, 255, -255, 255);

			LF_velocity = speedY - speedX + rotation;
			RF_velocity = speedY + speedX - rotation;
			LB_velocity = speedY + speedX + rotation;
			RB_velocity = speedY - speedX - rotation;

			Serial.print("XL = ");		Serial.print(Xleft);
			Serial.print(" - YL = ");	Serial.print(Yleft);
			Serial.print("\t - XR = ");	Serial.print(Xright);
			Serial.print(" - YR = ");	Serial.println(Yright);

		} else {
			LF_velocity = 0;
			RF_velocity = 0;
			LB_velocity = 0;
			RB_velocity = 0;
		}

		if (PS4.getButtonPress(UP)) {
			LF_velocity = 255;
		}
		if (PS4.getButtonPress(RIGHT)) {
			RF_velocity = 255;
		}
		if (PS4.getButtonPress(DOWN)) {
			LB_velocity = 255;
		}
		if (PS4.getButtonPress(LEFT)) {
			RB_velocity = 255;
		}


		if (PS4.getAnalogButton(L2) || PS4.getAnalogButton(R2)) { // These are the only analog buttons on the PS4 controller
			Serial.print(F("\r\nL2: "));
			Serial.print(PS4.getAnalogButton(L2));
			Serial.print(F("\tR2: "));
			Serial.print(PS4.getAnalogButton(R2));
		}
		if (PS4.getAnalogButton(L2) != oldL2Value || PS4.getAnalogButton(R2) != oldR2Value) // Only write value if it's different
			PS4.setRumbleOn(PS4.getAnalogButton(L2), PS4.getAnalogButton(R2));
		oldL2Value = PS4.getAnalogButton(L2);
		oldR2Value = PS4.getAnalogButton(R2);

		if (PS4.getButtonClick(PS)) {
			Serial.print(F("\r\nPS"));
			PS4.disconnect();
		}
		if (PS4.getButtonClick(TRIANGLE))
			Serial.print(F("\r\nTriangle"));
		if (PS4.getButtonClick(CIRCLE))
			Serial.print(F("\r\nCircle"));
		if (PS4.getButtonClick(CROSS))
			Serial.print(F("\r\nCross"));
		if (PS4.getButtonClick(SQUARE))
			Serial.print(F("\r\nSquare"));


		if (PS4.getButtonClick(L1))
			Serial.print(F("\r\nL1"));
		if (PS4.getButtonClick(L3))
			Serial.print(F("\r\nL3"));
		if (PS4.getButtonClick(R1))
			Serial.print(F("\r\nR1"));
		if (PS4.getButtonClick(R3))
			Serial.print(F("\r\nR3"));

		if (PS4.getButtonClick(SHARE))
			Serial.print(F("\r\nShare"));
		if (PS4.getButtonClick(OPTIONS)) {
			Serial.print(F("\r\nOptions"));
			printAngle = !printAngle;
		}
		if (PS4.getButtonClick(TOUCHPAD)) {
			Serial.print(F("\r\nTouchpad"));
			printTouch = !printTouch;
		}

		if (printAngle) { // Print angle calculated using the accelerometer only
			Serial.print(F("\r\nPitch: "));
			Serial.print(PS4.getAngle(Pitch));
			Serial.print(F("\tRoll: "));
			Serial.print(PS4.getAngle(Roll));
		}

		if (printTouch) { // Print the x, y coordinates of the touchpad
			if (PS4.isTouching(0) || PS4.isTouching(1)) // Print newline and carriage return if any of the fingers are touching the touchpad
				Serial.print(F("\r\n"));
			for (uint8_t i = 0; i < 2; i++) { // The touchpad track two fingers
				if (PS4.isTouching(i)) { // Print the position of the finger if it is touching the touchpad
					Serial.print(F("X")); Serial.print(i + 1); Serial.print(F(": "));
					Serial.print(PS4.getX(i));
					Serial.print(F("\tY")); Serial.print(i + 1); Serial.print(F(": "));
					Serial.print(PS4.getY(i));
					Serial.print(F("\t"));
				}
			}
		}


		// Determine rotation for the motors and change neg to pos pwm value
		if (LF_velocity < 0) { Motor::pcf.digitalWrite(0, LOW); Motor::pcf.digitalWrite(1, HIGH); LF_velocity *= -2;}
		else { Motor::pcf.digitalWrite(0, HIGH); Motor::pcf.digitalWrite(1, LOW); LF_velocity *= 2;}
		if (LF_velocity > 255) LF_velocity = 255;


		if (RF_velocity < 0) { Motor::pcf.digitalWrite(2, LOW); Motor::pcf.digitalWrite(3, HIGH); RF_velocity *= -2;}
		else { Motor::pcf.digitalWrite(2, HIGH); Motor::pcf.digitalWrite(3, LOW); RF_velocity *= 2;}
		if (RF_velocity > 255) RF_velocity = 255;


		if (LB_velocity < 0) { Motor::pcf.digitalWrite(4, LOW); Motor::pcf.digitalWrite(5, HIGH); LB_velocity *= -2;}
		else { Motor::pcf.digitalWrite(4, HIGH); Motor::pcf.digitalWrite(5, LOW); LB_velocity *= 2;}
		if (LB_velocity > 255) LB_velocity = 255;


		if (RB_velocity < 0) { Motor::pcf.digitalWrite(6, HIGH); Motor::pcf.digitalWrite(7, HIGH); RB_velocity *= -2;}
		else { Motor::pcf.digitalWrite(6, LOW); Motor::pcf.digitalWrite(7, LOW); RB_velocity *= 2;}
		if (RB_velocity > 255) RB_velocity = 255;

		analogWrite(A0, LF_velocity > 15 ? LF_velocity : 0);
		analogWrite(A1, LB_velocity > 15 ? LB_velocity : 0);
		analogWrite(A2, RF_velocity > 15 ? RF_velocity : 0);
		analogWrite(A3, RB_velocity > 15 ? RB_velocity : 0);


		Serial.print("\t\t\t\tFL = ");			Serial.print(LF_velocity);
		Serial.print(" - FR = ");		Serial.print(RF_velocity);
		Serial.print("\t - BL = ");		Serial.print(LB_velocity);
		Serial.print(" - BR = ");		Serial.println(RB_velocity);
		Serial.println();


	}
	delay(250);
}
