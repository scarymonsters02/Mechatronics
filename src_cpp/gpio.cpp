#include <wiringPi.h>

#define INPUT_PIN = 12
#define OUTPUT_PIN = 13

int main() {
    wiringPisetup();
    pinMode(INPUT, INPUT_PIN);
    pinMode(OUTPUT, OUTPUT_PIN);

    digitalWrite(OUTPUT_PIN, 1);
    delay(1000);
    digitalWrite(OUTPUT_PIN, 1);

    int value = digitalRead(INPUT_PIN);
}