uint16_t movestep = 50;
uint32_t move_queue_check_timer = 0;
uint32_t vel_spit_timer = 0;
uint32_t vel_freq = 50;
uint32_t forward_timer =0;

void heartbeat(void)
{


  currentMillis = millis();

//  if ((currentMillis - forward_timer >= 2000))
//  {
//    do_command("f");
//    forward_timer = currentMillis;
//
//  }
  
  if ((currentMillis - vel_spit_timer >= vel_freq) && motion_done == false)
  {
    Serial.print("MtrA: ");
    Serial.println(Motor_A.velocity_check());
    Serial.print("MtrB: ");
    Serial.println(Motor_B.velocity_check());
    vel_spit_timer = currentMillis;
  }

  
  if (0 < stop_time && stop_time < millis())
  {
    stop();
  }

  if (currentMillis - move_queue_check_timer >= movestep)
  {
    if ((motion_done == true) && (movequeue.rear() > movequeue.front()))
    {
      //Serial.println("cmd interpet");

      interpret_movement_queue();

    }
    if ( DC_control == true)
    {
      if ((movequeue.rear() != QUEUE_LENGTH - 1) && (movequeue.rear() > movequeue.front()))
      {
        if (movequeue.read_cmd(movequeue.front() - 1) == movequeue.read_cmd())
        {
          //Serial.println("duplicate");
          interpret_movement_queue();
        }
      }
    }
  }


  if (DC_control == true)
  {
    if (currentMillis - prevMillis >= tstep)
    {
      prevMillis = currentMillis;
            if (Motor_A.finish_checker() == false||Motor_B.finish_checker()==false)
      {
        Motor_A.DC_control_loop();
      }
      if ( Motor_B.finish_checker() == false||Motor_B.finish_checker()==false)
      {
        Motor_B.DC_control_loop();
      }

      if (Motor_A.finish_checker() == true || Motor_B.finish_checker() == true)
      {
        DC_control = false;
        //Serial.println("A: ");
        Motor_A.stop_motion();
        //Serial.println("B: ");
        Motor_B.stop_motion();
        Motor_A.clean_DC_control_variables();
        Motor_B.clean_DC_control_variables();
        //Serial.println("cleaned");
        motion_done = true;
      }
    }
  }
}
