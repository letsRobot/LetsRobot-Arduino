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


//void cmd_n1() {
//  uint8_t value = nextarg_int(0, 180, 50, -1);
//  mySerial.print("m1 ");
//  mySerial.println(value);
//}
//
//void cmd_n2() {
//  uint8_t value = nextarg_int(0, 180, 50, -1);
//  mySerial.print("m2 ");
//  mySerial.println(value);
//}
//
//void cmd_n3() {
//  uint8_t value = nextarg_int(0, 180, 50, -1);
//  mySerial.print("m3 ");
//  mySerial.println(value);
//}
//
//void cmd_n4() {
//  uint8_t value = nextarg_int(0, 180, 50, -1);
//  mySerial.print("m4 ");
//  mySerial.println(value);
//}
//
//void cmd_n5() {
//  uint8_t value = nextarg_int(0, 180, 50, -1);
//  mySerial.print("m5 ");
//  mySerial.println(value);
//}
//
//void cmd_n6() {  
//  uint8_t value = nextarg_int(0, 180, 50, -1);
//  mySerial.print("m6 ");
//  mySerial.println(value);
//}


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

