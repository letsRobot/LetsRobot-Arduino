
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <stdio.h>

#define SLAVE_ADDRESS 0x04
#define PIN 6

int lightIndex = 16; //This is the number of lights in the strip controlled by this arduino
Adafruit_NeoPixel strip = Adafruit_NeoPixel(lightIndex, 6, NEO_GRB + NEO_KHZ800);
//the first number is the number of LED's in the strip, the second is the output pin of the arduino

int number = 0;
//assign ints for each color
int R = 0; //Red
int G = 0; //Green
int B = 0; //Blue
int pixel = 0; //Pixel Number
int thisStep = 0; //Each light takes 4 steps, 1 to pick the pixel, and 3 to choose Red, Blue, and Green values
int allLights = 0; //Applies the color to all lights instead of just 1 of them.


void setup() {
  
  strip.begin(); //This Initializes the neoPixel library
  strip.show(); // Initialize all pixels to 'off'
  
  pinMode(13, OUTPUT);
  
  //initialize the i2c as a slave
  Wire.begin(SLAVE_ADDRESS);
  
  //define callbacks for i2c communcation
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
}

void loop() {
}

//callback for received data
void receiveData(int byteCount) {
  
  while(Wire.available()) {
    number = Wire.read();
    changeColor(); //changes the color of each pixel, duh!
  }
}

//callback for sending data
void sendData(){
  Wire.write(thisStep);
}

//input a sequence of 4 integers on the raspi which correspond to pixel #, and RGB values.
//the step in the sequence is returned before the next value can be taken in.
int changeColor(){
   if (thisStep == 0) {
     if (number == 255) { //Trigger behavior to change all the lights.
      allLights = 1; 
     }
     pixel = number;
     thisStep = 1;
   } else if (thisStep == 1) {
      R = number;
      thisStep = 2;
   } else if (thisStep == 2) {
      G = number;
      thisStep = 3;
   } else if (thisStep == 3) {
      B = number;
      if (allLights == 1) { //if allLights == 1, then change all lights to the corresponding color
        for (int i = 0; i < lightIndex; i++) {
         strip.setPixelColor(i, R, G, B); 
        } allLights = 0;
      } else {
      strip.setPixelColor(pixel, R, G, B);
      } 
      strip.show();
      thisStep = 0;
    }
    return (thisStep);
}


    
