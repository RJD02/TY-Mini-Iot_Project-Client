#include <Servo.h>

Servo servo;

void setup() {
  servo.attach(2);
  // put your setup code here, to run once:
  servo.write(0);
  delay(3000);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 180; i++) {
    servo.write(i);
    Serial.println("Rotating");
    delay(10);
  }
  for(int i = 179; i > -1; i--) {
    servo.write(i);
    delay(10);
  }
}
