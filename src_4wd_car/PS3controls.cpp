//
// Created by mr on 3/23/2024.
//

#include "PS3controls.h"

#include <PS3USB.h>
#include <Wire.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

#include "SPI.h"
#include "motor.h"

USB Usb;
boolean printAngle;
uint8_t state = 0;
/* You can create the instance of the class in two ways */
//PS3USB PS3(&Usb); // This will just create the instance
PS3USB PS3(&Usb,0x00,0x15,0x83,0x3D,0x0A,0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

void PS3controls::PS3setup() {
        while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
        if (Usb.Init() == -1) {
            Serial.print(F("\r\nOSC did not start"));
            while (1); //halt
        }
        Serial.print(F("\r\nPS3 USB Library Started"));
};

void PS3controls::PS3loop() {
            Usb.Task();
            if (PS3.PS3Connected) {
                int RB_velocity, LB_velocity, RF_velocity, LF_velocity;


                if (PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117 ||
                    PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117 ||
                    PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117 ||
                    PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117) {

                    LF_velocity = (PS3.getAnalogHat(LeftHatY) - 128) - (PS3.getAnalogHat(LeftHatX) - 128);
                    LB_velocity = (PS3.getAnalogHat(LeftHatY) - 128) - (PS3.getAnalogHat(LeftHatX) - 128);
                    RF_velocity = (PS3.getAnalogHat(LeftHatY) - 128) + (PS3.getAnalogHat(LeftHatX) - 128);
                    RB_velocity = (PS3.getAnalogHat(LeftHatY) - 128) + (PS3.getAnalogHat(LeftHatX) - 128);

                    Serial.print(F("\r\nLF_velocity: "));
                    Serial.print(LF_velocity);
                    Serial.print(F("\tLB_velocity: "));
                    Serial.print(LB_velocity);
                    Serial.print(F("\tRF_velocity: "));
                    Serial.print(RF_velocity);
                    Serial.print(F("\tRB_velocity: "));
                    Serial.print(RB_velocity);

                }
                // Analog button values can be read from almost all buttons
                if (PS3.getAnalogButton(L2) || PS3.getAnalogButton(R2)) {
                    Serial.print(F("\r\nL2: "));
                    Serial.print(PS3.getAnalogButton(L2));
                    Serial.print(F("\tR2: "));
                    Serial.print(PS3.getAnalogButton(R2));

                    LF_velocity = (PS3.getAnalogButton(R2)) - (PS3.getAnalogButton(L2));
                    LB_velocity = LF_velocity;
                    RF_velocity = LF_velocity;
                    RB_velocity = LF_velocity;

                    Serial.print(F("\r\nLF_velocity: "));
                    Serial.print(LF_velocity);
                    Serial.print(F("\tLB_velocity: "));
                    Serial.print(LB_velocity);
                    Serial.print(F("\tRF_velocity: "));
                    Serial.print(RF_velocity);
                    Serial.print(F("\tRB_velocity: "));
                    Serial.print(RB_velocity);
                }
                if (PS3.getButtonClick(PS))         Serial.print(F("\r\nPS"));
                if (PS3.getButtonClick(TRIANGLE))   Serial.print(F("\r\nTraingle"));
                if (PS3.getButtonClick(CIRCLE))     Serial.print(F("\r\nCircle"));
                if (PS3.getButtonClick(CROSS))      Serial.print(F("\r\nCross"));
                if (PS3.getButtonClick(SQUARE))     Serial.print(F("\r\nSquare"));
                if (PS3.getButtonClick(UP))     { Serial.print(F("\r\nUp"));    PS3.setLedOff(); PS3.setLedOn(LED4); }
                if (PS3.getButtonClick(RIGHT))  { Serial.print(F("\r\nRight")); PS3.setLedOff(); PS3.setLedOn(LED1);  }
                if (PS3.getButtonClick(DOWN))   { Serial.print(F("\r\nDown"));  PS3.setLedOff(); PS3.setLedOn(LED2); }
                if (PS3.getButtonClick(LEFT))   { Serial.print(F("\r\nLeft"));  PS3.setLedOff(); PS3.setLedOn(LED3); }
                if (PS3.getButtonClick(L1)) Serial.print(F("\r\nL1"));
                if (PS3.getButtonClick(L3)) Serial.print(F("\r\nL3"));
                if (PS3.getButtonClick(R1)) Serial.print(F("\r\nR1"));
                if (PS3.getButtonClick(R3)) Serial.print(F("\r\nR3"));
                if (PS3.getButtonClick(SELECT)) { Serial.print(F("\r\nSelect - ")); PS3.printStatusString(); }
                if (PS3.getButtonClick(START)) { Serial.print(F("\r\nStart")); printAngle = !printAngle; }
                if (printAngle) {
                    Serial.print(F("\r\nPitch: "));
                    Serial.print(PS3.getAngle(Pitch));
                    Serial.print(F("\tRoll: "));
                    Serial.print(PS3.getAngle(Roll));
                }
                // Determine rotation for the motors and change neg to pos pwm value
                if (LF_velocity < 0) { Motor::pcf.digitalWrite(0, LOW); Motor::pcf.digitalWrite(1, HIGH); LF_velocity *= -2; /*Serial.print("_");*/} else{  Motor::pcf.digitalWrite(0, HIGH); Motor::pcf.digitalWrite(1, LOW); LF_velocity *= 2;}
                if (LF_velocity > 255) { LF_velocity = 255; } //    Serial.print(L_velocity);

                if (RF_velocity < 0)  { Motor::pcf.digitalWrite(2, LOW); Motor::pcf.digitalWrite(3, HIGH); RF_velocity *= -2; /*Serial.print("_");*/} else{  Motor::pcf.digitalWrite(2, HIGH); Motor::pcf.digitalWrite(3, LOW); RF_velocity *= 2;}
                if (RF_velocity > 255) { RF_velocity = 255; } //    Serial.println(R_velocity);

                if (LB_velocity < 0) { Motor::pcf.digitalWrite(4, LOW); Motor::pcf.digitalWrite(5, HIGH); LB_velocity *= -2; /*Serial.print("_");*/} else{  Motor::pcf.digitalWrite(4, HIGH); Motor::pcf.digitalWrite(5, LOW); LB_velocity *= 2;}
                if (LB_velocity > 255) { LB_velocity = 255; } //    Serial.print(L_velocity);

                if (RB_velocity < 0)  { Motor::pcf.digitalWrite(6, LOW); Motor::pcf.digitalWrite(7, HIGH); RB_velocity *= -2; /*Serial.print("_");*/} else{  Motor::pcf.digitalWrite(6, HIGH); Motor::pcf.digitalWrite(7, LOW); RB_velocity *= 2;}
                if (RB_velocity > 255) { RB_velocity = 255; } //    Serial.println(R_velocity);

                if (PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117 ||
                    PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117 ||
                    PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117 ||
                    PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117 ||
                    PS3.getAnalogButton(L2) || PS3.getAnalogButton(R2)) {
                    analogWrite(4,LF_velocity);
                    analogWrite(5,LB_velocity);
                    analogWrite(6,RF_velocity);
                    analogWrite(7,RB_velocity);

                } else {
                    Motor::Car_Stop();
                }
            }
    delay(250);
};