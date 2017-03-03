int32_t slapTime = 1500;
int slapState = 0;
int32_t lastSlapTime = 0;
int32_t slap_queue_timer = 0;
int32_t slap_step = 50;


uint32_t braccio_timer = 0;
uint16_t braccio_delay = 100;

void heartbeat(void) {
uint32_t currentMillis = millis();

if(currentMillis - braccio_timer >=braccio_delay)
{
Braccio.ServoMovement(20,           m1,  m2, m3, m4, m5,  m6);
braccio_timer=currentMillis;
}
  

  
  if (currentMillis - slap_queue_timer >= slap_step)
  {
    if ((bittySlapGo == false && bitslapGo == false ) && (slap_queue.rear() > slap_queue.front()))
    {
      Serial.println("cmd interpet");

      interpret_slap_queue();
 
    }
    slap_queue_timer = currentMillis;
  }

  if (bittySlapGo == true) {
    if (millis() - lastSlapTime >= slapTime) {
      if (slapState == 0) {
        wrist_serv.write(0);
        slapState = 1;
      } else if (slapState == 1) {
        wrist_serv.write(180);
        slapState = 2;
      } else if (slapState == 2) {
        wrist_serv.write(90);
        slapState = 0;
        bittySlapGo = false;
      }
      lastSlapTime = millis();
    }
  }

  if (bitslapGo == true) {
    if (millis() - lastSlapTime >= slapTime) {
      if (slapState == 0) {
        slap_serv.write(0);
        wrist_serv.write(0);
        slapState = 1;
        Serial.println("one");
      } else if (slapState == 1) {
        slap_serv.write(180);
        wrist_serv.write(180);
        slapState = 2;
                Serial.println("two");
      } else if (slapState == 2) {
        slap_serv.write(90);
        wrist_serv.write(90);
        slapState = 0;
        bitslapGo = false;
                Serial.println("three");
      }
      lastSlapTime = millis();
    }
  }
  millis();

}

