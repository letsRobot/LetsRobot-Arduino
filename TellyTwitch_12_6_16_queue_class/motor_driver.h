// Header file for class for reading a single encoder. 


// Header file for class for driving a single motor with encoder feedback
// Motor driver object must be made and setup function must be called
//EXAMPLE
/*  motor_driver Motor_A(INA1pin,INA2pin,PWMApin,EncA_pt);
 *  Motor_A.default_setup(setpointAdefault,KP,KI,ease,tstep);
*/

#ifndef motor_driver_h
#define motor_driver_h

#include "Arduino.h"
#include "Encoder_1.h"

class motor_driver
{
  private:

   
    uint16_t time_step;
    uint16_t control_freq;
    uint8_t KP;
    uint8_t KI;
    uint32_t ease;
    uint8_t stall_cnt;

    int32_t velocity = 0;

    uint8_t IN1_pin;
    uint8_t IN2_pin;
    uint8_t PWM_pin;

    int32_t pos;

    bool pos_done;
    bool pos_control;
    Encoder_1* p_encoder;
        int32_t v_setpoint;
    int32_t v_setpoint_default;
    int32_t desired_position;
    int32_t last_position;
    int32_t PWM_signal;
    int32_t error;
    int32_t integral_error;
    

  public:


   
    
    motor_driver(uint8_t Dir_pin_1, uint8_t Dir_pin_2, uint8_t PWM_con_pin, Encoder_1* encoder);
    
    void default_setup(int32_t default_speed, uint8_t proportional_gain, uint8_t integral_gain, uint32_t easing_value,uint8_t tstep);
 
    void initiate_DC_control();

    void DC_control_loop();

    void clean_DC_control_variables();

    bool finish_checker();

    void update_position(int32_t distance_added);
    
    int32_t posit();

    void stop_motion();

    int32_t velocity_check();
};

#endif

