
/*
   Arduino sketch for Telly
   (c) Copyright 2016 Let's Robot.
*/

#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include "Encoder_1.h"  
#include "motor_driver.h"
#include "library.h"
#include "config.h"
#include "queue.h"

/***/

#define KNIFE_PIN 5

#ifdef TELLYBEAN
/*-------------------------------------------------------------------*/
int8_t ENC1pin = 7;   //Motor 1 Encoder A Pin
int8_t  ENC2pin = 12;   //Motor 2 Encoder A Pin
#define NEOPIXEL_PIN    10

#define GRIP_PWM        9
#define GRIP_INA        0
#define GRIP_INB        1
#define GRIP_LIMIT      3
#define FLASHLIGHT_PIN  5

#define UV_LED_PIN      4
#define DEFAULT_SPEED 1000 

       
#define ONE_REVOLUTION 768*2




int8_t PWMApin = 23;   //PWM Pin Controlling Motor A
int8_t INA1pin = 17;   //PIN Controlling Motor A output 1
int8_t INA2pin = 16;   //PIN Controlling Motor A output 2

int8_t PWMBpin = 22;   //PWM Pin Controlling Motor A
int8_t INB1pin = 20;   //PIN Controlling Motor A output 1
int8_t INB2pin = 21;   //PIN Controlling Motor A output 2


#endif
#ifdef TELLYTWITCH
/*-------------------------------------------------------------------*/
#define GRIPPER_PIN     9
#define NEOPIXEL_PIN    10
#define GRIPPER_OPEN 120
#define GRIPPER_CLOSED 40

#define ONE_REVOLUTION 768*2
//
//#define GRIP_PWM        9
//#define GRIP_INA        0
//#define GRIP_INB        1
//#define GRIP_LIMIT      3
//#define FLASHLIGHT_PIN  5
//#define UV_LED_PIN      4
int8_t  ENC1pin = 8;   //Motor 1 Encoder A Pin
int8_t  ENC2pin = 7;   //Motor 2 Encoder A Pin

int8_t PWMApin = 23;   //PWM Pin Controlling Motor A
int8_t INA1pin = 17;   //PIN Controlling Motor A output 1
int8_t INA2pin = 16;   //PIN Controlling Motor A output 2

int8_t PWMBpin = 22;   //PWM Pin Controlling Motor A
int8_t INB1pin = 20;   //PIN Controlling Motor A output 1
int8_t INB2pin = 21;   //PIN Controlling Motor A output 2

#define DEFAULT_SPEED 1500

#endif


Encoder_1 EncA(ENC1pin);
Encoder_1 EncB(ENC2pin);

Encoder_1 *EncA_pt = &EncA; 
Encoder_1 *EncB_pt = &EncB; 

queue movequeue;

Servo gripper_servo;
Servo knife_servo;

motor_driver Motor_A(INA1pin,INA2pin,PWMApin,EncA_pt);
motor_driver Motor_B(INB1pin,INB2pin,PWMBpin,EncB_pt);

void EncAupdate(){
EncA.update();
}
void EncBupdate(){
EncB.update();
}

uint8_t cRate = 20;          // Rate at which control loop runs
uint32_t prevMillis = 0;    // Previous Time in ms

int32_t setpointAdefault = DEFAULT_SPEED / cRate;
int32_t setpointBdefault = DEFAULT_SPEED / cRate;
int32_t TickswantedA = 0;  // Desired number of ticks for the motor to travel (1 rev = 3576)
int32_t TickswantedB = 0;  // Desired number of ticks for the motor to travel (1 rev = 3576)


uint32_t currentMillis = 0;  // Current Time in ms
uint16_t tstep = 1000 / cRate;      // Timestep between each control update

bool DC_control = false;
uint8_t ease = 5;
uint32_t stop_time;

uint8_t KP = 14;//15;                // Propoprtional Gain Constant
uint8_t KI = 12;//12;//8;                // Integral Gain Constant
bool motion_done = true;


bool open_grip = false;
uint8_t grip_strength = 0;
uint16_t deg_360 = 2925;

bool pos_control = false;

bool closing_flg = false;
uint32_t flashlight_timer = 0;
uint32_t distance_timer = 0;
bool flashlight_on = false;
uint8_t flashlight_max = 100;
#define FLASH_INTERVAL 20
bool uv_on = false;

/*
   Continuous rotation servos for the two wheels.  Because of the way the
   servos had to be mounted, they're facing different directions.  With this
   library, 90 is the stop value, 0 is full-speed in one direction, and 180
   is full-speed in the other direction.
*/

int dspeed = 10;
int tspeed = 10;
int default_drive_time = 1000;
int default_turn_time = 250;


#define NUM_LEDS         (9*2)



/*
   The mapping of LEDs as they appear on the NeoPixel strip, vs the human
   address we'd like to give them.
*/
// mirror (low towards center)
int led_map[NUM_LEDS] = {
   6,  //  0
  7,  //  1
  8,  //  2
  3,  //  3
  4,  //  4
  5,  //  5
  0,  //  6
  1,  //  7
  2,  //  8
  15,  //  9
  16,  // 10
  17,  // 11
  12,  // 12
  13,  // 13
  14,  // 14
  9,  // 15
  10,  // 16
  11, // 17
  /*
  0,  //  0
  3,  //  1
  6,  //  2
  1,  //  3
  4,  //  4
  7,  //  5
  2,  //  6
  5,  //  7
  8,  //  8
  15,  //  9
  16,  // 10
  17,  // 11
  12,  // 12
  13,  // 13
  14,  // 14
  9,  // 15
  10,  // 16
  11,  // 17
  */
};


