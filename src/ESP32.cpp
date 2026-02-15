#include <Arduino.h>
#include <ESP32Servo.h>
// 데이터 개수 정의
#define DATA_IN_COUNT 5  // OPi에서 받을 데이터 개수
#define DATA_OUT_COUNT 5 // OPi로 보낼 데이터 개수
#define TX 5
#define RX 6

// 데이터 저장용 배열
int targetAngles[DATA_IN_COUNT] = {0, 0, 0, 0, 0}; // 수신값
int sensorValues[DATA_OUT_COUNT] = {10, 20, 30, 40, 50}; // 송신값 (테스트용)

// 타이머 변수
unsigned long lastSendTime = 0;
const int sendInterval = 50; // 50ms마다 송신 (1초에 20번)

HardwareSerial Serial2(1);
void setup() {
 
  Serial.begin(115200); // PC 디버깅용
  // [중요] OPi 연결 (RX:16, TX:17)
  // 타임아웃을 10ms로 짧게 설정해야 모터 제어 루프가 안 끊김
  Serial2.begin(115200, SERIAL_8N1, RX, TX);
  Serial2.setTimeout(10); 
}

void loop() {
  // ----------------------------------------
  // 1. 수신 (OPi가 보낸 명령이 왔는가?) -> 즉시 처리
  // ----------------------------------------
  if (Serial2.available() > 0) {
    char c = Serial2.read();
    if (c == '<') { // 패킷 시작 발견
      for (int i = 0; i < DATA_IN_COUNT; i++) {
        targetAngles[i] = Serial2.parseInt(); // 콤마 건너뛰며 숫자 추출
      }
      Serial2.readStringUntil('>'); // 패킷 끝 처리
      
      // (디버깅) 잘 받았나 확인
      Serial.printf("Recv: %d, %d, %d\n", targetAngles[0], targetAngles[1], targetAngles[2]);
    }
  }

  // ----------------------------------------
  // 2. 송신 (내 센서값 보내기) -> 50ms마다 실행 (비동기)
  // ----------------------------------------
  unsigned long currentTime = millis();
  if (currentTime - lastSendTime >= sendInterval) {
    lastSendTime = currentTime;

    // 센서값 갱신 (예시: 가변저항 읽기 등)
    
    sensorValues[1]++; // 테스트용 카운터 증가

    // 데이터 전송: <값,값,값,값,값>\n
    Serial2.printf("<%d,%d,%d,%d,%d>\n", 
                   sensorValues[0], sensorValues[1], sensorValues[2], sensorValues[3], sensorValues[4]);
  }
  
  // ----------------------------------------
  // 3. 모터 제어 (P제어 등) -> 멈춤 없이 계속 실행
  // ----------------------------------------
  // motorControl(targetAngles); 
}