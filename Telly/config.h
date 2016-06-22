// Are we running on a teensy, or an Uno?
#define TEENSY
//#define FRANKENTELLY
#define TELLY3

// telly1 25 10 10
//#define EYE_BLINK_COLOR "25 10 10"
// telly2 25 0 25
// tellydev 0 25 0
// telly3 0 0 25
#define EYE_BLINK_COLOR "0 25 25"

// Hardware description
//#define HAS_GRIPPER
#define HAS_PANTILT

// Only define one of these
#define HAS_DCMOTOR
//#define HAS_SERVO
//#define DRIVE_TRAIN DCMOTOR_EXIST

// spend memory on older features
#define I2C_LIGHTS
//#define I2C_MOTOR

#define WANT_SERIAL
// (647-480) = 167 bytes
#define WANT_BLINK
// (470-262) = 188 bytes
//#define WANT_MTIMINGS 