Adafruit_NeoPixel eyes = Adafruit_NeoPixel(NUM_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
int eye_state;
bool eyes_no_show = false;


/***/

/*
   Asyncrhonously scheduled stop.

   Values:
      0   No stop scheduled
      1   Stop immediately
     >1   Stop when millis() is greater than this number
*/


void schedule_stop(unsigned long timer) {
  if (timer > 1)
    stop_time = millis() + timer;
  else
    stop_time = timer;
}

void stop(void) {

   closing_flg = false;
  stop_time = 0;
}

#define EYE_BLINK_COLOR "60 60 60"

void blink_open() {
  eyes_no_show = true;
  do_command("led 255 " EYE_BLINK_COLOR);
  do_command("led   5 " "0 0 0");
  do_command("led  14 " "0 0 0");
  eyes_no_show = false;
  eyes_show();
}

void blink_close () {
  eyes_no_show = true;
  do_command("led 255 0 0 0");

  do_command("led   4 " EYE_BLINK_COLOR);
  do_command("led   5 " EYE_BLINK_COLOR);
  do_command("led   6 " EYE_BLINK_COLOR);

  do_command("led  13 " EYE_BLINK_COLOR);
  do_command("led  14 " EYE_BLINK_COLOR);
  do_command("led  15 " EYE_BLINK_COLOR);
  eyes_no_show = false;
  eyes_show();
}

/***/

void eyes_show() {
  if (eyes_no_show)
    return;
  //  servo_detach();
  eyes.show();
  //  servo_attach();
}

void servo_attach() {
  gripper_servo.attach(GRIPPER_PIN);
  knife_servo.attach(KNIFE_PIN);
}

void servo_detach() {
    gripper_servo.detach();

}

void setup() {
  add_command("forward",  cmd_forward);
  add_command("back",     cmd_back);
  add_command("backward", cmd_back);
  add_command("left",     cmd_left);
  add_command("right",    cmd_right);
  //add_command("stop",     cmd_stop);
  add_command("f",        cmd_f);
  add_command("b",        cmd_b);
  add_command("l",        cmd_l);
  add_command("r",        cmd_r);
  
  add_command("led",      cmd_led);
  add_command("blink",    cmd_blink);
  add_command("open",    cmd_open);
  add_command("close",    cmd_close);

  add_command("ticks", cmd_ticks);
  //add_command("setv", cmd_setv);
  add_command("speedtwitchgo", cmd_setv);
  //add_command("pan",     cmd_pan);
  //
  //add_command("pos", cmd_pos);
  //add_command("grab",     cmd_grab);
  add_command("angle",     cmd_r_ticks);
  add_command("tics",     cmd_ticks);

  add_command("rticks", cmd_r_ticks);
  
  add_command("lticks", cmd_l_ticks);
  add_command("brightness", cmd_brightness);
  add_command("doot", cmd_doot);

  Serial.begin(19200);

  Serial.println("Check1");
delay(1000);
Serial.flush(); 
  /*
     For backwards compatability with the old protocol, only, which used I2C
     to control the LEDs.

     Become an I2C slave.  First change the bitrate to 400khz, which must be
     set before calling Wire.begin().  Then set the I2C callback functions.
  */
  //Wire.begin(I2C_SLAVE, I2C_ADDRESS, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE_400);
  //Wire.onReceive(receiveData);
  //Wire.onRequest(sendData);

  eyes.begin();
  eyes.setBrightness(80);
  eyes_show();


  servo_attach();
  
        pinMode(LED_BUILTIN, OUTPUT);
  
        for (int i = 0; i < 8; i++) {
            digitalWrite(LED_BUILTIN, LOW);
            delay(33);
  
            digitalWrite(LED_BUILTIN, HIGH);
            delay(33);
        }


  cmd_blink();
  attachInterrupt(ENC1pin, EncAupdate, CHANGE);
  attachInterrupt(ENC2pin, EncBupdate, CHANGE);
  Motor_A.default_setup(setpointAdefault,KP,KI,ease,tstep);
  Motor_B.default_setup(setpointBdefault+8,KP,KI,ease,tstep);
//  Gripper_Setup(GRIP_PWM,GRIP_INA,GRIP_INB);
  
  Serial.println("check2");
  Serial.flush();
  pinMode(LED_BUILTIN, OUTPUT); // LED
  digitalWrite(LED_BUILTIN, HIGH);
  analogWriteFrequency(9, 30000);
  cmd_open();
}

void loop() {
  library_loop();

}

//void Gripper_Setup(int8_t PWMpin, int8_t Dir1pin, int8_t Dir2pin)
//{
//  pinMode(PWMpin, OUTPUT);
//  pinMode(Dir1pin, OUTPUT);
//  pinMode(Dir2pin, OUTPUT);
//    pinMode(GRIP_LIMIT, INPUT);
//  digitalWrite(GRIP_LIMIT,HIGH);
//}

