// Header file for class for reading a single encoder. 
// Encoder object must be made and given the pin to use ie 
/* Encoder_1 EncA(ENC1pin);
*/
// You must create a function that is called whenever interrupts are triggered 
/*
 * void EncAupdate(){
 * EncA.update();}
 */
// Additionally you must attach the pin to trigger interrupts in setup
/* attachInterrupt(ENC1pin, EncAupdate, CHANGE);
 */

#ifndef Encoder_1_h
#define Encoder_1_h

#include "Arduino.h"

class Encoder_1 
{
  private:
    int32_t ticks;
    uint8_t encoder_pin;

  public:
    Encoder_1(uint8_t Enc_pin);

    void update();

    int32_t read();

    void write(int32_t newval);
};

#endif

