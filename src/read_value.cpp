#include <Arduino.h>
#include <Servo.h>

Servo myServo;

void setup() {
  Serial.begin(9600);
  myServo.attach(9);
  myServo.write(180);
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    if (cmd == "D") {
      myServo.write(0); // หมุนไป 0 องศา
      delay(1000);
      Serial.println("Deployed to 0 degrees.");
    }
  }
}