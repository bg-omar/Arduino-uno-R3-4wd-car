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

#include <IRremote.h>

IRrecv IRrecv(IR_Pin); // Set the remote
decode_results results;
static uint32_t ir_rec, previousIR = 0x000000;




//E916EF00-384372992

uint32_t sCommand = 0xF906EF00;
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



/*    Serial.println();
    Serial.print(F("Send now: address=0x00, command=0x"));
    Serial.print(sCommand, HEX);

    Serial.flush();

// Receiver output for the first loop must be: Protocol=NEC Address=0x102 Command=0x34 Raw-Data=0xCB340102 (32 bits)
    IrSender.sendNEC(0x00, sCommand, sRepeats);

    sRepeats++;
    if (sRepeats > 4) {
        sRepeats = 4;
    }*/

    //IRrecv.enableIRIn();

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
        switch (results.decode_type){
            case NEC: Serial.println("NEC"); break ;
            case SONY: Serial.println("SONY"); break ;
            case RC5: Serial.println("RC5"); break ;
            case RC6: Serial.println("RC6"); break ;
           // case DISH: Serial.println("DISH"); break ;
            case SHARP: Serial.println("SHARP"); break ;
            case JVC: Serial.println("JVC"); break ;
           // case SANYO: Serial.println("SANYO"); break ;
           // case MITSUBISHI: Serial.println("MITSUBISHI"); break ;
            case SAMSUNG: Serial.println("SAMSUNG"); break ;
            case LG: Serial.println("LG"); break ;
            case WHYNTER: Serial.println("WHYNTER"); break ;
           // case AIWA_RC_T501: Serial.println("AIWA_RC_T501"); break ;
            case PANASONIC: Serial.println("PANASONIC"); break ;
            case DENON: Serial.println("DENON"); break ;
            default:
            case UNKNOWN: Serial.println("UNKNOWN"); break ;
        }

        IrReceiver.resume();                            // Prepare for the next value


        /*switch (ir_rec) {
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
        }*/

    }
}

#define RAW_DATA_LEN 68
uint16_t IR_ON[RAW_DATA_LEN]={
    9090, 4394, 690, 434, 698, 430, 670, 458,
    694, 434, 698, 430, 694, 434, 670, 458,
    670, 454, 698, 1534, 670, 1558, 694, 1534,
    694, 1534, 670, 454, 698, 1534, 694, 1534,
    694, 1534, 670, 454, 694, 434, 698, 430,
    694, 434, 694, 430, 698, 430, 698, 430,
    694, 434, 694, 1538, 666, 1562, 666, 1558,
    694, 1534, 694, 1538, 694, 1534, 694, 1534,
    690, 1538, 690, 1000};

#define RAW_DATA_LEN 68
uint16_t IR_OFF[RAW_DATA_LEN]={
    9118, 4366, 698, 426, 674, 458, 698, 426,
    698, 430, 698, 430, 698, 434, 698, 430,
    694, 430, 674, 1562, 694, 1534, 694, 1534,
    670, 1562, 670, 454, 694, 1538, 694, 1534,
    690, 1538, 694, 1534, 670, 454, 674, 454,
    670, 458, 670, 458, 670, 458, 694, 434,
    694, 434, 670, 458, 670, 1558, 670, 1558,
    670, 1562, 666, 1562, 666, 1562, 666, 1562,
    694, 1534, 674, 1000};


#define RAW_DATA_LEN 68
uint16_t IR_BRIGHT[RAW_DATA_LEN]={
    9090, 4390, 666, 454, 674, 454, 674, 454,
    694, 430, 698, 430, 698, 430, 698, 430,
    670, 458, 694, 1534, 670, 1558, 694, 1534,
    670, 1558, 694, 430, 694, 1538, 666, 1558,
    694, 1538, 690, 430, 698, 1534, 666, 458,
    694, 434, 670, 458, 670, 454, 694, 434,
    694, 434, 670, 1558, 694, 434, 694, 1534,
    694, 1534, 694, 1538, 690, 1538, 690, 1538,
    690, 1538, 690, 1000};


uint16_t IR_DIM[RAW_DATA_LEN]={
    9102, 4390, 670, 458, 670, 458, 670, 458,
    670, 458, 670, 458, 670, 458, 670, 454,
    674, 458, 670, 1558, 670, 1562, 666, 1562,
    670, 1558, 670, 454, 674, 1558, 670, 1558,
    670, 1558, 674, 1554, 674, 1554, 674, 454,
    674, 454, 674, 454, 674, 454, 670, 458,
    670, 458, 670, 458, 670, 458, 674, 1558,
    670, 1558, 670, 1558, 670, 1558, 670, 1558,
    674, 1558, 670, 1000};

#define RAW_DATA_LEN 68
uint16_t IR_PINK[RAW_DATA_LEN]={
    9142, 4358, 702, 422, 706, 422, 710, 418,
    706, 422, 710, 418, 710, 422, 706, 418,
    710, 422, 706, 1522, 710, 1522, 678, 1550,
    706, 1522, 706, 422, 706, 1526, 702, 1526,
    706, 1522, 678, 446, 706, 422, 706, 1530,
    698, 1530, 698, 426, 678, 450, 702, 426,
    702, 426, 702, 1530, 702, 1526, 702, 426,
    698, 430, 698, 1534, 694, 1534, 698, 1530,
    702, 1530, 698, 1000};


#define RAW_DATA_LEN 68
uint16_t VAPE_1[RAW_DATA_LEN]={
    9302, 4374, 686, 430, 714, 426, 658, 490,
    690, 430, 714, 430, 658, 486, 694, 430,
    718, 1554, 690, 1558, 650, 1598, 710, 1558,
    690, 1554, 650, 1602, 706, 1562, 686, 1558,
    646, 490, 690, 430, 710, 426, 650, 1602,
    706, 426, 650, 490, 690, 430, 710, 426,
    646, 494, 686, 1558, 646, 1602, 710, 426,
    646, 1602, 706, 1558, 686, 1558, 646, 1602,
    710, 1558, 682, 1000};


#define RAW_DATA_LEN 68
uint16_t VAPE_0[RAW_DATA_LEN]={
    9170, 4402, 650, 462, 674, 458, 618, 518,
    658, 458, 678, 458, 618, 518, 654, 462,
    678, 1586, 654, 1590, 614, 1630, 678, 1590,
    650, 1590, 614, 1634, 674, 1590, 654, 1586,
    618, 522, 654, 458, 682, 454, 618, 522,
    654, 1590, 614, 522, 658, 458, 682, 454,
    618, 522, 654, 1590, 614, 1630, 678, 1590,
    654, 458, 682, 1586, 654, 1590, 614, 1634,
    674, 1590, 654, 1000};


#define RAW_DATA_LEN 68
uint16_t VAPE_POWER[RAW_DATA_LEN]={
    9182, 4402, 678, 434, 702, 430, 650, 490,
    682, 434, 706, 426, 650, 490, 682, 434,
    682, 1586, 682, 1558, 646, 1602, 682, 1586,
    686, 1558, 646, 1602, 702, 1562, 678, 1562,
    642, 494, 682, 434, 678, 1586, 682, 1554,
    646, 1602, 706, 1562, 682, 430, 682, 454,
    646, 494, 678, 1562, 642, 494, 658, 458,
    682, 454, 642, 494, 654, 1590, 638, 1606,
    698, 1566, 674, 1000};