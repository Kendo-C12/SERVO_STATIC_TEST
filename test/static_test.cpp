#include <Arduino.h>
#include <Servo.h>
#include <Arduino_FreeRTOS.h>

#define DELAY(ms) vTaskDelay(pdMS_TO_TICKS(ms))
#define TICK_TO_MS(ticks) ticks * portTICK_PERIOD_MS

Servo myServo;
int height = 0;
int times = 0;

bool deploy_yet = false;

// ฟังก์ชัน
extern void static_test();
extern void deploy();

void setup() {
  Serial.begin(115200);     

  height = 900; 

  myServo.attach(9);       
  myServo.write(180);
}

void loop() {
  static_test();
}

// Function
void static_test() {
  TickType_t start;
  int currentTime; 
  
  for(;;)
  {
    start = xTaskGetTickCount();

    height += 100; 
    times += 1; 

    if(height >= 1000) {
        deploy(); 
    }
    
    currentTime = TICK_TO_MS(xTaskGetTickCount()-start);
    currentTime = 1000 - currentTime; 
    if (currentTime > 0) {
        delay(currentTime);  
    }
    else{
        times += currentTime-1000 / 1000;
    }
  }
}

void printStatus() {
  TickType_t start;
  int currentTime; 

  for(;;)
  {
    Serial.print(height);
    Serial.print(',');
    Serial.print(times);
    Serial.print(',');
    Serial.print(deploy_yet);
    Serial.println();
    
    currentTime = TICK_TO_MS(xTaskGetTickCount()-start);
    currentTime = 1000 - currentTime; 
    if (currentTime > 0) {
        delay(currentTime);  
    }
    else{
        times += currentTime-1000 / 1000;
    }
  }
}

void deploy() {
  myServo.write(0); 
  delay(553); 
}