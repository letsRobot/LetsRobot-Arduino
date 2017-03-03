#include "Arduino.h"
#include "queue.h"

queue::queue() {
  queue_length = QUEUE_LENGTH;
  queue_rear = 0;
  queue_front = 0;
}

char queue::read_cmd() {
  char cmd = command_queue[queue_front] ;
  return cmd;
}

char queue::read_cmd(int8_t specific_location) {
  char cmd = command_queue[specific_location] ;
  return cmd;
}

void queue::write_cmd(char cmd) {
  command_queue[queue_rear] = cmd;
}

int32_t queue::read_value() {
  int32_t value = value_queue[queue_rear] ;
  return value;
}



void queue::write_value(int32_t value) {
  value_queue[queue_rear] = value;
}

void queue::increment_rear() {
  if(queue_rear<queue_length-1){
    queue_rear+=1;
  }
  else{
  queue_rear = 0;
  queue_front = 0;
  Serial.println("overflow");
  }

}

void queue::increment_front() {
  if(queue_front < queue_length -1){
    queue_front +=1;
  }

}
void queue::queue_clear() {

}

uint8_t queue::front()
{
  return queue_front;
}

uint8_t queue::rear()
{
  return queue_rear;
}
