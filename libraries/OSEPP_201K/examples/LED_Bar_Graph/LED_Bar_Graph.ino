/*
 * Tutorial 3c: LED Knight Rider
 * 
 * Turns on a series of LEDs based on the value of an analog sensor.
 * This is a simple way to make a bar graph display. Though this graph
 * uses 6 LEDs, you can use any number by changing the LED count
 * and the pins in the array.
 *
 * The circuit:
 *  - 6 LEDs attached to digital pins 2 through 7 with 330ohm resistors
 *
 * created 4 Sep 2010
 * by Tom Igoe
 * modified 14 August 2013
 * by Blaise Jarrett
 *
 * This example code is in the public domain.
 *
 * Derivative work from:
 * http://www.arduino.cc/en/Tutorial/BarGraph
 *
 */

// the pin that the potentiometer is attached to
int potPin = A0;
// an array of pin numbers to which LEDs are attached
// to add more LEDs just list them here in this array
int ledPins[] = {2, 3, 4, 5, 6, 7};
// the number of LEDs in the bar graph
int ledCount = sizeof(ledPins) / sizeof(ledPins[0]);

void setup()
{
    // use a for loop to initialize each pin as an output
    for (int thisLed = 0; thisLed < ledCount; thisLed++)
    {
        pinMode(ledPins[thisLed], OUTPUT);
    }
}

void loop()
{
    // read the potentiometer
    int potReading = analogRead(potPin);
    // map the result to a range from 0 to the number of LEDs
    int ledLevel = map(potReading, 0, 1023, 0, ledCount);

    // loop over the LED array:
    for (int thisLed = 0; thisLed < ledCount; thisLed++)
    {
        // if the array element's index is less than ledLevel
        // turn the pin for this element on
        if (thisLed < ledLevel)
        {
            digitalWrite(ledPins[thisLed], HIGH);
        }
        // turn off all pins higher than the ledLevel
        else
        {
            digitalWrite(ledPins[thisLed], LOW);
        }
    }
}
