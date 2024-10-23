//
// Created by mr on 6/18/2024.
//
#include "PS4controls.h"
#include "motor.h"
#include "main.h"

#include <PS4BT.h>
#include <usbhub.h>
// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode
//PS4BT PS4(&Btd, PAIR);
// After that you can simply create the instance like so and then press the PS button on the device
PS4BT PS4(&Btd);
bool printAngle5, printTouch5;
uint8_t oldL2Value, oldR2Value;

unsigned long lastActivityTime = 0;
unsigned long currentMillis;

int r = 255;
int g = 0;
int b = 0;

// Calculates the next value in a rainbow sequence
void nextRainbowColor() {
	if (r > 0 && b == 0) { r--;  g++; }
	if (g > 0 && r == 0) { g--;  b++; }
	if (b > 0 && g == 0) { r++;  b--; }
}


void PS4controls::PS4setup() {
	while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
	if (Usb.Init() == -1) {
		Serial.println("OSC did not start");
		while (1); //halt
	}
	Serial.println("PS4 USB Library Started");
}

void PS4controls::PS4loop() {
	Usb.Task();

	if (PS4.connected()) {
		PS4.setLed(r, g, b);
		nextRainbowColor();
		int FL_velocity, BR_velocity, BL_velocity, FR_velocity;

		if (PS4.getAnalogHat(LeftHatX) > 137 || PS4.getAnalogHat(LeftHatX) < 117 ||
			PS4.getAnalogHat(LeftHatY) > 137 || PS4.getAnalogHat(LeftHatY) < 117 ||
			PS4.getAnalogHat(RightHatX) > 137 || PS4.getAnalogHat(RightHatX) < 117 ||
			PS4.getAnalogHat(RightHatY) > 137 || PS4.getAnalogHat(RightHatY) < 117) {

			uint8_t Yright = PS4.getAnalogHat(RightHatY);
			uint8_t Xright = PS4.getAnalogHat(RightHatX);
			uint8_t Xleft = PS4.getAnalogHat(LeftHatX);
			uint8_t Yleft = PS4.getAnalogHat(LeftHatY);

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

			FR_velocity = speedY + speedX + rotation ;
			BR_velocity = speedY + speedX - rotation + drifting;
			BL_velocity = speedY - speedX + rotation + drifting;
			FL_velocity = speedY - speedX - rotation ;

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
		delay(5);

/*
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
		else {
			if (PS4.getButtonClick(TRIANGLE)) {
				Serial.print(F("\r\nTriangle"));
				PS4.setRumbleOn(RumbleLow);
			}
			if (PS4.getButtonClick(CIRCLE)) {
				Serial.print(F("\r\nCircle"));
				PS4.setRumbleOn(RumbleHigh);
			}
			if (PS4.getButtonClick(CROSS)) {
				Serial.print(F("\r\nCross"));
				PS4.setLedFlash(10, 10); // Set it to blink rapidly
			}
			if (PS4.getButtonClick(SQUARE)) {
				Serial.print(F("\r\nSquare"));
				PS4.setLedFlash(0, 0); // Turn off blinking
			}

			if (PS4.getButtonClick(UP)) {
				Serial.print(F("\r\nUp"));
				PS4.setLed(Red);
			} if (PS4.getButtonClick(RIGHT)) {
				Serial.print(F("\r\nRight"));
				PS4.setLed(Blue);
			} if (PS4.getButtonClick(DOWN)) {
				Serial.print(F("\r\nDown"));
				PS4.setLed(Yellow);
			} if (PS4.getButtonClick(LEFT)) {
				Serial.print(F("\r\nLeft"));
				PS4.setLed(Green);
			}

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
				printAngle5 = !printAngle5;
			}
			if (PS4.getButtonClick(TOUCHPAD)) {
				Serial.print(F("\r\nTouchpad"));
				printTouch5 = !printTouch5;
			}

			if (printAngle5) { // Print angle calculated using the accelerometer only
				Serial.print(F("\r\nPitch: "));
				Serial.print(PS4.getAngle(Pitch));
				Serial.print(F("\tRoll: "));
				Serial.print(PS4.getAngle(Roll));
			}

			if (printTouch5) { // Print the x, y coordinates of the touchpad
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
		}*/

	}

}
