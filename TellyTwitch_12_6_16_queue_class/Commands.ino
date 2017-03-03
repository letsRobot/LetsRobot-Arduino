void Pos_DC_motor_loop ()
{
  DC_control = true;
  Motor_A.initiate_DC_control();
  Motor_B.initiate_DC_control();
}

void cmd_doot(void)
{
  uint8_t angle = nextarg_int(0,180,0,-1);
  gripper_servo.write(angle);

}

void cmd_brightness(void)
{
  uint8_t value = nextarg_int(0, 100, 50, -1);
  eyes.setBrightness(value);
  eyes.show();
}


void cmd_f()
{
    cmd_forward();
}

void cmd_r()
{
    cmd_right();
}

void cmd_l()
{
    cmd_left();
}

void cmd_b()
{
    cmd_back();
}

void cmd_grip_strength (void)
{
  grip_strength = nextarg_int(0, 255, 0, -1);
}




void cmd_forward (void)
{
  movequeue.write_cmd('f');
  movequeue.increment_rear();

}


void cmd_back    (void)
{
  movequeue.write_cmd('b');
  movequeue.increment_rear();
}


void cmd_left    (void)
{
  movequeue.write_cmd('l');
  movequeue.increment_rear();
}


void cmd_right   (void)
{
  movequeue.write_cmd('r');
  movequeue.increment_rear();
}


void cmd_stop(void) {
  if (stop_time)
    schedule_stop(1);
  heartbeat();
  OK();
}




//void cmd_setgc(void) {
//  int pos = nextarg_int(-360, 360, 0, -1);
//  //Serial.println(format("Gripper close position set: %d", pos));
//  gripper_close = pos;
//  OK();
//}

void cmd_open(void) {
gripper_servo.write(GRIPPER_OPEN);

}

void cmd_close(void) {
gripper_servo.write(GRIPPER_CLOSED);
}

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
     The human-readable pixel index is 1 based, so the valid range
     is [1, NUM_LEDS], not [0, NUM_LEDS-1].  After input validation,
     subtract one from index to use a stanard 0-based array.
  */
  if (! (1 <= pixel && pixel <= NUM_LEDS))
    return ERR("LED index out of range");

  pixel -= 1;

  eyes.setPixelColor(led_map[pixel], red, green, blue);
  //servo_detach();
  eyes_show();
  //servo_attach();
  OK();
}

void cmd_blink() {
  blink_open();   delay(350);
  blink_close(); delay(100);

  blink_open();   delay(350);
  blink_close(); delay(100);

  blink_open();
}

void cmd_ticks(void) {
  int32_t value = nextarg_int(-50000, 50000, 0, -1);
  movequeue.write_cmd('t');
  movequeue.write_value(value);
  movequeue.increment_rear();
}

void cmd_l_ticks(void) {
  int32_t value = nextarg_int(-180, 180, 0, -1);
  Serial.print("angle");
  Serial.println(value);
  value = deg_360/4 * value / 90;
  Serial.print("lticks");
  Serial.println(value);
  movequeue.write_cmd('a');
  movequeue.write_value(value);
  movequeue.increment_rear();
}

void cmd_r_ticks(void) {
  int32_t value = nextarg_int(-180, 180, 0, -1);
  Serial.print("angle");
  Serial.println(value);
  value = deg_360/4 * -value / 90;
  Serial.print("lticks");
  Serial.println(value);
  movequeue.write_cmd('a');
  movequeue.write_value(value);
  movequeue.increment_rear();
}

void cmd_setv(void) {
  int32_t value = nextarg_int(700, 1800, 1500, -1);
setpointAdefault = value / cRate;
setpointBdefault = value / cRate;

}

void cmd_grab(void)
{
  cmd_open();
  delay(200);
  do_command("pan 150");
  delay(1000);
  cmd_close();
  delay(400);
  do_command("pan 30");
  delay(200);
  cmd_open();


}
void cmd_pos(void)
{
  int32_t lastposA = EncA.read();
  int32_t lastposB = EncB.read();
  Serial.println("A Pos, B pos");
  Serial.println(lastposA);
  Serial.println(lastposB);
}



