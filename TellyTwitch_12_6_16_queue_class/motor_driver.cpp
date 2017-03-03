#include "Arduino.h"
#include "motor_driver.h"

// Sets up pins for controlling motor driver and saves encoder pointer
motor_driver::motor_driver(uint8_t Dir_pin_1, uint8_t Dir_pin_2, uint8_t PWM_con_pin, Encoder_1* encoder) {
  IN1_pin = Dir_pin_1;
  IN2_pin = Dir_pin_2;
  PWM_pin = PWM_con_pin;
  pinMode(PWM_pin, OUTPUT);
  pinMode(IN1_pin, OUTPUT);
  pinMode(IN2_pin, OUTPUT);
  analogWrite(PWM_pin, 0);
  last_position = 0;
  pos_control = true;
  p_encoder = encoder;
  stall_cnt = 0;
}
// Sets up default gains and speed
void motor_driver::default_setup(int32_t default_speed, uint8_t proportional_gain, uint8_t integral_gain, uint32_t easing_value, uint8_t tstep)
{
  v_setpoint_default = default_speed;
  v_setpoint = v_setpoint_default;
  KP = proportional_gain;
  KI = integral_gain;
  ease = easing_value;
  time_step = tstep;
  control_freq = 1000 / tstep;
  stall_cnt = 0;
}

// Initializes DC_motor control and makes sure velocity is in right direction
void motor_driver::initiate_DC_control() {

  if (desired_position < 0)
  {
    v_setpoint = -1 * v_setpoint_default;
  }
  if (pos_done == true) {
    p_encoder -> write(0);
    //Serial.println("posdoodo");
  }
  pos_done = false;
  //Serial.println(v_setpoint);
}

void motor_driver::DC_control_loop() {
  int32_t new_position = p_encoder->read();
  //Serial.println(new_position);
  if (v_setpoint < 0) {
    new_position = -new_position;
  }
  velocity = new_position - last_position;
//  Serial.print("v: ");
//  Serial.println(velocity);
  last_position = new_position;
  error = v_setpoint - velocity;
  integral_error = integral_error + error;
  




//  if (((desired_position > 0 && desired_position > 400) || (desired_position < 0 && desired_position < -400))&& (abs(v_setpoint_default)>=50))
//  {
//    if(new_position > 0 && new_position > desired_position - 400) 
//    {
//      v_setpoint = (v_setpoint-25)* (desired_position - new_position) /500 + 500/control_freq;
//      //Serial.println(v_setpoint);
//    }
//  }



  if (stall_cnt >= control_freq/5)
  {
    pos_done = true;
    analogWrite(PWM_pin, 0);
    Serial.println("stall");
  }

  else if ((desired_position > 0 && desired_position > new_position) || (desired_position < 0 && desired_position < new_position))
  {
    PWM_signal = ((v_setpoint - velocity) * KP + integral_error * KI ) / control_freq;
//    Serial.print("PWM: ");
//    Serial.println(PWM_signal);
    if (PWM_signal < 0)
    {
      digitalWrite(IN1_pin, LOW);
      digitalWrite(IN2_pin, HIGH);
      PWM_signal = -1 * PWM_signal;
    }
    else
    {
      digitalWrite(IN1_pin, HIGH);
      digitalWrite(IN2_pin, LOW);
    }
    if (PWM_signal >= 255)
    {
      PWM_signal = 255;
    }
    //Serial.println(PWM_signal);
    analogWrite(PWM_pin, PWM_signal);
  }
  else {
    pos_done = true;
    analogWrite(PWM_pin, 0 );
    //Serial.println(new_position);
  }
  if ((velocity >= 0 && velocity <= 100 / control_freq) || (velocity <= 0 && velocity >= -100 / control_freq))
  {
    //stall_cnt += 1;
  }
  else
  {
    stall_cnt = 0;
  }

}

bool motor_driver::finish_checker() {
  return pos_done;
}

void motor_driver::stop_motion(){
    pos_done = true;
    analogWrite(PWM_pin, 0 );
     int32_t new_position = p_encoder->read();
    //Serial.println(new_position);
}

void motor_driver::update_position(int32_t distance_added) {

  desired_position += distance_added;
}

void motor_driver::clean_DC_control_variables() {
  integral_error = 0;
  stall_cnt = 0;
  pos_done = true;
  last_position = 0;
  v_setpoint = v_setpoint_default;
  desired_position = 0;
  p_encoder -> write(0);
  pos_control = false;
  velocity=0;
  error = 0;
}

int32_t motor_driver::posit() {
  pos = p_encoder -> read();
  return pos;
}

int32_t motor_driver::velocity_check(void){
  return velocity;
}



