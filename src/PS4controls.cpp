
#include "PS4controls.h"
#include "main.h"

#include <SPI.h> // Not actually used but needed to compile

#include "esp_gap_bt_api.h"
#include <PS4controller.h>
#include "motor.h"
#include <cstdint>


int r = 255;
int g = 0;
int b = 0;
int direction = 1;
int speed = 0;


// Wheels

int rightX, rightY, leftY, leftX = 0;

float brightness = 0;
bool forward = true;
// Calculates the next value in a rainbow sequence
void nextRainbowColor() {
    if (r > 0 && b == 0) { r--;  g++; }
    if (g > 0 && r == 0) { g--;  b++; }
    if (b > 0 && g == 0) { r++;  b--; }
}

void onConnect() {
    Serial.println("PS4 Connected!");
    Serial.println("");
}

void onDisConnect() {
    Serial.println("Disconnected!");
}


void notify() {
#if EVENTS
    boolean sqd = PS4.event.button_down.square,     squ = PS4.event.button_up.square,
                trd = PS4.event.button_down.triangle,   tru = PS4.event.button_up.triangle,
                crd = PS4.event.button_down.cross,      cru = PS4.event.button_up.cross,
                cid = PS4.event.button_down.circle,     ciu = PS4.event.button_up.circle,
                upd = PS4.event.button_down.up,         upu = PS4.event.button_up.up,
                rid = PS4.event.button_down.right,      riu = PS4.event.button_up.right,
                dod = PS4.event.button_down.down,       dou = PS4.event.button_up.down,
                led = PS4.event.button_down.left,       leu = PS4.event.button_up.left,
                l1d = PS4.event.button_down.l1,         l1u = PS4.event.button_up.l1,
                r1d = PS4.event.button_down.r1,         r1u = PS4.event.button_up.r1,
                l3d = PS4.event.button_down.l3,         l3u = PS4.event.button_up.l3,
                r3d = PS4.event.button_down.r3,         r3u = PS4.event.button_up.r3,
                psd = PS4.event.button_down.ps,         psu = PS4.event.button_up.ps,
                tpd = PS4.event.button_down.touchpad,   tpu = PS4.event.button_up.touchpad,
                opd = PS4.event.button_down.options,    opu = PS4.event.button_up.options,
                shd = PS4.event.button_down.share,      shu = PS4.event.button_up.share;

        if      (sqd) Serial.println(" --> sqd");
        else if (squ) Serial.println(" --> squ");
        else if (crd) Serial.println(" --> crd");
        else if (cru) Serial.println(" --> cru");
        else if (cid) Serial.println(" --> cid");
        else if (ciu) Serial.println(" --> ciu");
        else if (trd) Serial.println(" --> trd");
        else if (tru) Serial.println(" --> tru");
        else if (upd) Serial.println(" --> upd");
        else if (upu) Serial.println(" --> upu");
        else if (rid) Serial.println(" --> rid");
        else if (riu) Serial.println(" --> riu");
        else if (dod) Serial.println(" --> dod");
        else if (dou) Serial.println(" --> dou");
        else if (led) Serial.println(" --> led");
        else if (leu) Serial.println(" --> leu");

        else if (l1d) Serial.println(" --> l1d");
        else if (l1u) Serial.println(" --> l1u");
        else if (r1d) Serial.println(" --> r1d");
        else if (r1u) Serial.println(" --> r1u");
        else if (l3d) Serial.println(" --> l3d");
        else if (l3u) Serial.println(" --> l3u");
        else if (r3d) Serial.println(" --> r3d");
        else if (r3u) Serial.println(" --> r3u");
        else if (psd) Serial.println(" --> psd");
        else if (psu) Serial.println(" --> psu");
        else if (tpd) Serial.println(" --> tpd");
        else if (tpu) Serial.println(" --> tpu");
        else if (shd) Serial.println(" --> shd");
        else if (shu) Serial.println(" --> shu");
        else if (opd) Serial.println(" --> opd");
        else if (opu) Serial.println(" --> opu");
#endif

#if JOYSTICKS
    Serial.printf("%4d, %4d, %4d, %4d, %4d, %4d \r\n",
                      (PS4.LStickX() <= -15 || PS4.LStickX() >= 15 ) ? 6127 + PS4.LStickX() : 6127,
                      (PS4.LStickY() <= -15 || PS4.LStickY() >= 15 ) ? 7127 + PS4.LStickY() : 7127,
                      (PS4.RStickX() <= -15 || PS4.RStickX() >= 15 ) ? 8127 + PS4.RStickX() : 8127,
                      (PS4.RStickY() <= -15 || PS4.RStickY() >= 15 ) ? 9127 + PS4.RStickY() : 9127,
                      (PS4.L2()) ? PS4.L2Value() : 0,
                      (PS4.R2()) ? PS4.R2Value() : 0
        );
#endif

#if SENSORS
    Serial.printf("gx%5dgy%5dgz%5d", PS4.GyrX(), PS4.GyrY(), PS4.GyrZ());
        Serial.printf("ax%5day%5daz%5d", PS4.AccX(), PS4.AccY(), PS4.AccZ());
#endif
}





