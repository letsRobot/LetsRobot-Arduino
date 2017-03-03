/*
 * Tutorial 4b: Pitch Follower
 * 
 * Plays a varying pitch through the speaker. The pitch follows the light
 * present on the photocell. Cover the light and the pitch will go lower,
 * shine more light and it will go higher.
 *
 * The circuit:
 * - 8-ohm speaker conected to digital pin 8 through a 100 ohm resistor
 * - photoresistor from analog in 0 to +5V
 * - 10K resistor from analog in 0 to ground
 *
 * created 21 Jan 2010
 * modified 31 May 2012
 * by Tom Igoe, with suggestion from Michael Flynn
 * modified 14 August 2013
 * by Blaise Jarrett
 *
 * This example code is in the public domain.
 *
 * Derivative work from:
 * http://arduino.cc/en/Tutorial/Tone2
 *
 */

// the pin the speaker is attached to
int speakerPin = 8;
// the photocell voltage divider pin
int photocellPin = A0;

// sensor minimum, discovered through experiment
const int sensorMin = 0;
// sensor maximum, discovered through experiment
const int sensorMax = 600;
// the lowest pitch possible
const int lowestPitch = 150;
// the highest pitch possible
const int highestPitch = 1000;

void setup()
{
    // nothing to do here
}

void loop()
{
    int analogValue;
    int pitch;

    // read our photocell
    analogValue = analogRead(photocellPin);

    // map the analog input range
    // to the output pitch range
    // play with the constants up above
    pitch = map(analogValue, sensorMin,
                sensorMax, lowestPitch, highestPitch);

    // play the pitch
    tone(speakerPin, pitch);
}
