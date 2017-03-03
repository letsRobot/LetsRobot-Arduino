/*
 * Tutorial 7b: Stepper Motor Control
 * 
 * Stepper motor follows the turns of your potentiometer.
 *
 * The circuit:
 * - Stepper driver powered with 5v, GND
 * - D8-D11 connected to IN1-IN4 on the stepper driver
 * - Potentiometer connected to 5v, gnd as a voltage divider
 * - Potentiometer wiper connected to Analog pin 0
 *
 * modified 14 August 2013
 * by Blaise Jarrett
 *
 * This example code is in the public domain.
 *
 * Derivative work from:
 * http://arduino.cc/en/Tutorial/MotorKnob
 *
 */

#include <Stepper.h>

// the pins we've connected our stepper driver to
int stepIN1Pin = 8;
int stepIN2Pin = 9;
int stepIN3Pin = 10;
int stepIN4Pin = 11;
// analog pin used to connect the potentiometer
int potPin = A0; 

// steps required for one full revolution
int stepsPerRevolution = 2048;

// make our stepper object
Stepper myStepper(stepsPerRevolution,
                  stepIN1Pin, stepIN3Pin,
                  stepIN2Pin, stepIN4Pin);

// the previous reading from the analog input
int previous = 0;

void setup()
{
    // set the speed of the motor to 10 RPMs
    myStepper.setSpeed(10);
}

void loop()
{
    int analogValue;
    int position;

    // reads the value of the potentiometer (value between 0 and 1023)
    analogValue = analogRead(potPin);

    // scale it to use it with the stepper
    position = map(analogValue, 0, 1023, 0, stepsPerRevolution * 0.6);

    // Unlike the servo motor, the stepper isn't controled
    // with an absolute position, we need a relative one.

    // move a number of steps equal to the change in the
    // sensor reading
    myStepper.step(position - previous);

    // remember the previous value of the sensor
    previous = position;
}
