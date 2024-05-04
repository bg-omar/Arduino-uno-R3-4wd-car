#include <Arduino.h>

#include "PS3controls.h"
#include "motor.h"


void setup() {
    while (!Serial) { delay(10); }
    Serial.begin(115200);

    Motor::motor_setup();

    PS3controls::PS3setup();
}

void loop() {
    PS3controls::PS3loop();
}


