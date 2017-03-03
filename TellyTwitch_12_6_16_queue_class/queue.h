// Header file for class for creating queues 

//Information
/* More Information
 */

#ifndef queue_h
#define queue_h

#include "queue.h"

#define QUEUE_LENGTH 20

class queue 
{
 private:
    int8_t  queue_length;
    char    command_queue[QUEUE_LENGTH];
    int32_t value_queue[QUEUE_LENGTH];
    uint8_t queue_front;
    uint8_t queue_rear;

  public:
    queue();

   int32_t read_value();
   void write_value(int32_t value);
   char read_cmd();
   char read_cmd(int8_t specific_location);
   void write_cmd(char cmd);
   void increment_rear();
   void increment_front();
   void queue_clear();
   uint8_t front();
   uint8_t rear();
   
   
};

#endif

