#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int LED = 8;         // LED 핀 번호
int speakerpin = 9;  // 피에조 부저 핀 번호
const int threshold = 200;  // 임계값
unsigned long startTime = 0;  // 90% 초과 상태 시작 시간
bool AboveThreshold = false;  // 초과 상태 여부

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C LCD 주소(0x27), LCD 크기(16x2)

int lastPercentage = -1;  // 이전에 출력한 퍼센트 값 저장
String lastStatus = "";   // 이전에 출력한 상태 메시지 저장

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(speakerpin, OUTPUT);
  Serial.begin(9600);

  lcd.init();            // LCD 초기화
  lcd.backlight();       // LCD 백라이트 켜기
  lcd.setCursor(0, 0);   // LCD 첫 번째 줄로 커서 이동
  lcd.print("Initializing...");
  delay(2000);
}

void loop() {
  int sensorval = analogRead(A0); // 압력 센서 값 읽기
  sensorval = constrain(sensorval, 0, 200); // 값 제한
  int percentage = map(sensorval, 0, 200, 0, 100); // 센서 값을 0-100%로 변환

  // 시리얼 모니터 출력
  Serial.print("Sensor Value: ");
  Serial.print(sensorval);
  Serial.print(", Percentage: ");
  Serial.println(percentage);

  // LCD에 퍼센트 값 업데이트 (값이 변경된 경우에만)
  if (percentage != lastPercentage) {
    lcd.setCursor(0, 0);       // LCD 첫 번째 줄로 커서 이동
    lcd.print("Pressure: ");   // "Pressure" 표시
    lcd.print(percentage);     // 퍼센트 값 출력
    lcd.print("%   ");         // 이전 값 지우기 위해 공백 추가
    lastPercentage = percentage; // 마지막 출력 값 업데이트
  }

  // 센서 값이 임계값의 90%를 넘는지 확인
  String currentStatus;
  if (sensorval > threshold * 0.9) {
    currentStatus = "ACCEL WARNING"; // 즉시 경고 메시지 설정

    if (!AboveThreshold) {
      AboveThreshold = true;       // 초과 상태 기록
      startTime = millis();        // 초과 상태 시작 시간 기록
    } else if (millis() - startTime >= 2000) { // 2초 이상 유지되었는지 확인
      digitalWrite(LED, HIGH);     // LED 켜기
      tone(speakerpin, 1000);      // 피에조 부저 울리기
      delay(500);                  // 0.5초 대기
      digitalWrite(LED, LOW);      // LED 끄기
      noTone(speakerpin);          // 피에조 부저 끄기
    }
  } else {
    AboveThreshold = false;       // 초과 상태 해제
    digitalWrite(LED, LOW);       // LED 끄기
    noTone(speakerpin);           // 피에조 부저 끄기
    currentStatus = "Normal State"; // 정상 상태 메시지
  }

  // 상태 메시지 업데이트 (값이 변경된 경우에만)
  if (currentStatus != lastStatus) {
    lcd.setCursor(0, 1);          // LCD 두 번째 줄로 커서 이동
    lcd.print(currentStatus);     // 상태 메시지 출력
    lcd.print("                "); // 이전 값 지우기 위해 공백 추가
    lastStatus = currentStatus;   // 마지막 출력 값 업데이트
  }

  delay(100);
}