void PS4controls::PS4setup() {
    PS4.attach(notify);
    PS4.attachOnConnect(onConnect);
    PS4.attachOnDisconnect(onDisConnect);
    PS4.begin("3C:E9:0E:88:65:16"); //mac Address that ESP should use

    /* Remove Paired Devices  */
    uint8_t pairedDeviceBtAddr[20][6];
    int count = esp_bt_gap_get_bond_device_num();
    esp_bt_gap_get_bond_device_list(&count, pairedDeviceBtAddr);
    for (int i = 0; i < count; i++) {
        esp_bt_gap_remove_bond_device(pairedDeviceBtAddr[i]);
    }
    Serial.println("my BT mac -> 3C:E9:0E:88:65:16");
    Serial.println("Ready for PS4");
}

void PS4controls::PS4loop() {
    if (PS4.isConnected()) {
        if (PS4.Up()) Serial.print(F("\r\nUp"));
        if (PS4.Right()) Serial.print(F("\r\nRight"));
        if (PS4.Down()) Serial.print(F("\r\nDown"));
        if (PS4.Left()) Serial.print(F("\r\nLeft"));

        if (PS4.Square()) Serial.print(F("\r\nSquare"));
        if (PS4.Cross()) Serial.print(F("\r\nCross"));
        if (PS4.Circle()) Serial.print(F("\r\nCircle"));
        if (PS4.Triangle()) Serial.print(F("\r\nTraingle"));

        if (PS4.L1()) Serial.println("\tL1");
        if (PS4.R1()) Serial.println("\tR1");

        if (PS4.L3()) Serial.println("\tL3");
        if (PS4.Share()) Serial.println("\tShare");
        if (PS4.Options()) Serial.println("\tOptions");
        if (PS4.PSButton()) Serial.println("\tPS");
        if (PS4.Touchpad()) {
            Serial.print("\tBattery Level: ");
            Serial.println(PS4.Battery());
        }

		#if THUMB_STICKS
			int RB_velocity, LB_velocity, RF_velocity, LF_velocity = 0;

			rightX = PS4.RStickX();
			rightY = PS4.RStickY();
			leftX = PS4.LStickX();
			leftY = PS4.LStickY();

			// Movement
			int speedX = map(leftX, -128, 127, -255, 255);
			int speedY = map(leftY, -128, 127, -255, 255);
			int rotation = map(rightX, -128, 127, -255, 255);

			LF_velocity = speedY - speedX + rotation;
			RF_velocity = speedY + speedX - rotation;
			LB_velocity = speedY + speedX + rotation;
			RB_velocity = speedY - speedX - rotation;

			Serial.print("RightX = ");
			Serial.print(rightX);
			Serial.print(" - RightY = ");
			Serial.print(rightY);
			Serial.print(" - LeftX = ");
			Serial.print(leftX);
			Serial.print(" - FL = ");
			Serial.print(LF_velocity);
			Serial.print(" - FR = ");
			Serial.print(RF_velocity);
			Serial.print(" - BL = ");
			Serial.print(LB_velocity);
			Serial.print(" - BR = ");
			Serial.print(RB_velocity);
			Serial.println();
		#endif

		// Determine rotation for the motors and change neg to pos pwm value
		if (LF_velocity < 0) { Motor::pcf.digitalWrite(0, LOW); Motor::pcf.digitalWrite(1, HIGH); LF_velocity *= -2;
		} else{  Motor::pcf.digitalWrite(0, HIGH); Motor::pcf.digitalWrite(1, LOW); LF_velocity *= 2;}
		if (LF_velocity > 255) { LF_velocity = 255; }    Serial.print(LF_velocity);

		if (RF_velocity < 0)  { Motor::pcf.digitalWrite(2, LOW); Motor::pcf.digitalWrite(3, HIGH); RF_velocity *= -2;
		} else{  Motor::pcf.digitalWrite(2, HIGH); Motor::pcf.digitalWrite(3, LOW); RF_velocity *= 2;}
		if (RF_velocity > 255) { RF_velocity = 255; }     Serial.println(RF_velocity);

		if (LB_velocity < 0) { Motor::pcf.digitalWrite(4, LOW); Motor::pcf.digitalWrite(5, HIGH); LB_velocity *= -2;
		} else{  Motor::pcf.digitalWrite(4, HIGH); Motor::pcf.digitalWrite(5, LOW); LB_velocity *= 2;}
		if (LB_velocity > 255) { LB_velocity = 255; }    Serial.print(LB_velocity);

		if (RB_velocity < 0)  { Motor::pcf.digitalWrite(6, LOW); Motor::pcf.digitalWrite(7, HIGH); RB_velocity *= -2;
		} else{  Motor::pcf.digitalWrite(6, HIGH); Motor::pcf.digitalWrite(7, LOW); RB_velocity *= 2;}
		if (RB_velocity > 255) { RB_velocity = 255; }    Serial.println(RB_velocity);

		if (PS4.LStickY() <= -15 || PS4.LStickY() >= 15 || PS4.LStickX() <= -15 || PS4.LStickX() >= 15 ||
			PS4.RStickX() <= -15 || PS4.RStickX() >= 15 || PS4.RStickY() <= -15 || PS4.RStickY() >= 15 ||
			PS4.L2Value() > 15 || PS4.R2Value() > 15) {
			analogWrite(4,LF_velocity);
			analogWrite(5,LB_velocity);
			analogWrite(6,RF_velocity);
			analogWrite(7,RB_velocity);

		} else {
			Motor::Car_Stop();
		}
        if (PS4.Battery() < 2) {
            r = 255;
            g = 0;
            b = 0;
            PS4.setFlashRate(25, 10); // 250ms on 100ms off
        } else {
            PS4.setFlashRate(0, 0); // no flashing
            nextRainbowColor();
        }
        PS4.setLed(r, g, b);
        PS4.sendToController();

    }
    delay(5);
}


