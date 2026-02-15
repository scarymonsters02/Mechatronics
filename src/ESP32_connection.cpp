#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <unistd.h> // usleep

// dmesg | grep ttys
// 

using namespace std;

// 데이터 개수
#define DATA_CMD_COUNT 5 // 보낼 명령 개수

int setupSerial();

int main() {
    int fd = setupSerial();

    // 보낼 명령 데이터
    int cmdData[DATA_CMD_COUNT] = {90, 90, 90, 90, 0};

    // 타이머용 변수
    unsigned long lastSendTime = 0;
    const int sendInterval = 50; // 50ms (단위: ms)

    // 수신용 버퍼 문자열
    string recvBuffer = "";

    cout << "Bidirectional UART Started..." << endl;

    while (true) {
        // ==================================================
        // 1. 수신 (ESP32가 보낸 데이터 읽기) - Non-blocking
        // ==================================================
        while (serialDataAvail(fd) > 0) {
            char ch = serialGetchar(fd);

            if (ch == '<') { 
                recvBuffer = ""; // 시작 문자 오면 버퍼 초기화
            } 
            else if (ch == '>') { // 끝 문자 오면 파싱 시작
                // CSV 파싱: "1024,512,100..." 문자열을 숫자로 분리
                // 간단하게 수신된 원본만 출력해 봅니다 (실제론 sscanf나 strtok 사용)
                cout << "[OPi Received]: " << recvBuffer << endl;
                
                // CSV 파싱
                int s1, s2, s3, s4, s5;
                sscanf(recvBuffer.c_str(), "%d,%d,%d,%d,%d", &s1, &s2, &s3, &s4, &s5);
            } 
            else if (ch != '\n') { // 줄바꿈 빼고 담기
                recvBuffer += ch;
            }
        }

        // ==================================================
        // 2. 송신 (ESP32로 명령 보내기) - Timer Check
        // ==================================================
        unsigned long currentTime = millis(); // wiringPi의 millis()
        if (currentTime - lastSendTime >= sendInterval) {
            lastSendTime = currentTime;

            // 보낼 데이터 갱신 (테스트용)
            cmdData[0]++; 
            if(cmdData[0] > 180) cmdData[0] = 0;

            // 패킷 생성 및 전송
            char sendBuf[64];
            snprintf(sendBuf, sizeof(sendBuf), "<%d,%d,%d,%d,%d>\n", 
                     cmdData[0], cmdData[1], cmdData[2], cmdData[3], cmdData[4]);
            
            serialPuts(fd, sendBuf);
            // cout << "Sent: " << sendBuf; // 너무 시끄러우면 주석 처리
        }

        // CPU 점유율 방어용 미세 딜레이 (1ms)
        // 통신 속도엔 지장 없고 CPU 과부하만 막아줌
        delay(1); 
    }

    return 0;
}

int setupSerial() {
    int fd;
    if ((fd = serialOpen("/dev/ttyS6", 115200)) < 0) return 1;
    if (wiringPiSetup() == -1) return 1;
    return fd;
}