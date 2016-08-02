// vim:set sw=4 ts=4 ai et:

/*
 * Arduino sketch for Telly
 * (c) Copyright 2016 Let's Robot.
 */

#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

// set up enums for config
#define SERVO_EXIST 1
#define DCMOTOR_EXIST 2

#include "library.h"
#include "config.h"

/*** PINOUT MAPPINGS ***/

#ifdef TEENSY
    #define GRIPPER_PIN     2
    #define NEOPIXEL_PIN    3
    #define LEFT_PIN        8
    #define RIGHT_PIN       10
    #ifdef HAS_DCMOTOR
        #define DIR1PINA 21
        #define DIR2PINA 20
        #define SPEEDPINA 9

        #define DIR1PINB 19
        #define DIR2PINB 18
        #define SPEEDPINB 10
    #endif
#else
    #define GRIPPER_PIN     2222
    #define NEOPIXEL_PIN    6
    #define LEFT_PIN        9
    #define RIGHT_PIN       10
#endif

/*
 * Continuous rotation servos for the two wheels.  Because of the way the
 * servos had to be mounted, they're facing different directions.  With this
 * library, 90 is the stop value, 0 is full-speed in one direction, and 180
 * is full-speed in the other direction.
 */
//#define LEFT_STOP       95
//#define LEFT_FORWARD    (LEFT_STOP  -10)
//#define LEFT_BACKWARD   (LEFT_STOP  +10)
//
//#define RIGHT_STOP      95
//#define RIGHT_FORWARD   (RIGHT_STOP +10)
//#define RIGHT_BACKWARD  (RIGHT_STOP -10)

/*
 * 5/12/16 - rtharp
 * Sketch uses 15,298 bytes (47%) of program storage space. Maximum is 32,256 bytes.
 * Global variables use 1,755 bytes (68%) of dynamic memory, leaving 805 bytes for local variables. Maximum is 2,560 bytes.
 */


#ifdef HAS_SERVO
    const int left_stop=95;
    const int right_stop=95;
//#if DRIVE_TRAIN==SERVO_EXIST
  #ifdef WANT_MTIMINGS
    int dspeed=10;
    int tspeed=10;
  #else
    #define dspeed 10
    #define tspeed 10
  #endif
#endif

#ifdef HAS_DCMOTOR
  const int left_stop=255;
  const int right_stop=255;
  #ifdef WANT_MTIMINGS
    int dspeed=255;
    int tspeed=255;
  #else
    #define dspeed 255
    #define tspeed 255
  #endif
#endif



#ifdef HAS_SERVO
  Servo left_servo, right_servo;
#endif
#ifdef HAS_GRIPPER
  Servo gripper_servo;
#endif
#ifdef HAS_PANTILT
  Servo pan_servo, tilt_servo;
#endif

/*
 * How long in milliseconds to drive the motors when moving forward and
 * backward, and left and right.
 */
#define MAX_DRIVE_TIME      3000
#define MAX_TURN_TIME       3000
//#define DEFAULT_DRIVE_TIME  1000
//#define DEFAULT_TURN_TIME    250

#ifdef WANT_MTIMINGS
  int default_drive_time=1000;
  int default_turn_time=250;
#else
  #define default_drive_time 1000
  #define default_turn_time 250
#endif

#ifdef HAS_GRIPPER
  int gripper_open=180;
  int gripper_close=0;
#endif

#if defined(I2C_LIGHTS) && defined(I2C_MOTOR)
  bool i2c_compat=true;
  //bool i2c_v3=true;
#endif
#if defined(I2C_LIGHTS) || defined(I2C_MOTOR)
  /*
   * I2C, for supporting the old protocol's LED interface
   */
  #define I2C_ADDRESS      0x04
  #define I2C_TIMEOUT_TIME 1000
#endif

#define NUM_LEDS         (9*2)
/*
 * The mapping of LEDs as they appear on the NeoPixel strip, vs the human
 * address we'd like to give them.
 */
