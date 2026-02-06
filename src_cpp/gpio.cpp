#include <wiringPi.h>
#include <wiringSerial.h>

#define INPUT_PIN 12
#define OUTPUT_PIN 13

int main() {
    wiringPisetup();
    pinMode(INPUT_PIN, INPUT);
    pinMode(OUTPUT_PIN, OUTPUT);

    digitalWrite(OUTPUT_PIN, 1);
    delay(1000);
    digitalWrite(OUTPUT_PIN, 1);

    int value = digitalRead(INPUT_PIN);
}