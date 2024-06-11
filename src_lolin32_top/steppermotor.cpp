//
// Created by mr on 11/15/2023.
//
#include "Arduino.h"
#include "steppermotor.h"

//as for four phase stepping motor, four steps is a cycle. the function is used to drive the stepping motor clockwise or anticlockwise to take four steps
void motor::moveOnePeriod(int dir,int ms){
    for (int j=0;j<4;j++){  //cycle according to power supply order
        for (int i=0;i<4;i++){  //assign to each pin, a total of 4 pins
            if(dir == 1)
                digitalWrite(motorPins[i],(CCWStep[j] == (1<<i)) ? HIGH : LOW);
            else
                digitalWrite(motorPins[i],(CWStep[j] == (1<<i)) ? HIGH : LOW);
        }
        if(ms<3)        //the delay can not be less than 3ms, otherwise it will exceed speed limit of the motor
            ms=3;
        delay(ms);
    }
}

void motor::moveSteps(int dir, int ms, int steps){
    for(int i=0;i<steps;i++){
        moveOnePeriod(dir,ms);
    }
}
