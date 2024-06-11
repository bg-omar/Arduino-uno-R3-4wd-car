//
// Created by mr on 11/15/2023.
//

#ifndef ARDUINO_WEMOS_LOLIN32_LITE_MOTOR_H
#define ARDUINO_WEMOS_LOLIN32_LITE_MOTOR_H

const int motorPins[]={32,33,25,26};    //define pins connected to four phase ABCD of stepper motor
const int CCWStep[]={0x01,0x02,0x04,0x08};  //define power supply order for coil for rotating anticlockwise
const int CWStep[]={0x08,0x04,0x02,0x01};   //define power supply order for coil for rotating clockwise

class motor {
public:
    static void moveOnePeriod(int dir,int ms);
    static void moveSteps(int dir, int ms, int steps);
};


#endif //ARDUINO_WEMOS_LOLIN32_LITE_MOTOR_H
