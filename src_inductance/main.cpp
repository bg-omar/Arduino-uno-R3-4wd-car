#include <Arduino.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Initialize LCD


//double pulse, frequency, capacitance, inductance;

const int inputPin = 8;  // Input from the comparator output
const int triggerPin = 9; // Output to the LC circuit

double pulse, frequency, inductance_uH, inductance_mH, inductance;
const double capacitance = 1.E-6;  // Capacitance value (adjust as needed)


void setup() {

    Serial.begin(115200);
    pinMode(inputPin, INPUT_PULLUP);
    pinMode(triggerPin, OUTPUT);

    lcd.init();  // Initialize LCD
    lcd.backlight();  // Turn on backlight

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("booting");
    lcd.setCursor(0, 1);
    delay(500);



}

void loop() {

    digitalWrite(triggerPin, HIGH);
    delay(5);
    digitalWrite(triggerPin,LOW);
    delayMicroseconds(5);
    pulse = pulseIn(inputPin,HIGH,5000);

    if(pulse > 0.1){
        // capacitance = 1.E-6;
        frequency = 1.E6/(2*pulse);
        inductance = 1./(capacitance*frequency*frequency*4.*3.14159*3.14159);
        inductance *= 1E6;


        Serial.print("Inductance! ");
        Serial.print(inductance);
        Serial.println(" uH");
        delay(10);
    }

    digitalWrite(triggerPin, HIGH); // Trigger the LC circuit
    delay(5); // Allow time for the inductor to charge
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(5); // Ensure resonation is measured
    pulse = pulseIn(inputPin, HIGH, 5000); // Measure pulse width (returns 0 if timeout)



    if (pulse > 0.1) { // If a timeout did not occur and a reading was taken
        frequency = 1.E6 / (2 * pulse); // Calculate frequency in Hz
        inductance = 1. / (capacitance * frequency * frequency * 4. * 3.14159 * 3.14159); // Calculate inductance in microhenries
        inductance_uH = 1E6 * inductance;
        inductance_mH = inductance_uH / 1000; // Convert inductance to millihenries

        // Serial print
        Serial.print("High for uS:");
        Serial.print(pulse);
        Serial.print("\tFrequency Hz:");
        Serial.print(frequency);
        Serial.print("\tInductance uH:");
        Serial.print(inductance_uH);
        Serial.print("\tInductance mH:");
        Serial.println(inductance_mH);


        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("uH Inductance mH");
        lcd.setCursor(0, 1);
        lcd.print(inductance_uH);
        lcd.setCursor(10, 1);
        lcd.print(inductance_mH);
    }

    delay(1000); // Allow time for the inductor to charge


}