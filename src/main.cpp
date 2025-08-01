#include <Arduino.h>  // เรียกใช้ไลบรารี Arduino
#include <Servo.h>  // เรียกใช้ไลบรารี Servo
#include <FreeRTOS.h>  // เรียกใช้ไลบรารี FreeRTOS สำหรับการจัดการ task

#define DELAY(ms) vTaskDelay(pdMS_TO_TICKS(ms))

Servo myServo;      // สร้างอ็อบเจกต์ servo
int angle = 0;      // ตัวแปรเก็บองศาที่ต้องการ
int height = 0;    // ตัวแปรเก็บความสูงปัจจุบัน (เมตร)
int times = 0;     // ตัวแปรเก็บเวลาที่ผ่านไป (วินาที)
unsigned long start, currentTime; // ตัวแปรเก็บเวลาเริ่มต้นและเวลาปัจจุบัน (มิลลิวินาที)

// ฟังก์ชัน
extern void deploy();
extern void test();
extern void Delay_program();

void setup() {
  Serial.begin(115200);      // เริ่ม serial communication ที่ความเร็ว 115200 bps

  height = 900; // กำหนดความสูงปัจจุบันเป็น 900 เมตร

  myServo.attach(9);       // กำหนดขา D9 ให้ควบคุม servo
  Serial.println("Enter angle (0 - 180):");  // แสดงข้อความแนะนำ
  myServo.write(180);
  Serial.print("Set deploy angle to 180 degrees.");
}

void loop() {
  test();  
  delay(100);  // หน่วงเวลาเล็กน้อยเพื่อหลีกเลี่ยงการอ่านข้อมูลซ้ำซ้อน
}

// Function

void deploy() {
  // เพิ่มความสูง 100 เมตรทุก 1 วินาที
  Serial.println("height: " + String(height) + " meters\tTime: " + String(times) + " seconds");
  height += 100; 
  times += 1; // เพิ่มเวลาที่ผ่านไป 1 วินาที
  delay(1000);  // หน่วงเวลาเพื่อให้ servo หมุนเสร็จ
}

void test() {
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
  delay(1000);  // หน่วงเวลาเพื่อให้ servo หมุนเสร็จ
}

void Delay_program() {
  // หน่วงเวลา 5 วินาที
  delay(5000);
}