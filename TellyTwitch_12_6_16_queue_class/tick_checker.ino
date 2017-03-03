void interpret_movement_queue()
{
    if ((movequeue.rear() > movequeue.front()))
  {
    char mcmd = movequeue.read_cmd();
    Serial.println(mcmd);

    switch (mcmd) {
      case 'f':
        Motor_A.update_position(ONE_REVOLUTION);  // Desired number of ticks for the motor to travel (1 rev = 3576)
        Motor_B.update_position(ONE_REVOLUTION);  // Desired number of ticks for the motor to travel (1 rev = 3576)
        //Serial.println("movef");
        Pos_DC_motor_loop();
        break;
      case 'b':

        Motor_A.update_position(-ONE_REVOLUTION);  // Desired number of ticks for the motor to travel (1 rev = 3576)
        Motor_B.update_position(-ONE_REVOLUTION); // Desired number of ticks for the motor to travel (1 rev = 3576)
        //Serial.println("moveb");
        Pos_DC_motor_loop();
        break;
      case 'l':

        Motor_A.update_position( -deg_360 / 8); // Desired number of ticks for the motor to travel (1 rev = 3576)
        Motor_B.update_position( deg_360 / 8); // Desired number of ticks for the motor to travel (1 rev = 3576)
        //Serial.println("movel");
        Pos_DC_motor_loop();
        break;
      case 'r':

        Motor_A.update_position( deg_360 / 8);  // Desired number of ticks for the motor to travel (1 rev = 3576)
        Motor_B.update_position( -deg_360 / 8);  // Desired number of ticks for the motor to travel (1 rev = 3576)
        //Serial.println("mover");
        Pos_DC_motor_loop();
        break;
      case 't':

        Motor_A.update_position(-movequeue.read_value());  // Desired number of ticks for the motor to travel (1 rev = 3576)
        Motor_B.update_position(-movequeue.read_value());  // Desired number of ticks for the motor to travel (1 rev = 3576)
        //Serial.print("ticks");
        Pos_DC_motor_loop();
        break;
      case 'a':

        Motor_A.update_position(-movequeue.read_value()); // Desired number of ticks for the motor to travel (1 rev = 3576)
        Motor_B.update_position( movequeue.read_value()); // Desired number of ticks for the motor to travel (1 rev = 3576)
        //Serial.println("angle");
        Pos_DC_motor_loop();
        break;
    }
    motion_done = false;
    movequeue.increment_front();

  }
}


