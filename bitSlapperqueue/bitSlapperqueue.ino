#include <Servo.h>
#include "queue.h"

#include "library.h"
#include "config.h"


Servo slap_serv;
Servo wrist_serv;

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

void setup() {
  // put your setup code here, to run once:
  slap_serv.attach(7);
  wrist_serv.attach(8);
  slap_serv.write(90);
  wrist_serv.write(90);
  add_command("slap",  cmd_doot);
  add_command("tinyslap",  cmd_tinyslap);
  add_command("bitslap", cmd_bitslap_queue);
  add_command("bittyslap", cmd_bittyslap_queue);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  library_loop();
}

void interpret_slap_queue(void){
  //Serial.println(slap_queue.rear());
  //Serial.println(slap_queue.front());
  if ((slap_queue.rear() > slap_queue.front()))
  {
    char slpcmd = slap_queue.read_cmd();
    switch (slpcmd) {
      case 't':
        cmd_bittyslap();
        break;
      case 's':
        //Serial.println("in s");
        cmd_bitslap();
        break;
    }
    slap_queue.increment_front();
  }
}
