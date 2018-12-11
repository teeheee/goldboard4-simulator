#ifndef _USER_INTERFACE_H_
#define _USER_INTERFACE_H_

#include <ncurses.h>
#include <string.h>

#define SERIAL_BUFFER_SIZE 1024

typedef struct robot_status_t{
  int motor_speed[4];
  int angle;
  int line_sensors[16];
  int ultrasonic_sensors[4];
  FILE* serial_output_stream;
  uint8_t buffer[SERIAL_BUFFER_SIZE];
  int abort;
} robot_status_t;


void user_interface_init(robot_status_t* robot_status);
void user_interface_poll(robot_status_t* robot_status);
void user_interface_display(robot_status_t* robot_status);

#endif
