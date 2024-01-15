// Include the libraries: mklink /J arduino_libraries "C:\Program Files (x86)\Arduino\libraries"
#include <Arduino.h>
#define IR_SEND_PIN        9
#define RAW_BUFFER_LENGTH  750
#define DECODE_NEC
//#define DISABLE_CODE_FOR_RECEIVER // Disables restarting receiver after each send. Saves 450 bytes program memory and 269 bytes RAM if receiving functions are not used.
//#define SEND_PWM_BY_TIMER         // Disable carrier PWM generation in software and use (restricted) hardware PWM.
//#define USE_NO_SEND_PWM           // Use no carrier PWM, just simulate an active low receiver signal. Overrides SEND_PWM_BY_TIMER definition

#define TONE_PIN            4
#define APPLICATION_PIN     5
#define ALTERNATIVE_IR_FEEDBACK_LED_PIN 6 // E.g. used for examples which use LED_BUILDIN for example output.
#define _IR_TIMING_TEST_PIN 7


#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define IR_Pin      A0

#define Rem_OK  0xBF407F
#define Rem_U   0xB946FF
#define Rem_D   0xEA15FF
#define Rem_L   0xBB44FF
#define Rem_R   0xBC43FF

#define Rem_1   0xE916FF
#define Rem_2   0xE619FE
#define Rem_3   0xF20DFE
#define Rem_4   0xF30CFF
#define Rem_5   0xE718FF
#define Rem_6   0xA15EFD
#define Rem_7   0xF708FF
#define Rem_8   0xE31CFF
#define Rem_9   0xA55AFF
#define Rem_0   0xAD52FF
#define Rem_x   0xBD42FF
#define Rem_y   0xB54ADF

#include <IRremote.hpp>
IRrecv IRrecv(IR_Pin); // Set the remote
decode_results results;
static uint32_t ir_rec, previousIR = 0x000000;




//E916EF00-384372992

uint32_t sCommand = 0x384372992;
uint8_t sRepeats = 0;

/************ Setup (booting the arduino) **************/
void setup() {
    Serial.begin(115200);
    Serial.print(F("Receive IR signals at pin "));
    Serial.println(IR_Pin);
    Serial.print(F("Send IR signals at pin "));
    Serial.println(IR_SEND_PIN);
    IrSender.begin(); // Start with IR_SEND_PIN as send pin and disable feedback LED at default feedback LED pin
    IRrecv.enableIRIn(); // Initialize the IR receiver
    IrReceiver.begin(IR_Pin, ENABLE_LED_FEEDBACK);

}
/************ Main loop (running the arduino) **************/
void loop() {
    if (IrReceiver.decode()) {  // Grab an IR code   At 115200 baud, printing takes 200 ms for NEC protocol and 70 ms for NEC repeat
        if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_WAS_OVERFLOW) {         // Check if the buffer overflowed
            Serial.print(
                    F("Try to increase the \"RAW_BUFFER_LENGTH\" value of " STR(RAW_BUFFER_LENGTH) " in " __FILE__));
            delay(100);
        } else {
            if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
                Serial.print(F("?"));
                ir_rec = IrReceiver.decodedIRData.decodedRawData;
                delay(100);
            } else {
                ir_rec = IrReceiver.decodedIRData.decodedRawData;
            }


        }
        Serial.println(ir_rec, HEX);
        Serial.println(ir_rec);
        if (ir_rec == 0xFFFFFF || ir_rec == 0x000000) {
            ir_rec = previousIR;
        }
        IrReceiver.resume();                            // Prepare for the next value


        switch (ir_rec) {
            case Rem_L:
                Serial.println("Rem_L");
                break;
            case Rem_R:
                Serial.println("Rem_R");
                break;
            case Rem_OK:
                Serial.println("Rem_OK");
                break;
            case Rem_U:
                Serial.println("Rem_U");
                break;
            case Rem_D:
                Serial.println("Rem_D");
                break;
            case Rem_0:
                Serial.println("Rem_0");
                break;
            case Rem_1:
                Serial.println("Rem_1");
                break;
            case Rem_2:
                Serial.println("Rem_2");
                break;
            case Rem_3:
                Serial.println("Rem_3");
                break;
            case Rem_4:
                Serial.println("Rem_4");
                break;
            case Rem_5:
                Serial.println("Rem_5");
                break;
            case Rem_6:
                Serial.println("Rem_6");
                break;
            case Rem_7:
                Serial.println("Rem_7");
                break;
            case Rem_8:
                Serial.println("Rem_8");
                break;
            case Rem_9:
                Serial.println("Rem_9");
                break;
            case Rem_x:
                Serial.println("Rem_x");
                break;
            case Rem_y:
                Serial.println("Rem_y");
                break;

                delay(10);
                previousIR = ir_rec;
        }

    } else {

        Serial.println();
        Serial.print(F("Send now: address=0x00, command=0x"));
        Serial.print(sCommand, HEX);

        Serial.flush();

// Receiver output for the first loop must be: Protocol=NEC Address=0x102 Command=0x34 Raw-Data=0xCB340102 (32 bits)
        IrSender.sendNEC(0x00, sCommand, sRepeats);

/*
* Increment send values
*/
        sCommand += 0x11;
        sRepeats++;
// clip repeats at 4
        if (sRepeats > 4) {
            sRepeats = 4;
        }
    }
}

