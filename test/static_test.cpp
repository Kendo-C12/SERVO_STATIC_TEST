#include <Arduino.h>
#include <Servo.h>
#include <Arduino_FreeRTOS.h>

#define DELAY(ms) vTaskDelay(pdMS_TO_TICKS(ms))
#define TICK_TO_MS(ticks) ticks * portTICK_PERIOD_MS

Servo myServo;
uint8_t height = 0;
uint8_t times = 0;
bool deploy_yet = false;

// ฟังก์ชัน
extern void static_test(void *);
extern void printStatus(void *);
// extern void deploy();

void setup() {
  Serial.begin(9600);     

  height = 0; 

  myServo.attach(9);       
  myServo.write(180);

  xTaskCreate(static_test,"StaticTestTask",64,nullptr,2,nullptr);
  xTaskCreate(printStatus,"PrintStatusTask",64,nullptr,1,nullptr);
}

void loop() {

}

// Function
void static_test(void *) {
//   TickType_t start;
//   int currentTime; 
  
  for(;;)
  {
    // start = xTaskGetTickCount();

    height += 50; 
    times += 1; 

    if(height >= 1000 && !deploy_yet) {
        // deploy(); 
        myServo.write(0); 
        deploy_yet = true;
    }
    
    // currentTime = TICK_TO_MS(xTaskGetTickCount()-start);
    // currentTime = 1000 - currentTime; 
    // if (currentTime > 0) {
    //     DELAY(currentTime);  
    // }
    // else{
    //     times += currentTime-1000 / 1000;
    // }

    DELAY(10); 
  }
}

void printStatus(void *) {
//   TickType_t start;
//   int currentTime; 
//   int i = 0;
  for(;;)
  {
    // start = xTaskGetTickCount();

    Serial.print(height);
    Serial.print(',');
    Serial.print(times);
    Serial.print(',');
    Serial.print(deploy_yet);
    Serial.println();
    
    // currentTime = TICK_TO_MS(xTaskGetTickCount()-start);
    // currentTime = 1000 - currentTime; 
    // if (currentTime > 0) {
    //     DELAY(currentTime);  
    // }
    // else{
    //     times += currentTime-1000 / 1000;
    // }

    // i += 1;
    DELAY(10); 
  }
}

// void deploy() {
//   myServo.write(0); 
//   DELAY(553); 
// }