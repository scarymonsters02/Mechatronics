#include <iostream>
#include <string>
#include <array>
//  #include <wiringPi.h>
//  #include <wiringSerial.h>

#define DATA_IN_COUNT 5
#define DATA_OUT_COUNT 5

using namespace std;

int main() {
    int fd;
    /*
    if ((fd = serialOpen("/dev/ttyS2", 115200) < 0)) {
        cout << "error: cannot open Serial port." << endl;
        return 1;
    }

    if (wiringPiSetup() == -1) {
        cout << "error: cannot reset wiringPi." << endl;
        return 1;
    }
    */
    array<int, 5> motor = {1, 2, 3, 4, 5};
    cout << "setup success." << endl;

    array<int, DATA_IN_COUNT> data2OPI;
    array<int, DATA_OUT_COUNT> data4ESP;
    
    array<char, 20> buffer;
    snprintf(buffer, sizeof(buffer), "<%d,%d,%d,%d,%d>\n", motor[0], motor[1], motor[2], motor[3], motor[4]);
}