
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

        if      (sqd) send(3110);
        else if (squ) send(3101);
        else if (crd) send(3210);
        else if (cru) send(3201);
        else if (cid) send(3310);
        else if (ciu) send(3301);
        else if (trd) send(3410);
        else if (tru) send(3401);
        else if (upd) send(1110);
        else if (upu) send(1101);
        else if (rid) send(1210);
        else if (riu) send(1201);
        else if (dod) send(1310);
        else if (dou) send(1301);
        else if (led) send(1410);
        else if (leu) send(1401);

        else if (l1d) send(2110);
        else if (l1u) send(2101);
        else if (r1d) send(2210);
        else if (r1u) send(2201);
        else if (l3d) send(2310);
        else if (l3u) send(2301);
        else if (r3d) send(2410);
        else if (r3u) send(2401);
        else if (psd) send(2510);
        else if (psu) send(2501);
        else if (tpd) send(2710);
        else if (tpu) send(2701);
        else if (shd) send(2810);
        else if (shu) send(2801);
        else if (opd) send(2910);
        else if (opu) send(2901);
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



void send(const String& texting) {
    Serial.println(texting);
}



int direction = 1;
int speed = 0;




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
		int RB_velocity, LB_velocity, RF_velocity, LF_velocity;
        if (PS4.Up()) Serial.print(F("\r\nUp"));
        if (PS4.Right()) Serial.print(F("\r\nRight"));
        if (PS4.Down()) Serial.print(F("\r\nDown"));
        if (PS4.Left()) Serial.print(F("\r\nLeft"));

        if (PS4.Square()) Serial.print(F("\r\nSquare"));
        if (PS4.Cross()) Serial.print(F("\r\nCross"));
        if (PS4.Circle()) Serial.print(F("\r\nCircle"));
        if (PS4.Triangle()) Serial.print(F("\r\nTraingle"));

        if (PS4.L1()) send("\tL1");
        if (PS4.R1()) send("\tR1");

        if (PS4.L3()) send("\tL3");
        if (PS4.R3()) send("\tR3");
        if (PS4.Share()) send("\tShare");
        if (PS4.Options()) send("\tOptions");
        if (PS4.PSButton()) send("\tPS");
        if (PS4.Touchpad()) {
            Serial.print("\tBattery Level: ");
            Serial.println(PS4.Battery());
        }

#if THUMB_STICKS
        if (PS4.LStickY() <= -15 || PS4.LStickY() >= 15 || PS4.LStickX() <= -15 || PS4.LStickX() >= 15) {


			LF_velocity =  (PS4.LStickY() - 128) - (PS4.LStickX() - 128);
			RF_velocity =  (PS4.LStickY() - 128) + (PS4.LStickX() - 128);
			LB_velocity = -(PS4.LStickY() - 128) - (PS4.LStickX() - 128);
			RB_velocity = -(PS4.LStickY() - 128) + (PS4.LStickX() - 128);

			Serial.print(F("\r\nLF_velocity: ")); Serial.print(LF_velocity);
			Serial.print(F("\tLB_velocity: ")); Serial.print(LB_velocity);
			Serial.print(F("\tRF_velocity: ")); Serial.print(RF_velocity);
			Serial.print(F("\tRB_velocity: ")); Serial.print(RB_velocity);
        }

        if (PS4.RStickX() <= -15 || PS4.RStickX() >= 15 || PS4.RStickY() <= -15 || PS4.RStickY() >= 15) {

			LF_velocity =  (PS4.RStickY() - 128) - (PS4.RStickX() - 128);
			RF_velocity =  (PS4.RStickY() - 128) + (PS4.RStickX() - 128);
 			LB_velocity = -(PS4.RStickY() - 128) + (PS4.RStickX() - 128);
			RB_velocity = -(PS4.RStickY() - 128) - (PS4.RStickX() - 128);

			Serial.print(F("\r\nLF_velocity: ")); Serial.print(LF_velocity);
			Serial.print(F("\tLB_velocity: ")); Serial.print(LB_velocity);
			Serial.print(F("\tRF_velocity: ")); Serial.print(RF_velocity);
			Serial.print(F("\tRB_velocity: ")); Serial.print(RB_velocity);
        }

        if (PS4.L2Value() > 15 || PS4.R2Value() > 15) {

			LF_velocity = (PS4.R2Value() - PS4.L2Value());
			LB_velocity = -LF_velocity;
			RF_velocity = LF_velocity;
			RB_velocity = -LF_velocity;

			Serial.print(F("\r\nLF_velocity: ")); Serial.print(LF_velocity);
			Serial.print(F("\tLB_velocity: ")); Serial.print(LB_velocity);
			Serial.print(F("\tRF_velocity: ")); Serial.print(RF_velocity);
			Serial.print(F("\tRB_velocity: ")); Serial.print(RB_velocity);
        }
#endif
		// Determine rotation for the motors and change neg to pos pwm value
		if (LF_velocity < 0) { Motor::pcf.digitalWrite(0, LOW); Motor::pcf.digitalWrite(1, HIGH); LF_velocity *= -2; /*Serial.print("_");*/} else{  Motor::pcf.digitalWrite(0, HIGH); Motor::pcf.digitalWrite(1, LOW); LF_velocity *= 2;}
		if (LF_velocity > 255) { LF_velocity = 255; } //    Serial.print(L_velocity);

		if (RF_velocity < 0)  { Motor::pcf.digitalWrite(2, LOW); Motor::pcf.digitalWrite(3, HIGH); RF_velocity *= -2; /*Serial.print("_");*/} else{  Motor::pcf.digitalWrite(2, HIGH); Motor::pcf.digitalWrite(3, LOW); RF_velocity *= 2;}
		if (RF_velocity > 255) { RF_velocity = 255; } //    Serial.println(R_velocity);

		if (LB_velocity < 0) { Motor::pcf.digitalWrite(4, LOW); Motor::pcf.digitalWrite(5, HIGH); LB_velocity *= -2; /*Serial.print("_");*/} else{  Motor::pcf.digitalWrite(4, HIGH); Motor::pcf.digitalWrite(5, LOW); LB_velocity *= 2;}
		if (LB_velocity > 255) { LB_velocity = 255; } //    Serial.print(L_velocity);

		if (RB_velocity < 0)  { Motor::pcf.digitalWrite(6, LOW); Motor::pcf.digitalWrite(7, HIGH); RB_velocity *= -2; /*Serial.print("_");*/} else{  Motor::pcf.digitalWrite(6, HIGH); Motor::pcf.digitalWrite(7, LOW); RB_velocity *= 2;}
		if (RB_velocity > 255) { RB_velocity = 255; } //    Serial.println(R_velocity);

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
//        if (PS4.Battery() < 2) {
//            r = 255;
//            g = 0;
//            b = 0;
//            PS4.setFlashRate(25, 10); // 250ms on 100ms off
//        } else {
//            PS4.setFlashRate(0, 0); // no flashing
//            nextRainbowColor();
//        }
//        PS4.setLed(r, g, b);
//        PS4.sendToController();

    }
    delay(5);
}


