#include "Arduino.h"
#include "Encoder_1.h"

    Encoder_1::Encoder_1(uint8_t Enc_pin) {
      encoder_pin = Enc_pin;
      pinMode(encoder_pin, INPUT_PULLUP);
    }

    void Encoder_1::update() {
      ticks += 1;
      //Serial.println(ticks);
    }

    int32_t Encoder_1::read() {
      return ticks;
    }

    void Encoder_1::write(int32_t newval) {
      ticks=newval;
    }