// starts (0) bot's farthest left top, end bot's right bottom (1)
#ifdef TELLY3
  // upper stage left to bottom right
  int led_map[NUM_LEDS] = {
         11,  //  0
         10,  //  1
          9,  //  2
         14,  //  3
         13,  //  4
         12,  //  5
         17,  //  6
         16,  //  7
         15,  //  8

          2,  //  9
          1,  // 10
          0,  // 11
          5,  // 12
          4,  // 13
          3,  // 14
          8,  // 15
          7,  // 16
          6,  // 17
      };

#else
  // mirror (low towards center)
  int led_map[NUM_LEDS] = {
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
      };
#endif

Adafruit_NeoPixel eyes = Adafruit_NeoPixel(NUM_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

#if defined(I2C_LIGHTS) || defined(I2C_MOTOR)
  int eye_state;
#endif

#ifdef WANT_BLINK
  bool eyes_no_show = false;
#endif

/***/

/*
 * Asyncrhonously scheduled stop.
 *
 * Values:
 *    0   No stop scheduled
 *    1   Stop immediately
 *   >1   Stop when millis() is greater than this number
 */
unsigned int stop_time;
//unsigned short gstop_time;

void schedule_stop(unsigned long timer) {
    if (timer > 1)
        stop_time = millis() + timer;
    else
        stop_time = timer;
}

/*
void schedule_gstop(unsigned long timer) {
    if (timer > 1)
        gstop_time = millis() + timer;
    else
        gstop_time = timer;
}
*/

void stop(void) {
    #ifdef HAS_SERVO
      left_servo.write(left_stop);
      right_servo.write(right_stop);
    #endif

    #ifdef HAS_DCMOTOR
      analogWrite(SPEEDPINA, 0);
      digitalWrite(DIR1PINA, LOW);
      digitalWrite(DIR2PINA, HIGH);
      analogWrite(SPEEDPINB, 0);
      digitalWrite(DIR1PINB, LOW);
      digitalWrite(DIR2PINB, HIGH);
    #endif
    /*
     * If the value was 1, this is a special case, and the STOP is being
     * executed immediately.  Return the normal OK string.  Otherwise, the
     * stop is the result of a previously scheduled asynchronous movement
     * command.  Return the asynchronous OK  string.
     */

#ifdef WANT_SERIAL
    if (compat) {
        OK();
    }
    else {
#endif 
        if (stop_time == 1)
            OK();
        else
            redraw_prompt = true;
            Serial.println("\rASYNC_OK");
#ifdef WANT_SERIAL
    }
#endif 

    stop_time = 0;
}

/*
void gstop(void) {
    gripper_servo.write(left_stop);

    if (compat) {
        OK();
    }

    else {
        if (stop_time == 1)
            OK();
        else
            redraw_prompt = true;
            Serial.println("\rASYNC_OK");
    }

    gstop_time = 0;
}
*/

void move(int left, int right, int default_time) {
    unsigned int duration;

    duration = nextarg_int(10, MAX_DRIVE_TIME, default_time, -1);
    Serial.println(format("Moving for %d/%d: %d", left, right, duration));
    if (duration < 0) {
        Serial.println(format("Out of range: %d", duration));
        Serial.println("ASYNC_ERR Invalid argument");
        return;
    }
    #ifdef WANT_SERIAL
      if (! compat)
    #endif
        Serial.println("ASYNC_RUNNING");
    #ifdef HAS_SERVO
      left_servo.write(left);
      right_servo.write(right);
    #endif
    #ifdef HAS_DCMOTOR
      int lspeed=abs(left_stop-left);
      int rspeed=abs(right_stop-right);
      Serial.println(format("Speed: %d %d", lspeed, rspeed));
      if (left<left_stop) {
        // motor 1 forward
        analogWrite(SPEEDPINA, lspeed);
        digitalWrite(DIR1PINA, HIGH);
        digitalWrite(DIR2PINA, LOW);
      } else if (left>left_stop) {
        // motor 1 backwards
        analogWrite(SPEEDPINA, lspeed);
        digitalWrite(DIR1PINA, LOW);
        digitalWrite(DIR2PINA, HIGH);
      }
      if (right<right_stop) {
        // motor 2 forward
        analogWrite(SPEEDPINB, rspeed);
        digitalWrite(DIR1PINB, HIGH);
        digitalWrite(DIR2PINB, LOW);
      } else if (right>left_stop) {
        // motor 2 backwards
        analogWrite(SPEEDPINB, rspeed);
        digitalWrite(DIR1PINB, LOW);
        digitalWrite(DIR2PINB, HIGH);
      }
    #endif
    schedule_stop(duration);
}

// heartbeat for checking for stop
void heartbeat(void) {
    if (0 < stop_time && stop_time < millis())
        stop();
}

/***/

void cmd_forward (void) { move(left_stop-dspeed,  right_stop+dspeed, default_drive_time); }
void cmd_backward    (void) { move(left_stop+dspeed,  right_stop-dspeed, default_drive_time); }
void cmd_left    (void) { move(left_stop+tspeed,  right_stop+tspeed, default_turn_time);  }
void cmd_right   (void) { move(left_stop-tspeed,  right_stop-tspeed, default_turn_time);  }

/*
void cmd_forward (void) { move(LEFT_FORWARD,  RIGHT_FORWARD,  default_drive_time); }
void cmd_back    (void) { move(LEFT_BACKWARD, RIGHT_BACKWARD, default_drive_time); }
void cmd_left    (void) { move(LEFT_BACKWARD, RIGHT_FORWARD,  default_turn_time);  }
void cmd_right   (void) { move(LEFT_FORWARD,  RIGHT_BACKWARD, default_turn_time);  }
*/

void cmd_stop(void) {
    if (stop_time)
        schedule_stop(1);
    heartbeat();
    OK();
}

#ifdef WANT_MTIMINGS
  void cmd_setdt(void) {
      int time = nextarg_int(1, 0x7fff, 3000, -1);
      //Serial.println(format("Drive time set: %d", spd));
      default_drive_time=time;
      OK();
  }

  void cmd_settt(void) {
      int time = nextarg_int(1, 0x7fff, 250, -1);
      //Serial.println(format("Turn time set: %d", spd));
      default_turn_time=time;
      OK();
  }

  void cmd_setds(void) {
      uint8_t spd = nextarg_int(1, 20, 10, -1);
      Serial.println(format("Drive speed set: %d", spd));
      dspeed=spd;
      OK();
  }

  void cmd_setts(void) {
      uint8_t spd = nextarg_int(1, 20, 10, -1);
      Serial.println(format("Turn speed set: %d", spd));
      tspeed=spd;
      OK();
  }
#endif

#ifdef HAS_GRIPPER
  void cmd_setgo(void) {
      int pos= nextarg_int(-360, 360, 180, -1);
      //Serial.println(format("Gripper open position set: %d", pos));
      gripper_open=pos;
      OK();
  }

  void cmd_setgc(void) {
      int pos= nextarg_int(-360, 360, 0, -1);
      //Serial.println(format("Gripper close position set: %d", pos));
      gripper_close=pos;
      OK();
  }

  void cmd_open(void) {
      //Serial.println(format("setting gripper %d", gripper_open));
      gripper_servo.write(gripper_open);
      //schedule_gstop(1000);
      OK();
  }

  void cmd_close(void) {
      //Serial.println(format("setting gripper %d", gripper_close));
      gripper_servo.write(gripper_close);
      //schedule_gstop(1000);
      OK();
  }
#endif

#ifdef HAS_PANTILT
  void cmd_pan(void) {
      int pos= nextarg_int(-360, 360, 0, -1);
      //Serial.println(format("setting pan %d", pos));
      pan_servo.write(pos);
  }

  void cmd_tilt(void) {
      int pos= nextarg_int(-360, 360, 0, -1);
      //Serial.println(format("setting tilt %d", pos));
      tilt_servo.write(pos);
  }
#endif

/*
void cmd_i2c2(void) {
  i2c_compat=false;
  OK();
}
*/

void cmd_led(void) {
    int pixel = nextarg_int(0, 0xff, -1, -1);
    int red   = nextarg_int(0, 0xff, -1, -1);
    int green = nextarg_int(0, 0xff, -1, -1);
    int blue  = nextarg_int(0, 0xff, -1, -1);

    int i;

    if (red < 0 || green < 0 || blue < 0 || pixel < 0) {
        ERR("Invalid led argument");
        return;
    }

    /* use i2c to turn it off if you're going to use the i2c mode */
    /*
    if (pixel == 254) {
      verbose("cmd_led setting i2c_compat off");
      i2c_compat=false;
      OK();
      return;
    }
    */

    if (pixel == 0xFF) {
        for (i = 0; i < NUM_LEDS; i++)
            eyes.setPixelColor(i, red, green, blue);
        //servo_detach();
        eyes_show();
        //servo_attach();
        OK();
        return;
    }

    /*
     * The human-readable pixel index is 1 based, so the valid range
     * is [1, NUM_LEDS], not [0, NUM_LEDS-1].  After input validation,
     * subtract one from index to use a stanard 0-based array.
     */
    if (! (1 <= pixel && pixel <= NUM_LEDS))
        return ERR(format("LED index out of range %d", pixel));

    pixel -= 1;

    eyes.setPixelColor(led_map[pixel], red, green, blue);
    // I think we detach to avoid twitching due to the power flux
    //servo_detach();
    eyes_show();
    //servo_attach();
    OK();
}

/*
 * I2C callbacks, for the old protocol
 */

#ifdef I2C_MOTOR
struct result_s{
  unsigned int request;
  unsigned long result;
};

unsigned resbuf=0;
result_s results[5];
//char i2c_buffer[5]="";
//unsigned short i2cbuflen=0;
#endif

void sendData() {
    //verbose("I2C write eye_state %d", eye_state);
    //Serial.println(format("I2C write eye_state %d", eye_state));
#if defined(I2C_LIGHTS) && defined(I2C_MOTOR)
    if (i2c_compat) {
#endif
#ifdef I2C_LIGHTS
      Wire.write(eye_state);
#endif
#if defined(I2C_LIGHTS) && defined(I2C_MOTOR)
    } else {
#endif
#ifdef I2C_MOTOR
      //verbose("new i2c mode");
      //Serial.println("new i2c mode");
      // lets write 0 if there's nothing there
      // otherwise byte size
      // then the bytes
      if (resbuf) {
        Wire.write(results[0].request);
        Wire.write(results[0].result);
        for(int i=0; i<5; i++) {
          results[i]=results[i+1];
        }
        resbuf--;
      }
#endif
      /*
      if (i2cbuflen) {
        Wire.write(i2c_buffer);
        i2c_buffer[0]=0;
      } else {
        //verbose("nothing in buffer");
        //Serial.println("nothing in buffer");
      }
      */
#if defined(I2C_LIGHTS) && defined(I2C_MOTOR)
    }
#endif
}

#ifdef I2C_MOTOR
// or uint8_t
int i2c_input[5];
//unsigned char i2c_output[5];
#endif

void receiveData(int num_bytes) {
#if defined(I2C_LIGHTS)
    static int R, G, B;
    static int pixel;
#endif

    static unsigned long last_read;

    verbose("I2C receiveDat num_bytes %d", num_bytes);
    //Serial.println(format("I2C receiveDat num_bytes %d", num_bytes));

    /*
     * A timeout event, to help avoid out-of-sync errors with the Pi.  If we
     * haven't heard anything from the Pi in over half a second, go back to
     * state 0, assuming the next byte we're gonig to read is the start of a
     * new LED command.
     */
    //if (i2c_compat) {
    // probably a good idea for everything
    if (millis() - last_read > I2C_TIMEOUT_TIME && eye_state) {
      Serial.println(format("resetting eye_state %d timedout", eye_state, millis() - last_read));
      eye_state = 0;
    }
    last_read = millis();
    //}

    // read each byte, one by one
    while (Wire.available()) {
        uint8_t val = Wire.read();
        verbose("I2C read %d", val);
        //Serial.println(format("I2C read %d", val));

#if defined(I2C_LIGHTS) && defined(I2C_MOTOR)
        if (i2c_compat) {
#endif
#ifdef I2C_LIGHTS
          switch (eye_state) {
              case 0:
                  /*
                  if (val==254) {
                    i2c_compat=false;
                    verbose(format("toggling i2c_compat %d", i2c_compat));
                    Serial.println(format("toggling i2c_compat %d", i2c_compat));
                    eye_state=0;
                    return;
                  }
                  */
#if defined(I2C_LIGHTS) && defined(I2C_MOTOR)
                  if (val==253) {
                    i2c_compat=false;
                    //i2c_v3=true;
                    verbose(format("toggling i2c_v3 %d", i2c_compat));
                    Serial.println(format("toggling i2c_v3 %d", i2c_compat));
                    eye_state=0;
                    return;
                  }
#endif
                  /*
                  if (val==253) {
                    i2c_compat=true;
                    verbose(format("toggling i2c_compat %d", i2c_compat));
                    Serial.println(format("toggling i2c_compat %d", i2c_compat));
                    return;
                  }
                  if (val==254) {
                    i2c_compat=!i2c_compat;
                    verbose(format("toggling i2c_compat %d", i2c_compat));
                    Serial.println(format("toggling i2c_compat %d", i2c_compat));
                    return;
                  }
                  */
                  pixel = val;
                  eye_state++;
                  break;

              case 1:
                  R = val;
                  eye_state++;
                  break;

              case 2:
                  G = val;
                  eye_state++;
                  break;

              case 3:
                  B = val;
                  eye_state = 0;
                  do_command(format("led %d %d %d %d", pixel, R, G, B));
                  break;
          } // end switch
#endif
#if defined(I2C_LIGHTS) && defined(I2C_MOTOR)
        } else {
#endif
#ifdef I2C_MOTOR
          // what now
          //if (i2c_v3) {
            // we're goingt o make all commands 5 bytes
            // servos:
            // 0 left
            // 1 right
            // 2 gripper
            // 3 pan
            // 4 tilt
            //
            // 0-led:      command,i,r,g,b
            // 10-start:    command,servo1,speed1,servo2,speed2 (new low)
            // 15-stop:     command,servo1,servo2,0,0 (new low)
            // 20-step:     command,servo,tics,dir,speed (new low, tics & dirs will just be an signed short -32767 to 32767)
            // 25-setAngle: command,servo1,angle1,servo2,angle2 (new low, pan/tilt/grippers)
            // 230-setPos:   command,servo,pos,angle,0 (old, 180, gripper,closed)
            // 235-gotoPos:  command,servo,pos,0,0 (old, gripper, open)
            // 240-setTime:  command,dir,time1,time2,0 (old, dir is straight or turn, time is unsigned short 0-65535 in ms, older drive method)
            // 245-setSpeed: command,dir,speed,0,0 (old, dir is straight or turn, older drive method)
            // 247-setStop:  command,dir,speed,0,0 (old, usually 90)
            // 250-goFor:    command,motion,0,0,0 (old, should know left/right motors already)
            // -request:  command,0,0,0,0
            //Serial.println(format("i2c_v3 put %d in %d", val, eye_state));
            i2c_input[eye_state]=val;
            eye_state++;
#ifdef WANT_MTIMINGS
            unsigned int t=0;
#endif
            if (eye_state==5) {
              // we have 0-4 bytes
              Serial.println(format("i2c_v3 exec %d_[%d %d %d %d]", i2c_input[0], i2c_input[1], i2c_input[2], i2c_input[3], i2c_input[4]));
              // execute command
              switch(i2c_input[0]) {
                case 0: // led command,i,r,g,b
                  do_command(format("led %d %d %d %d", i2c_input[1], i2c_input[2], i2c_input[3], i2c_input[4]));
                break;
                case 10: // start command,servo1,speed1,servo2,speed2 (new low, 70<90<100 speed)
                  Serial.println(format("i2c_v3 servo1 %d=%d servo2 %d=%d", i2c_input[1], i2c_input[2], i2c_input[3], i2c_input[4]));
                  switch(i2c_input[1]) {
                    case 0: // left
                      Serial.println(format("i2c_v3 left1 to %d", i2c_input[2]));
#ifdef HAS_SERVO
                      left_servo.write(i2c_input[2]);
#endif
                    break;
                    case 1: //right
                      Serial.println(format("i2c_v3 right1 to %d", i2c_input[2]));
#ifdef HAS_SERVO
                      right_servo.write(i2c_input[2]);
#endif
                    break;
                  }
                  switch(i2c_input[3]) {
                    case 0: // left
                      Serial.println(format("i2c_v3 left2 to %d", i2c_input[2]));
#ifdef HAS_SERVO
                      left_servo.write(i2c_input[4]);
#endif
                    break;
                    case 1: //right
                      Serial.println(format("i2c_v3 right2 to %d", i2c_input[2]));
#ifdef HAS_SERVO
                      right_servo.write(i2c_input[4]);
#endif
                    break;
                  }
                break;
                case 15: // stop command,servo1,servo2,0,0 (new low)
                  switch(i2c_input[1]) {
                    case 0: // left
                      Serial.println("i2c_v3 #1 left stop");
#ifdef HAS_SERVO
                      left_servo.write(left_stop);
#endif
                    break;
                    case 1: //right
                      Serial.println("i2c_v3 #1 right stop");
#ifdef HAS_SERVO
                      right_servo.write(right_stop);
#endif
                    break;
                  }
                  switch(i2c_input[2]) {
                    case 0: // left
                      Serial.println("i2c_v3 #2 left stop");
#ifdef HAS_SERVO
                      left_servo.write(left_stop);
#endif
                    break;
                    case 1: //right
                      Serial.println("i2c_v3 #2 right stop");
#ifdef HAS_SERVO
                      right_servo.write(right_stop);
#endif
                    break;
                  }
                break;
                case 20: // step:     command,servo,tics,dir,speed (new low, tics & dirs will just be an signed short -32767 to 32767)
                  // FIXME/TODO
                  switch(i2c_input[1]) {
                    case 0: // left
                      // tbd
                    break;
                    case 1: //right
                      // tbd
                    break;
                  }
                break;
                case 25: // setAngle: command,servo1,angle1,servo2,angle2 (new low, pan/tilt/grippers)
                  switch(i2c_input[1]) {
                    case 2: // gripper
#ifdef HAS_GRIPPER
                      gripper_servo.write(i2c_input[2]);
#endif
                    break;
                    case 3: // pan
#ifdef HAS_PANTILT
                      pan_servo.write(i2c_input[2]);
#endif
                    break;
                    case 4: // tilt
#ifdef HAS_PANTILT
                      tilt_servo.write(i2c_input[2]);
#endif
                    break;
                  }
                  switch(i2c_input[3]) {
                    case 2: // gripper
#ifdef HAS_GRIPPER
                      gripper_servo.write(i2c_input[4]);
#endif
                    break;
                    case 3: // pan
#ifdef HAS_PANTILT
                      pan_servo.write(i2c_input[4]);
#endif
                    break;
                    case 4: // tilt
#ifdef HAS_PANTILT
                      tilt_servo.write(i2c_input[4]);
#endif
                    break;
                  }
                break;
                case 230: // setPos: command,servo,pos,angle,0 (old, 180, gripper,closed)
                  if (i2c_input[1]==2) { // gripper
#ifdef HAS_GRIPPER
                      if (i2c_input[2]) { // pos = 0 open
                        gripper_open=i2c_input[3];
                      } else {
                        gripper_close=i2c_input[4];
                      }
#endif
                  }
                break;
                case 235: // gotoPos: command,servo,pos,0,0 (old, gripper, open)
#ifdef HAS_GRIPPER
                  if (i2c_input[1]==2) { // gripper
                      if (i2c_input[2]) { // pos = 0 open
                        gripper_servo.write(gripper_open);
                      } else {
                        gripper_servo.write(gripper_close);
                      }
                  }
#endif
                break;
                case 240: // setTime: command,dir,time1,time2,0 (old, dir is straight or turn, time is unsigned short 0-65535 in ms, older drive method)
#ifdef WANT_MTIMINGS
                  t=(i2c_input[3]*255)+i2c_input[2];
                  if (i2c_input[1]) {
                    // turn
                    Serial.println(format("i2c_v3 set turn time to %d from %d & %d", t, i2c_input[3], i2c_input[2]));
                    default_turn_time=t;
                  } else {
                    // straight
                    Serial.println(format("i2c_v3 set drive time to %d from %d & %d", t, i2c_input[3], i2c_input[2]));
                    default_drive_time=t;
                  }
#endif
                break;
                case 245: // setSpeed: command,dir,speed,0,0 (old, dir is straight or turn, older drive method)
#ifdef WANT_MTIMINGS
                  if (i2c_input[1]) {
                    // turn
                    Serial.println(format("i2c_v3 set turn speed to %d", i2c_input[2]));
                    tspeed=i2c_input[2];
                  } else {
                    // straight
                    Serial.println(format("i2c_v3 set drive speed to %d", i2c_input[2]));
                    dspeed=i2c_input[2];
                  }
#endif
                break;
                case 247: // setStop command,dir,speed,0,0 (old, usually 90)
                  stop();
                break;
                case 250: // goFor: command,motion,0,0,0 (old, should know left/right motors already)
                  switch(i2c_input[1]) {
                    case 0:
                      Serial.println("forward");
                      cmd_forward();
                    break;
                    case 1:
                      Serial.println("backward");
                      cmd_backward();
                    break;
                    case 2:
                      Serial.println("left");
                      cmd_left();
                    break;
                    case 3:
                      Serial.println("right");
                      cmd_right();
                    break;
                  }
                break;
                case 253:
                  // reset motors
                  servo_detach();
                  servo_attach();
                break;
                case 254:
                  // ping
                  Serial.println("ping");
                break;
                case 255:
                  // reset teensy
                  cmd_reset();
                break;
              } // end switch 
              eye_state=0;
          /*
          } else {
            // change timing/speed
            Serial.println(format("eye_state %d", eye_state));
            if (eye_state==0) {
              switch(val) {
                case 1:
                  cmd_forward();
                break;
                case 2:
                  cmd_backward();
                break;
                case 3:
                  cmd_left();
                break;
                case 4:
                  cmd_right();
                break;
                case 5:
                  cmd_open();
                break;
                case 6:
                  cmd_close();
                break;
                case 7:
                  // pan
                break;
                case 8:
                  // tilt
                break;
                case 9:
                  // light
                  eye_state++;
                break;
                case 10:
                  // need to be able to values from −8,388,608 to 8,388,608 so 24bits (minute in ms)
                  // set&type <= 1 byte
                  // value    <= 3 bytes

                  // 32767 to 32767 16bit signed (short)
                  // time can never be negative, so an unsigned short for minute in ms should be fine
                  // servo (angular) 0 to 180 unsigned char

                  // setTime&type <= 1 byte
                  // time <= 2 bytes
                  // setAngle&type <= 1 byte
                  // time <= 1 byte
                  // setLight <= 1 byte
                  // pixel&color <= 4 bytes
                break;
                case 11: // up
                  do_command(format("tilt 180"));
                break;
                case 12: // center
                  do_command(format("tilt 120"));
                break;
                case 13: // down
                  do_command(format("tilt 0"));
                break;
                case 14: // left
                  do_command(format("pan 0"));
                break;
                case 15: // center
                  do_command(format("pan 90"));
                break;
                case 16: // right
                  do_command(format("pan 180"));
                break;
                default:
                  Serial.println(format("unknown i2c val %d", val));
                break;
              }
            } else if (eye_state==1) {
              pixel = val;
              eye_state++;
            } else if (eye_state==2) {
              R = val;
              eye_state++;
            } else if (eye_state==3) {
              G = val;
              eye_state++;
            } else if (eye_state==4) {
              B = val;
              eye_state=0;
              do_command(format("led %d %d %d %d", pixel, R, G, B));
            } else {
              Serial.println(format("unknown eyestate %d", eye_state));
            } // end eye_state if
          }
          */
       } // end if exec
#endif
#if defined(I2C_LIGHTS) && defined(I2C_MOTOR)
    } // end if i2c_compat check
#endif
  } // end while
}

/***/
#ifdef WANT_BLINK
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

  void cmd_blink() {
      blink_open();   delay(350);
      blink_close(); delay(100);

      blink_open();   delay(350);
      blink_close(); delay(100);

      blink_open();
  }
#endif

/***/

void eyes_show() {
#ifdef WANT_BLINK
    if (eyes_no_show)
        return;
#endif
    // prevent motor twitch due to power flux
    //servo_detach();
    eyes.show();
    //servo_attach();
}

void servo_attach() {
    verbose("servo_attach");
#ifdef HAS_SERVO
    left_servo.attach(LEFT_PIN);
    right_servo.attach(RIGHT_PIN);
#endif
#ifdef HAS_GRIPPER
    gripper_servo.attach(GRIPPER_PIN);
#endif
#ifdef HAS_PANTILT
    pan_servo.attach(12);
    // was 6, now 12
    tilt_servo.attach(11);
#endif
#ifdef HAS_DCMOTOR
    pinMode(DIR1PINA,OUTPUT);
    pinMode(DIR2PINA,OUTPUT);
    pinMode(SPEEDPINA,OUTPUT);
    pinMode(DIR1PINA,OUTPUT);
    pinMode(DIR2PINA,OUTPUT);
    pinMode(SPEEDPINB,OUTPUT);
#endif
}

void servo_detach() {
    verbose("servo_detach");
#ifdef HAS_SERVO
    left_servo.detach();
    right_servo.detach();
#endif
#ifdef HAS_GRIPPER
    gripper_servo.detach();
#endif
#ifdef HAS_PANTILT
    pan_servo.detach();
    tilt_servo.detach();
#endif
#ifdef HAS_DCMOTOR
/*
analogWrite(speedPinA, 0);
digitalWrite(dir1PinA, LOW);
digitalWrite(dir2PinA, HIGH);
 */
    for (int thisPin = 2; thisPin < 11; thisPin++) {
      digitalWrite(thisPin, LOW);
    }
#endif
}

void setup() {
    add_command("forward",  cmd_forward);
    add_command("back",     cmd_backward);
    add_command("backward", cmd_backward);
    add_command("left",     cmd_left);
    add_command("right",    cmd_right);
    add_command("stop",     cmd_stop);
    add_command("f",        cmd_forward);
    add_command("b",        cmd_backward);
    add_command("l",        cmd_left);
    add_command("r",        cmd_right);
    add_command("s",        cmd_stop);
    add_command("led",      cmd_led);
#ifdef WANT_BLINK
    add_command("blink",    cmd_blink);
#endif
#ifdef WANT_MTIMINGS
    add_command("setdt",    cmd_setdt);
    add_command("settt",    cmd_settt);
    add_command("setds",    cmd_setds);
    add_command("setts",    cmd_setts);
#endif
#ifdef HAS_GRIPPER
    add_command("setgo",    cmd_setgo);
    add_command("setgc",    cmd_setgc);
    add_command("open",    cmd_open);
    add_command("close",    cmd_close);
#endif
    //add_command("i2c2",    cmd_i2c2);
#ifdef HAS_PANTILT
    add_command("pan",     cmd_pan);
    add_command("tilt",    cmd_tilt);
#endif
    //Serial.begin(19200);
    servo_attach();

    #ifdef DRIVE_TRAIN
      Serial.println(format("Using %d motor mode", DRIVE_TRAIN));
    #endif
    /*
     * For backwards compatability with the old protocol, only, which used I2C
     * to control the LEDs.
     *
     * Become an I2C slave.  First change the bitrate to 400khz, which must be
     * set before calling Wire.begin().  Then set the I2C callback functions.
     */
#if defined(I2C_LIGHTS) || defined(I2C_MOTOR)
    TWBR = 12; // TwoWire BitRate
    Wire.begin(I2C_ADDRESS);
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);
#endif
    eyes.begin();

    eyes_show();
#ifdef HAS_GRIPPER
    cmd_open();
#endif

    pinMode(LED_BUILTIN, OUTPUT);

    for (int i = 0; i < 8; i++) {
        digitalWrite(LED_BUILTIN, LOW);
        delay(33);

        digitalWrite(LED_BUILTIN, HIGH);
        delay(33);
    }

#ifdef HAS_PANTILT
    do_command("pan 85");
    do_command("tilt 120");
#endif

#ifdef WANT_BLINK
    cmd_blink();
#endif

}

void loop() {
    library_loop();
}
