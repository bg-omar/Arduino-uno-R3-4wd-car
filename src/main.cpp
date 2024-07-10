#include <Arduino.h>
#include "main.h"
#include "Wire.h"
#include "motor.h"
#include "PS4controls.h"


/*
    UNO BLE -->     DC:54:75:C3:D9:EC   -
    PC USB Dongel   00:1F:E2:C8:82:BA
    ESP 1           66:CB:3E:E9:02:8A
    ESP             3c:e9:0e:89:80:84
    ESP small cam   3C:E9:0E:88:65:16
    PS4 Controller: A4:AE:11:E1:8B:B3 (SONYWA) GooglyEyes
    PS5 Controller: 88:03:4C:B5:00:66
*/


/*
	ESP32
	MOSI: 23
	MISO: 19
	SCK: 18
	SS: 5
	SDA: 21
	SCL: 22
 */


void setup() {
    Serial.begin(115200);
	Serial.print("UNO 4wd car + ps4 BLUE");
	PS4controls::PS4setup();
	Motor::motor_setup();
}

void loop() {
	PS4controls::PS4loop();
}







