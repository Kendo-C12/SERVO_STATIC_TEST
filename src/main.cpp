#include <Arduino.h>  // เรียกใช้ไลบรารี Arduino
#include <Servo.h>  // เรียกใช้ไลบรารี Servo

Servo myServo;      // สร้างอ็อบเจกต์ servo
int angle = 0;      // ตัวแปรเก็บองศาที่ต้องการ

// ฟังก์ชัน
extern void Serial_read();

void setup() {

  Serial.begin(9600);      // เริ่ม serial communication ที่ความเร็ว 9600 bps
  while(!Serial.available());

  myServo.attach(9);       // กำหนดขา D9 ให้ควบคุม servo
  myServo.write(180);
  Serial.print("Set deploy angle to 180 degrees.");
  Serial.println("Enter angle (0 - 180):");  // แสดงข้อความแนะนำ
}

void loop() {
  Serial_read();
  delay(100);  // หน่วงเวลาเล็กน้อยเพื่อหลีกเลี่ยงการอ่านข้อมูลซ้ำซ้อน
}

void Serial_read() {
  if (Serial.available()) {         // ถ้ามีข้อมูลที่รับเข้ามาใน serial
    angle = Serial.parseInt();      // อ่านตัวเลขจาก serial
    if (angle > 0 && angle <= 180) {
      myServo.write(angle);         // สั่ง servo หมุนไปยังองศานั้น
      Serial.print("Moved to ");
      Serial.print(angle);
      Serial.println(" degrees.");
    } else {
      Serial.println("Invalid angle. Please enter 0 - 180.");
    }
  }
  else if(angle == -1){
    myServo.write(0);
    Serial.println("Deployed to 0 degrees.");
    angle = 0;
  }
}