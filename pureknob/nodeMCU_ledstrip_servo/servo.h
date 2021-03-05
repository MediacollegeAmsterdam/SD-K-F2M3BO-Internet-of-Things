/*
 * Servo 
 */

#include <Servo.h>
Servo myservo;




void setupServo(){
  myservo.attach(servoPin);
  myservo.write(90);  // set servo to mid-point
  }
