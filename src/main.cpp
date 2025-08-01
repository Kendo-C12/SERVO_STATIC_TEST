#include <Arduino.h>  // เรียกใช้ไลบรารี Arduino
#include <Servo.h>  // เรียกใช้ไลบรารี Servo
#include <Arduino_FreeRTOS.h>  // เรียกใช้ไลบรารี FreeRTOS สำหรับการจัดการ task

#define DELAY(ms) vTaskDelay(pdMS_TO_TICKS(ms))
#define TICK_TO_MS(ticks) ticks * portTICK_PERIOD_MS

Servo myServo;      // สร้างอ็อบเจกต์ servo
int angle = 0;      // ตัวแปรเก็บองศาที่ต้องการ
int height = 0;    // ตัวแปรเก็บความสูงปัจจุบัน (เมตร)
int times = 0;     // ตัวแปรเก็บเวลาที่ผ่านไป (วินาที)
TickType_t start;
int currentTime; // ตัวแปรเก็บเวลาเริ่มต้นและเวลาปัจจุบัน (มิลลิวินาที)

// ฟังก์ชัน
extern void count_height();
extern void deploy();
extern void Serial_read();

void setup() {
  Serial.begin(115200);      // เริ่ม serial communication ที่ความเร็ว 115200 bps

  height = 900; // กำหนดความสูงปัจจุบันเป็น 900 เมตร

  myServo.attach(9);       // กำหนดขา D9 ให้ควบคุม servo
  myServo.write(180);
  Serial.print("Set deploy angle to 180 degrees.");
  Serial.println("Enter angle (0 - 180):");  // แสดงข้อความแนะนำ
}

void loop() {
  Serial_read();
  delay(100);  // หน่วงเวลาเล็กน้อยเพื่อหลีกเลี่ยงการอ่านข้อมูลซ้ำซ้อน
}

// Function

void count_height() {
  // เพิ่มความสูง 100 เมตรทุก 1 วินาที
  start = xTaskGetTickCount();
  
  Serial.println("height: " + String(height) + " meters\tTime: " + String(times) + " seconds");
  height += 100; 
  times += 1; // เพิ่มเวลาที่ผ่านไป 1 วินาที

  if(height >= 1000) { // ถ้าความสูงถึง 1000 เมตร
    Serial.println("Apogee reached at " + String(height) + " meters.");
    deploy(); // เรียกใช้ฟังก์ชัน deploy() เพื่อเริ่มการปล่อย
  }

  // คำนวณเวลาที่เหลือในวินาที
  currentTime = TICK_TO_MS(xTaskGetTickCount()-start);
  currentTime = 1000 - currentTime; // คำนวณเวลาที่เหลือในวินาที
  if (currentTime > 0) {
    delay(currentTime);  // หน่วงเวลาคำนวณ
  }
  else{
    times += currentTime-1000 / 1000; // เพิ่มเวลาที่ผ่านไปตามเวลาที่เหลือ
  }
}

void deploy() {
  Serial.println("Deploying parachute...");
  Serial.println("Parachute deployed at " + String(height) + " meters.\tTime: " + String(times) + " seconds.");

  myServo.write(0); // หมุน servo ไปยังองศา 0 เพื่อปล่อยพาราชูต
  Serial.println("Servo moved to 0 degrees for deployment.");

  // หน่วงเวลาเพื่อให้ servo หมุนเสร็จ
  vTaskDelay(pdMS_TO_TICKS(2000)); // หน่วงเวลา 2 วินาที
  Serial.println("Deployment complete.");
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
  delay(1000);  // หน่วงเวลาเพื่อให้ servo หมุนเสร็จ
}