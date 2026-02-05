#include <Arduino.h>

// 받을 데이터 개수
#define DATA_COUNT 5

// 데이터를 저장할 배열
int receivedData[DATA_COUNT];

void setup() {
  Serial.begin(115200);                      // 디버깅용
  Serial2.begin(115200, SERIAL_8N1, 16, 17); // OPi 통신용
}

void loop() {
  // 시리얼 버퍼에 데이터가 있는지 확인
  if (Serial2.available() > 0) {
    
    // 1. 시작 문자('<')를 찾을 때까지 읽어서 버림 (데이터 싱크 맞추기)
    char c = Serial2.read();
    
    if (c == '<') {
      // '<'를 만났으므로 이제부터 숫자 5개가 온다고 기대함
      
      for (int i = 0; i < DATA_COUNT; i++) {
        // parseInt: 숫자가 나올 때까지 기다렸다가, 콤마(,)나 문자가 나오면 멈춤
        // 즉, "512, 1024" 에서 512 뽑고, 콤마 건너뛰고 대기함
        receivedData[i] = Serial2.parseInt(); 
      }

      // 마지막 종료 문자('>')까지 읽어서 버퍼를 비워줌 (안전장치)
      Serial2.readStringUntil('>');

      // --- 2. 데이터 처리 (모터 제어 등) ---
      executeMotorControl();
    }
  }
  
  // (중요) 제어 루프는 여기서 계속 돌아야 함
  // 단, parseInt는 데이터가 올 때까지 잠깐 멈출(Blocking) 수 있으므로
  // OPi에서 데이터를 너무 띄엄띄엄 보내면 모터가 끊길 수 있음.
  // 50ms ~ 100ms 주기로 꾸준히 보내주는 게 좋음.
}

void executeMotorControl() {
  // 디버깅 출력
  Serial.print("Recv: ");
  for(int i=0; i<DATA_COUNT; i++){
    Serial.print(receivedData[i]);
    Serial.print(" ");
  }
  Serial.println();

  // 실제 모터 제어 적용 예시
  // motor1.setTarget(receivedData[0]);
  // motor2.setTarget(receivedData[1]);
  // digitalWrite(LED_PIN, receivedData[4]);
}