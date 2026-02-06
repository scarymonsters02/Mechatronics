#include <wiringPi.h>
#include <wiringSerial.h>

#define INPUT_PIN 12
#define OUTPUT_PIN 13

int main() {
    wiringPiSetup();
    pinMode(INPUT_PIN, INPUT);
    pinMode(OUTPUT_PIN, OUTPUT);

    digitalWrite(OUTPUT_PIN, HIGH);
    delay(1000);
    digitalWrite(OUTPUT_PIN, LOW);

    int value = digitalRead(INPUT_PIN);
}