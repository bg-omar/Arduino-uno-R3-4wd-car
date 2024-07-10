/*
UNO BLE -->     DC:54:75:C3:D9:EC   -
PC USB Dongel   00:1F:E2:C8:82:BA
ESP 1           66:CB:3E:E9:02:8A
ESP             3c:e9:0e:89:80:84
ESP small cam   3C:E9:0E:88:65:16
PS4 Controller: A4:AE:11:E1:8B:B3 (SONYWA) GooglyEyes
PS5 Controller: 88:03:4C:B5:00:66
*/
/********************************************** Setup booting the arduino **************************************/
// section Defines
/***************************************************************************************************************/

/********************************************** Setup booting the arduino **************************************/
// section Includes
/***************************************************************************************************************/

#include <arduino.h>
#include <PS4Controller.h>


/********************************************** Setup booting the arduino **************************************/
// section Variables & Defines
/***************************************************************************************************************/

#define EVENTS 1
#define BUTTONS 1
#define JOYSTICKS 0
#define SENSORS 0
#define THUMB_STICKS 1

/********************************************** Setup booting the arduino **************************************/
// section Functions
/***************************************************************************************************************/


int r = 255;
int g = 0;
int b = 0;

// Calculates the next value in a rainbow sequence
void nextRainbowColor() {
    if (r > 0 && b == 0) { r--;  g++; }
    if (g > 0 && r == 0) { g--;  b++; }
    if (b > 0 && g == 0) { r++;  b--; }
}

void onConnect() {
    Serial.println("PS4 Connected!");
}

void onDisConnect() {
    Serial.println("Disconnected!");
}

void send(char32_t texting) {
	Serial.println(texting);
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
//            r3d = PS4.event.button_down.r3,         r3u = PS4.event.button_up.r3,
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
//    else if (r3d) send(2410);
//    else if (r3u) send(2401);
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


/********************************************** Setup booting the arduino **************************************/
// section Setup
/***************************************************************************************************************/

void setup() {
    Serial.begin(115200);
    PS4.attach(notify);
    PS4.attachOnConnect(onConnect);
    PS4.attachOnDisconnect(onDisConnect);
    PS4.begin("3C:E9:0E:88:65:16"); //mac Address that ESP should use

    Serial.println("my BT mac -> 3C:E9:0E:88:65:16");
    Serial.println("Ready for PS4");
}

void loop() {
    if (PS4.isConnected()) {
        #if BUTTONS
            if (PS4.Up())        send(1100);
            if (PS4.Right())     send(1200);
            if (PS4.Down())      send(1300);
            if (PS4.Left())      send(1400);

            if (PS4.L1())        send(2100);
            if (PS4.R1())        send(2200);

            if (PS4.L3())        send(2300);
//            if (PS4.R3())        send(2400);

            if (PS4.PSButton())  send(2500);
            if (PS4.Touchpad()) {
                send(2700);
                Serial.print("Battery Level: ");
                Serial.println(PS4.Battery());
            }
            if (PS4.Share())     send(2800);
            if (PS4.Options())   send(2900);

            if (PS4.Square())    send(3100);
            if (PS4.Cross())     send(3200);
            if (PS4.Circle())    send(3300);
            if (PS4.Triangle())  send(3400);

        #endif

        #if THUMB_STICKS
            if (
				PS4.LStickY() <= -25 || PS4.LStickY() >= 25 ||
				PS4.LStickX() <= -25 || PS4.LStickX() >= 25 ||
				PS4.RStickX() <= -25 || PS4.RStickX() >= 25 ||
				PS4.RStickY() <= -25 || PS4.RStickY() >= 25 ) {
                Serial.printf("%4d+%4d+%4d+%4d \r\n",
                              (PS4.LStickX() <= -10 || PS4.LStickX() >= 10) ? 6127 + PS4.LStickX() : 6127,
                              (PS4.LStickY() <= -10 || PS4.LStickY() >= 10) ? 7127 + PS4.LStickY() : 7127,
                              (PS4.RStickX() <= -10 || PS4.RStickX() >= 10) ? 8127 + PS4.RStickX() : 8127,
                              (PS4.RStickY() <= -10 || PS4.RStickY() >= 10) ? 9127 + PS4.RStickY() : 9127
                );
            }
            if(PS4.L2Value() > 45 || PS4.R2Value() > 45) {
                Serial.printf("%4d+%4d \r\n",
                              (PS4.L2()) ? 4000 + PS4.L2Value() : 4000,
                              (PS4.R2()) ? 5000 + PS4.R2Value() : 5000
                );
            }
        #elif BUTTONS
            if (PS4.L2()) { Serial.println(4000 + PS4.L2Value());  }
                if (PS4.R2()) { Serial.println(5000 + PS4.R2Value());  }
                if (PS4.LStickX() <= -10 || PS4.LStickX() >= 10 ) { Serial.println(6127 + PS4.LStickX()); } // 6 000 - 6 254
                if (PS4.LStickY() <= -10 || PS4.LStickY() >= 10 ) { Serial.println(7127 + PS4.LStickY()); } // 7 000 - 7 254

                if (PS4.RStickX() <= -10 || PS4.RStickX() >= 10 ) { Serial.println(8127 + PS4.RStickX()); } // 8 000 - 8 254
                if (PS4.RStickY() <= -10 || PS4.RStickY() >= 10 ) { Serial.println(9127 + PS4.RStickY()); } // 9 000 - 9 254
        #endif

        if (PS4.Battery() < 2) {
            r = 255; g = 0;  b = 0;
            PS4.setFlashRate(25,10); // 250ms on 100ms off
        } else {
            PS4.setFlashRate(0,0); // no flashing
            nextRainbowColor();
        }

        PS4.setLed(r, g, b);
        PS4.sendToController();
        delay(50);

        // Params: Weak rumble intensity, Strong rumble intensity Range: 0->255
        // PS4.setRumble(PS4.L2Value(), PS4.R2Value());
        //        if (PS4.Audio())     send(3600);
        //        if (PS4.Mic())       send(3700);
    }
}
