/*
 * Tutorial 9: Door Alarm
 * 
 * Plays an alarm when the distance to the ultrasonic
 * range finder changes more then a preset tolerance.
 * Place infront of a door and press the push button to set
 * the alarm distance. Opening the door will now sound
 * the alarm, the alarm won't stop until the door is closed.
 *
 * The circuit:
 *  - 8-ohm speaker conected to digital pin 8 through a 100 ohm resistor
 *  - Push button connected to digital pin 2 and ground
 *  - 5v, ground connected to Ultrasonic sensor
 *  - digital pin 4 conneceted to Ultrasonic sensor echo pin
 *  - digital pin 5 conneceted to Ultrasonic sensor trig pin
 *
 * created August 26 2010
 * by Blaise Jarrett
 *
 * This example code is in the public domain.
 *
 * Derivative work from:
 * http://arduino.cc/en/Tutorial/Tone
 * http://www.arduino.cc/en/Tutorial/Ping
 *
 */

// include our list of note pitches
#include "pitches.h"

// the pins connected to the Ultrasonic sensor
int echoPin = 4;
int trigPin = 5;

// the pin connectd to our push button
int buttonPin = 2;

// the pin the speaker is attached to
int speakerPin = 8;

// a distance reading has to change more or less than
// the recorded distance and the tolerance
float distance_to_door_tolerance = 10;

// the distance to our door in CM
// -1 means it has not yet been set
float distance_to_door = -1;

// the notes in our melody and their duration in fractions of a second
// e.g. quarter note = 4, eighth note = 8, etc.
const int melody[][2] = 
{
    {NOTE_D7, 4},
    {NOTE_A6, 4},
    {NOTE_BLANK, 4}
};

void play_alarm()
{
    // figure out the number of notes in our melody
    int numberOfNotes = sizeof(melody) / sizeof(melody[0]);

    // iterate over the notes of the melody
    for (int thisNote = 0; thisNote < numberOfNotes; thisNote++)
    {
        // grab our note and note duration from our array
        int thisNoteTone = melody[thisNote][0];
        int thisNoteDuration = melody[thisNote][1];

        // to calculate the note duration in ms
        int noteDurationMS = 1000 / thisNoteDuration;

        // play the note
        tone(speakerPin, thisNoteTone, noteDurationMS);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        delay(noteDurationMS * 1.30);
    }
}

float get_distance_cm()
{
    float distanceCentimeters;
    int pulseLenMicroseconds;

    // bit-bang a small square wave
    // on the trig pin to start the range
    // finder
    digitalWrite(trigPin, LOW);
    delayMicroseconds(20);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(trigPin, LOW);

    // measure the pulse length from the echo pin
    pulseLenMicroseconds = pulseIn(echoPin, HIGH);

    // calculate the distance using the speed of sound
    distanceCentimeters = pulseLenMicroseconds / 29.387 / 2;

    return distanceCentimeters;
}

void setup()
{
    // set the pinmode on our ultrasonic echo, and tric pins
    pinMode(echoPin, INPUT);
    pinMode(trigPin, OUTPUT);
    // set the pinmode for our button to INPUT with pullup
    pinMode(buttonPin, INPUT_PULLUP);
}

void loop()
{
    int button;
    float distance;

    // read the ultrasonic sensor
    distance = get_distance_cm();

    // if the distance is greater or less then our preset alarm
    // distance, and we have set a distance, sound the alarm.
    if (distance > distance_to_door + distance_to_door_tolerance
        || distance < distance_to_door - distance_to_door_tolerance)
    {
        if (distance_to_door != -1)
            play_alarm();
    }

    // read our button state
    button = digitalRead(buttonPin);

    // if the button is pressed, set the distance to the last
    // distance read
    if (button == LOW)
    {
        distance_to_door = distance;
    }
}
