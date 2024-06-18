//
// Created by mr on 11/13/2023.
//

#ifndef MOTOR_H
#define MOTOR_H

#include "Adafruit_PCF8574.h"

#define LF_PWM    32
#define LB_PWM    33
#define RF_PWM    25
#define RB_PWM    26

class Motor {

public:
    static void Car_front() ;
    static void Car_left();
    static void Car_right() ;
    static void Car_Stop() ;
    static void Car_Back();
    static void motor_setup();

    static Adafruit_PCF8574 pcf;
};

#endif //MOTOR_H
