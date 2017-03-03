//Let's Robot Arduino Motor Controller

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

char inData[20]; // Allocate some space for the string
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character

//robot wheels servo address
int wrf = 0;
int wrb = 8;
int wlf = 2;
int wlb = 3;

//Steering servos for the wheels
int srf = 4; 
int srb = 5;
int slf = 6;
int slb = 7;

void setup() {
  Serial.begin(9600); //Sets up communication with serial monitor
  Serial.println("16 channel PWM test!\n");
  Serial.write("Power On\n");

  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode
  // some i2c devices dont like this so much so if you're sharing the bus, watch
  // out for this!

  pwm.begin();
  pwm.setPWMFreq(50);  // This is the maximum PWM frequency

  // save I2C bitrate
  uint8_t twbrbackup = TWBR;
  // must be changed after calling Wire.begin() (inside pwm.begin())
  TWBR = 12; // upgrade to 400KHz!
}

char Comp(char* This) {
  while (Serial.available() > 0) // Don't read unless
    // there you know there is data
  {
    if(index < 19) // One less than the size of the array
    {
      inChar = Serial.read(); // Read a character
      inData[index] = inChar; // Store it
      index++; // Increment where to write next
      inData[index] = '\0'; // Null terminate the string
    }
  }

  if (strcmp(inData,This)  == 0) {
    for (int i=0;i<19;i++) {
      inData[i]=0;
    }
    index=0;
    return(0);
  }
  else  {

    return(1);
  }
} 


void loop() {

  //Checks for a character in the serial monitor
  //if (Serial.available()>0) {
  //int x = Serial.parseInt();
  //Serial.println(x); 
  //}

  while (Comp != 0) {
    if (Comp("m1 on")==0) {
      Serial.write("Motor 1 -> Online\n");
    }
    else if (Comp("m1 off")==0) {
      Serial.write("Motor 1 -> Offline\n");
    }
    else if (Comp("s")==0) {
      Serial.write("Stopping\n"); 
      pwm.setPWM(0, 0, 0);
      pwm.setPWM(8, 0, 0);
      pwm.setPWM(2, 0, 0);
      pwm.setPWM(3, 0, 0);
      //pwm.setPWM(4, 0, 0);
      //pwm.setPWM(5, 0, 0);
      //pwm.setPWM(6, 0, 0);
      //pwm.setPWM(7, 0, 0);

    }  
    else if (Comp("f")==0) {
      Serial.write("Moving forward\n"); 
      pwm.setPWM(0, 0, 400);
      pwm.setPWM(8, 0, 400);
      pwm.setPWM(2, 0, 200);
      pwm.setPWM(3, 0, 200);
      pwm.setPWM(4, 0, 305);
      pwm.setPWM(5, 0, 260);
      pwm.setPWM(6, 0, 288);
      pwm.setPWM(7, 0, 290);

    }  
      else if (Comp("b")==0) {
      Serial.write("Moving Back\n"); 
      pwm.setPWM(0, 0, 200);
      pwm.setPWM(8, 0, 200);
      pwm.setPWM(2, 0, 400);
      pwm.setPWM(3, 0, 400);
      pwm.setPWM(4, 0, 305);
      pwm.setPWM(5, 0, 260);
      pwm.setPWM(6, 0, 288);
      pwm.setPWM(7, 0, 290);

    } 
    else if (Comp("r")==0) {
      Serial.write("Turning Right\n"); 
      pwm.setPWM(0, 0, 200);
      pwm.setPWM(8, 0, 200);
      pwm.setPWM(2, 0, 200);
      pwm.setPWM(3, 0, 200);
      pwm.setPWM(4, 0, 400);
      pwm.setPWM(5, 0, 170);
      pwm.setPWM(6, 0, 200);
      pwm.setPWM(7, 0, 400);


    }    
    else if (Comp("l")==0) {
      Serial.write("Turning Left\n"); 
      pwm.setPWM(0, 0, 400);
      pwm.setPWM(8, 0, 400);
      pwm.setPWM(2, 0, 400);
      pwm.setPWM(3, 0, 400);
      pwm.setPWM(4, 0, 400);
      pwm.setPWM(5, 0, 170);
      pwm.setPWM(6, 0, 200);
      pwm.setPWM(7, 0, 400);

    }
    
     else if (Comp("d")==0) {
      Serial.write("Strafing Right\n"); 
      pwm.setPWM(0, 0, 200);
      pwm.setPWM(8, 0, 400);
      pwm.setPWM(2, 0, 200);
      pwm.setPWM(3, 0, 400);
      pwm.setPWM(4, 0, 505);
      pwm.setPWM(5, 0, 80);
      pwm.setPWM(6, 0, 120);
      pwm.setPWM(7, 0, 488);

    }
    
         else if (Comp("a")==0) {
      Serial.write("Strafing left\n"); 
      pwm.setPWM(0, 0, 400);
      pwm.setPWM(8, 0, 200);
      pwm.setPWM(2, 0, 400);
      pwm.setPWM(3, 0, 200);
      pwm.setPWM(4, 0, 505);
      pwm.setPWM(5, 0, 80);
      pwm.setPWM(6, 0, 120);
      pwm.setPWM(7, 0, 488);

    }
    else {
          for (int i=0;i<19;i++) { 
      inData[i]=0;
    }
    index=0;
    }
  }
  Serial.write("Loop Ended\n");
}




