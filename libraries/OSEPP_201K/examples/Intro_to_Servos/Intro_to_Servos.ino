/*
 * Tutorial 6a: Introduction to Servo Motors
 * 
 * Simply rotates your server from 0 to 180 degrees and back.
 *
 * The circuit:
 * - Brown pin to ground
 * - Red pin to 5v
 * - Orange pin to digital pin 9
 *
 * by BARRAGAN <http://barraganstudio.com>
 * modified 14 August 2013
 * by Blaise Jarrett
 *
 * This example code is in the public domain.
 *
 * Derivative work from:
 * http://arduino.cc/en/Tutorial/Sweep
 *
 */

#include <Servo.h> 

// the Orange pin is connected to digital pin 9
int servoPin = 9;

// create servo object to control our servo
// a maximum of eight servo objects can be created 
Servo myServo;
 
void setup() 
{
    // attaches the servo on pin 9 to the servo object 
    myServo.attach(servoPin);
}

void loop()
{
    // move the servo to degree 0
    myServo.write(0);

    // wait for it to move
    delay(1000);

    // move the servo to degree 180
    myServo.write(180);

    // wait for it to move
    delay(1000);
}
