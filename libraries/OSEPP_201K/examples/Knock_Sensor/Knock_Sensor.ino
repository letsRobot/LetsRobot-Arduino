/*
 * Tutorial 4c: Knock Sensor
 * 
 * Prints "Knock!" to serial when the piezo buzzer
 * detects a knock. Try placing your board on your desk
 * and giving your desk a whack.
 *
 * The circuit:
 * + connection of the piezo attached to analog in 0
 * - connection of the piezo attached to ground
 * 1 megohm resistor attached from analog in 0 to ground
 *
 * created 25 Mar 2007
 * by David Cuartielles <http://www.0j0.org>
 * modified 30 Aug 2011
 * by Tom Igoe
 * modified 14 August 2013
 * by Blaise Jarrett
 *
 * This example code is in the public domain.
 *
 * Derivative work from:
 * http://www.arduino.cc/en/Tutorial/Knock
 *
 */

// the piezo is connected to analog pin 0
int piezoPin = A0;
// the pin our LED is connected to
int ledPin = 2;
// threshold value to decide when the detected sound is a knock or not
const int threshold = 10; 

void setup()
{
    // use the serial port
    Serial.begin(9600);
    
    // set up our LED
    pinMode(ledPin, OUTPUT);
}

void loop()
{
    int analogValue;

    // read the sensor and store it in the variable sensorReading:
    analogValue = analogRead(piezoPin);

    // if the sensor reading is greater than the threshold:
    if (analogValue > threshold)
    {
        // send the string "Knock!" back to the computer, followed by newline
        Serial.println("Knock!");

        // turn on our LED
        digitalWrite(ledPin, HIGH);
        // we have to wait before turning it off so you can see it light up
        delay(500);
        digitalWrite(ledPin, LOW);
    }

    // delay to avoid overloading the serial port buffer
    delay(100);
}
