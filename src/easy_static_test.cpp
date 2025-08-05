#include <Arduino.h>
#include <Servo.h>

Servo myServo;
uint8_t t = 1;
uint8_t times = 0;
float height = 0;
float velocity = 0;
float acceleration = 5;
float prediction_height = 0;

float diff_acceleration;
float diff_velocity;

bool deploy_yet = false;

extern float predictionHeight();

void setup() {

  Serial.begin(9600);  // เริ่ม serial communication ที่ความเร็ว 9600 bps
  while(!Serial);

  height = 0; 

  myServo.attach(9);       
  myServo.write(180);
  delay(4000); // หน่วงเวลาให้ servo หมุนเสร็จ
}

void loop() {

  times += t;
  acceleration -= 1;
  velocity += acceleration*t;
  height += velocity*t;
  prediction_height = height + (velocity * 0.5532); 

  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    if (cmd == "ROTATE") {
      myServo.write(0); 
      deploy_yet = true;
    }
  }

  if(((velocity < 0 && times > 4) || times > 10) && !deploy_yet) {
    myServo.write(0); 
    deploy_yet = true;
  }

  Serial.print(times);
  Serial.print(',');
  Serial.print(height);
  Serial.print(',');
  Serial.print(velocity);
  Serial.print(',');
  Serial.print(acceleration);
  Serial.print(',');
  Serial.print(prediction_height);
  Serial.print(',');
  Serial.print(deploy_yet);
  Serial.println();
    
  delay(1000); 
}

//functions

// float predictHeight(const float* heights, int length, float n_sec) {
//     const float dt = 0.01f;  // time interval between samples

//     if (length < 2) return (length == 1) ? heights[0] : 0.0f;

//     // Estimate velocity using the last two samples
//     float v = (heights[index - 1] - heights[index - 2]) / dt;

//     // Predict height at time n_sec
//     float predicted = heights[index - 1] + v * n_sec;

//     return predicted;
// }
