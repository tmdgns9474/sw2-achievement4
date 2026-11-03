#include <Servo.h>

// 핀 및 서보 기본 파라미터 설정
#define PIN_SERVO 10

#define _DUTY_MIN 550      // 0도일 때 펄스 폭 (us)
#define _DUTY_MAX 2400     // 180도일 때 펄스 폭 (us)

// =========================================================
// [실험 설정] 촬영할 실험에 맞게 속도와 목표 각도를 설정하십시오.
// =========================================================
// [실험 1 설정] 60초 동안 180도 등속 이동 (3도/초)
//#define _SERVO_SPEED 3.0
//#define _TARGET_ANGLE 180.0

// [실험 2 설정] 300초(5분) 동안 90도 등속 이동 (0.3도/초)
// ※ 실험 2를 실행/촬영할 때는 위 2줄을 주석 처리하고, 아래 2줄의 주석을 해제하십시오.
#define _SERVO_SPEED 0.3
#define _TARGET_ANGLE 90.0
// =========================================================

#define _LOOP_INTERVAL 20  // 제어 루프 주기 (20ms = 50Hz)

Servo myservo;

float current_angle = 0.0;     // 현재 각도 (도)
unsigned long last_loop_time = 0;

void setup() {
  myservo.attach(PIN_SERVO, _DUTY_MIN, _DUTY_MAX);
  
  // 시작 위치 0도로 초기화
  current_angle = 0.0;
  myservo.writeMicroseconds(_DUTY_MIN);
  
  Serial.begin(57600);
  delay(1000); // 전원 및 서보 보정 안정화 대기
}

void loop() {
  unsigned long current_time = millis();

  // 지정된 주기(_LOOP_INTERVAL ms)마다 등속 제어 계산 실행
  if (current_time - last_loop_time >= _LOOP_INTERVAL) {
    float dt = (current_time - last_loop_time) / 1000.0; // 실제 경과 시간 (초 단위)
    last_loop_time = current_time;

    // 목표 각도까지 등속 이동
    if (current_angle < _TARGET_ANGLE) {
      // 이동 각도 = 속도(deg/s) * 경과 시간(s)
      current_angle += _SERVO_SPEED * dt;

      // 목표 각도 초과 방지
      if (current_angle > _TARGET_ANGLE) {
        current_angle = _TARGET_ANGLE;
      }

      // 각도(deg) -> 서보 펄스 폭(us) 비례 변환
      int duty_us = (int)(_DUTY_MIN + (current_angle / 180.0) * (_DUTY_MAX - _DUTY_MIN));
      myservo.writeMicroseconds(duty_us);
    }

    // 시리얼 모니터 / 시리얼 플로터 출력
    Serial.print("Target:");      Serial.print(_TARGET_ANGLE);
    Serial.print(",Angle:");       Serial.print(current_angle);
    Serial.print(",Time_sec:");    Serial.println(millis() / 1000.0);
  }
}
