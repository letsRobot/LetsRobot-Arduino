#include <Servo.h>
#include <Braccio.h>
#include <SoftwareSerial.h>
#include "queue.h"

#include "library.h"
#include "config.h"

//SoftwareSerial mySerial(7, 8); // RX, TX

Servo slap_serv;
Servo wrist_serv;

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;

queue slap_queue;

int m1 = 90;
int m2 = 90;
int m3 = 90;
int m4 = 90;
int m5 = 90;
int m6 = 90;

bool bitslapGo = false;
bool bittySlapGo = false;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("test");
//  mySerial.begin(9600);
  //mySerial.println("Softtest");
  Braccio.begin();
  //slap_serv.attach(7);
  //wrist_serv.attach(8);
  //slap_serv.write(90);
  //wrist_serv.write(90);
  //add_command("slap",  cmd_doot);
  //add_command("tinyslap",  cmd_tinyslap);
  //add_command("bitslap", cmd_bitslap_queue);
  //add_command("bittyslap", cmd_bittyslap_queue);

  add_command("base", cmd_m1); //Rotates base of left arm
  add_command("shoulder", cmd_m2); //Rotates shoulder up and down
  add_command("elbow", cmd_m3); //rotates elbow up and down
  add_command("wrist1", cmd_m4); //rotates wrist joint up and down
  add_command("wrist2", cmd_m5); //rotates wrist joint left and right
  add_command("grip", cmd_m6); //open and close gripper
//  add_command("/rbase", cmd_n1);
//  add_command("/rshoulder", cmd_n2);
//  add_command("/relbow", cmd_n3);
//  add_command("/rwrist1", cmd_n4);
//  add_command("/rwrist2", cmd_n5);
//  add_command("/rgrip", cmd_n6);

  
  //Serial.println("test2");
  


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
