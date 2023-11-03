#include <Arduino.h>
#include <stdio.h>
#include <PS3USB.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
USB Usb;
/* You can create the instance of the class in two ways */
PS3USB PS3(&Usb); // This will just create the instance
//PS3USB PS3(&Usb,0x00,0x15,0x83,0x3D,0x0A,0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

bool printAngle;
uint8_t state = 0;

const int motorPins[]={4,5,6,7};    //define pins connected to four phase ABCD of stepper motor
const int CCWStep[]={0x01,0x02,0x04,0x08};  //define power supply order for coil for rotating anticlockwise
const int CWStep[]={0x08,0x04,0x02,0x01};   //define power supply order for coil for rotating clockwise
//as for four phase stepping motor, four steps is a cycle. the function is used to drive the stepping motor clockwise or anticlockwise to take four steps
void moveOnePeriod(int dir,int ms){
    int i=0,j=0;
    for (j=0;j<4;j++){  //cycle according to power supply order
        for (i=0;i<4;i++){  //assign to each pin, a total of 4 pins
            if(dir == 1)
                digitalWrite(motorPins[i],(CCWStep[j] == (1<<i)) ? HIGH : LOW);
            else
                digitalWrite(motorPins[i],(CWStep[j] == (1<<i)) ? HIGH : LOW);
            //"Serial.print("motorPin %d, %d \n" + motorPins[i] + digitalRead(motorPins[i]));
        }
        //Serial.println("Step cycle!\n");
        if(ms<3)        //the delay can not be less than 3ms, otherwise it will exceed speed limit of the motor
            ms=3;
        delay(ms);
    }
}
//continuous rotation function, the parameter steps specifies the rotation cycles, every four steps is a cycle
void moveSteps(int dir, int ms, int steps){
    int i;
    for(i=0;i<steps;i++){
        moveOnePeriod(dir,ms);
    }
}
void motorStop(){   //function used to stop rotating
    int i;
    for(i=0;i<4;i++){
        digitalWrite(motorPins[i],LOW);
    }
}
void setup() {
    Serial.begin(115200);
    Serial.println("Serial started");
    int i;
    for(i=0;i<4;i++){
        Serial.print("Setup Pin: ");
        Serial.println(motorPins[i]);
        pinMode(motorPins[i],OUTPUT);
    }
    if (Usb.Init() == -1) {
        Serial.println(("\r\nOSC did not start"));
    }
    Serial.println(("\r\nPS3 USB Library Started"));

}

void loop() {
    Usb.Task();

    if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
        if (PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117 || PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117 || PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117 || PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117) {
            Serial.println(("\r\nLeftHatX: "));
            Serial.print(PS3.getAnalogHat(LeftHatX));
            Serial.println(("\tLeftHatY: "));
            Serial.print(PS3.getAnalogHat(LeftHatY));
            if (PS3.PS3Connected) { // The Navigation controller only have one joystick
                Serial.println(("\tRightHatX: "));
                Serial.print(PS3.getAnalogHat(RightHatX));
                Serial.println(("\tRightHatY: "));
                Serial.print(PS3.getAnalogHat(RightHatY));
            }
        }
        // Analog button values can be read from almost all buttons
        if (PS3.getAnalogButton(L2) || PS3.getAnalogButton(R2)) {
            Serial.println(("\r\nL2: "));
            Serial.print(PS3.getAnalogButton(L2));
            if (!PS3.PS3NavigationConnected) {
                Serial.println(("\tR2: "));
                Serial.print(PS3.getAnalogButton(R2));
            }
        }
        if (PS3.getButtonClick(PS))
            Serial.println(("\r\nPS"));

        if (PS3.getButtonClick(TRIANGLE))
            Serial.println(("\r\nTriangle"));
        if (PS3.getButtonClick(CIRCLE))
            Serial.println(("\r\nCircle"));
        if (PS3.getButtonClick(CROSS))
            Serial.println(("\r\nCross"));
        if (PS3.getButtonClick(SQUARE))
            Serial.println(("\r\nSquare"));

        if (PS3.getButtonClick(UP)) {
            Serial.println(("\r\nUp"));
            PS3.setLedOff();
            PS3.setLedOn(LED4);
        }
        if (PS3.getButtonClick(RIGHT)) {
            Serial.println(("\r\nRight"));
            PS3.setLedOff();
            PS3.setLedOn(LED1);
        }
        if (PS3.getButtonClick(DOWN)) {
            Serial.println(("\r\nDown"));
            PS3.setLedOff();
            PS3.setLedOn(LED2);
        }
        if (PS3.getButtonClick(LEFT)) {
            Serial.println(("\r\nLeft"));
            PS3.setLedOff();
            PS3.setLedOn(LED3);
        }

        if (PS3.getButtonClick(L1))
            Serial.println(("\r\nL1"));
        if (PS3.getButtonClick(L3))
            Serial.println(("\r\nL3"));
        if (PS3.getButtonClick(R1))
            Serial.println(("\r\nR1"));
        if (PS3.getButtonClick(R3))
            Serial.println(("\r\nR3"));

        if (PS3.getButtonClick(SELECT)) {
            Serial.println(("\r\nSelect - "));
            PS3.printStatusString();
        }
        if (PS3.getButtonClick(START)) {
            Serial.println(("\r\nStart"));
            printAngle = !printAngle;
        }
        if (printAngle) {
            Serial.println(("\r\nPitch: "));
            Serial.print(PS3.getAngle(Pitch));
            Serial.println(("\tRoll: "));
            Serial.print(PS3.getAngle(Roll));
        }
    }
    else if (PS3.PS3MoveConnected) { // One can only set the color of the bulb, set the rumble, set and get the bluetooth address and calibrate the magnetometer via USB
        if (state == 0) {
            PS3.moveSetRumble(0);
            PS3.moveSetBulb(Off);
        } else if (state == 1) {
            PS3.moveSetRumble(75);
            PS3.moveSetBulb(Red);
        } else if (state == 2) {
            PS3.moveSetRumble(125);
            PS3.moveSetBulb(Green);
        } else if (state == 3) {
            PS3.moveSetRumble(150);
            PS3.moveSetBulb(Blue);
        } else if (state == 4) {
            PS3.moveSetRumble(175);
            PS3.moveSetBulb(Yellow);
        } else if (state == 5) {
            PS3.moveSetRumble(200);
            PS3.moveSetBulb(Lightblue);
        } else if (state == 6) {
            PS3.moveSetRumble(225);
            PS3.moveSetBulb(Purple);
        } else if (state == 7) {
            PS3.moveSetRumble(250);
            PS3.moveSetBulb(White);
        }

        state++;
        if (state > 7)
            state = 0;
        delay(1000);
    }
    moveSteps(1,4,1);     //rotating   360° clockwise, a total of 2048 steps in a circle, namely, 512 cycles.
}
