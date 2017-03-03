/*
 * Tutorial 7a: Introduction to Stepper Motors
 * 
 * Simply rotates your stepper from 360 degrees forward
 * and back.
 *
 * The circuit:
 * - Stepper driver powered with 5v, GND
 * - D8-D11 connected to IN1-IN4 on the stepper driver
 *
 * by Blaise Jarrett
 *
 * This example code is in the public domain.
 *
 *
 */

#include <Stepper.h>

int stepIN1Pin = 8;
int stepIN2Pin = 9;
int stepIN3Pin = 10;
int stepIN4Pin = 11;

int stepsPerRevolution = 2048;

Stepper myStepper(stepsPerRevolution,
                  stepIN1Pin, stepIN3Pin,
                  stepIN2Pin, stepIN4Pin);

void setup()
{
    // set the RPM
    myStepper.setSpeed(6);
}

void loop()
{
    // step one revolution in one direction
    myStepper.step(stepsPerRevolution);
    // wait a second
    delay(1000);

    // step one revolution in the other direction
    myStepper.step(-stepsPerRevolution);
    // wait a second
    delay(1000);
}
