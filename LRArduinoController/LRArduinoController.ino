#include <Servo.h> 
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
//Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(1x40);
//Adafruit_PWMServoDriver pwm3 = Adafruit_PWMServoDriver(2x40);
//Adafruit_PWMServoDriver pwm4 = Adafruit_PWMServoDriver(3x40);
 
//Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
//int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
  Serial.begin(9600);
  Serial.println("16 channel PWM test!");
 
  pwm1.begin();
  pwm1.setPWMFreq(1600);  // This is the maximum PWM frequency
  
//  pwm2.begin();
//  pwm2.setPWMFreq(1600);  // This is the maximum PWM frequency
//  
//  pwm3.begin();
//  pwm3.setPWMFreq(1600);  // This is the maximum PWM frequency
//  
//  pwm4.begin();
//  pwm4.setPWMFreq(1600);  // This is the maximum PWM frequency
} 
 
 
void loop() 
{

}
