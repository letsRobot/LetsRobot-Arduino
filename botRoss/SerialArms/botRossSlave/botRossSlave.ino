#include <Servo.h>
#include <Braccio.h>
#include "queue.h"

#include "library.h"
#include "config.h"


Servo slap_serv;
Servo wrist_serv;

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;

queue slap_queue;


bool bitslapGo = false;
bool bittySlapGo = false;

void cmd_bittyslap_queue(void){
  slap_queue.write_cmd('t');
  slap_queue.increment_rear();
}

void cmd_bitslap_queue(void){
  slap_queue.write_cmd('s');
  slap_queue.increment_rear();
}

void cmd_bittyslap () {
  bittySlapGo = true;
}

void cmd_bitslap () {
  bitslapGo = true;
}

void cmd_tinyslap() {
  uint8_t value = nextarg_int(0, 180, 50, -1);
  wrist_serv.write(value);
}

void cmd_doot()
{
  uint8_t value = nextarg_int(0, 180, 50, -1);
  slap_serv.write(value);
}

int m1 = 90;
int m2 = 90;
int m3 = 90;
int m4 = 90;
int m5 = 90;
int m6 = 90;


void cmd_m1() {
  uint8_t value = nextarg_int(0, 180, 50, -1);
  m1 = value;
}

void cmd_m2() {
  uint8_t value = nextarg_int(15, 165, 50, -1);
  m2 = value;
}

void cmd_m3() {
  uint8_t value = nextarg_int(0, 180, 50, -1);
  m3 = value;
}

void cmd_m4() {
  uint8_t value = nextarg_int(0, 180, 50, -1);
  m4 = value;
}

void cmd_m5() {
  uint8_t value = nextarg_int(0, 180, 50, -1);
  m5 = value;
}

void cmd_m6() {
  uint8_t value = nextarg_int(10, 73, 50, -1);
  m6 = value;
}

void setup() {
  // put your setup code here, to run once:
  Braccio.begin();
  //slap_serv.attach(7);
  //wrist_serv.attach(8);
  //slap_serv.write(90);
  //wrist_serv.write(90);
  //add_command("slap",  cmd_doot);
  //add_command("tinyslap",  cmd_tinyslap);
  //add_command("bitslap", cmd_bitslap_queue);
  //add_command("bittyslap", cmd_bittyslap_queue);

  add_command("m1", cmd_m1);
  add_command("m2", cmd_m2);
  add_command("m3", cmd_m3);
  add_command("m4", cmd_m4);
  add_command("m5", cmd_m5);
  add_command("m6", cmd_m6);

  

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  library_loop();
  //delay(1); 
  
}

void interpret_slap_queue(void){
  if ((slap_queue.rear() > slap_queue.front()))
{
char slpcmd = slap_queue.read_cmd();
  switch (slpcmd) {
    case 't':
    cmd_bittyslap();
      break;
    case 's':
    cmd_bitslap();
    break;

  }
    slap_queue.increment_front();
}
}
