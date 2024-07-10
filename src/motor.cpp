//
// Created by mr on 11/13/2023.
//

#include "motor.h"
#include <Adafruit_PCF8574.h>
#include "main.h"

/* Example for 8 output LEDs that are connected from power to the GPIO expander pins
 * Note the LEDs must be connected with the CATHODES to the expander, to SINK current!
 * The PCF8574 cannot SOURCE current!
 */

Adafruit_PCF8574  Motor::pcf;

/********************************************** the function to run motor **************************************/
// section motor function
/***************************************************************************************************************/

void Motor::Car_front(){
    Serial.println("Car front");
    analogWrite(LF_PWM,100);
    analogWrite(LB_PWM,100);
    analogWrite(RF_PWM,100);
    analogWrite(RB_PWM,100);
}

void Motor::Car_left(){
    // Serial.println('L');
    analogWrite(LF_PWM,200);
    analogWrite(LB_PWM,100);
    analogWrite(RF_PWM,100);
    analogWrite(RB_PWM,200);
}
void Motor::Car_right(){
    //Serial.println('R');
    analogWrite(LF_PWM,100);
    analogWrite(LB_PWM,200);
    analogWrite(RF_PWM,200);
    analogWrite(RB_PWM,100);
}
void Motor::Car_Stop(){
    //Serial.println("Car_Stop");
    analogWrite(LF_PWM,0);
    analogWrite(LB_PWM,0);
    analogWrite(RF_PWM,0);
    analogWrite(RB_PWM,0);
}

void Motor::Car_Back(){
    //Serial.println('B');
    analogWrite(LF_PWM,200);
    analogWrite(LB_PWM,200);
    analogWrite(RF_PWM,200);
    analogWrite(RB_PWM,200);
}

void Motor::motor_setup() {

    Serial.println("Adafruit PCF8574 4WD motors");

    if (!pcf.begin()) {
        Serial.println("Couldn't find PCF8574");
    }
    pinMode(LF_PWM, OUTPUT);
    pinMode(LB_PWM, OUTPUT);
    pinMode(RF_PWM, OUTPUT);
    pinMode(RB_PWM, OUTPUT);
	Serial.print("PWM using: ");
	Serial.print(LF_PWM);Serial.print(", ");
	Serial.print(LB_PWM);Serial.print(", ");
	Serial.print(RF_PWM);Serial.print(", ");
	Serial.print(RB_PWM);Serial.println(" ");


	Serial.print("i2c make PCF8574 ");
	for (uint8_t p=0; p<8; p++) {
        pcf.pinMode(p, OUTPUT);
		Serial.print(p);
		Serial.print(", ");
    }
	Serial.println(" ");
    delay(500);
}
