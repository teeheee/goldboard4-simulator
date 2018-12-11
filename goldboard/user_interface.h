#ifndef _USER_INTERFACE_H_
#define _USER_INTERFACE_H_

#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "uart.h"

#define SERIAL_BUFFER_SIZE 1024

typedef struct robot_status_t{
  int motor_speed[4];
  int angle;
  int line_sensors[16];
  int ultrasonic_sensors[4];
  uart_t* uart;
  int abort;
} robot_status_t;


void user_interface_init(robot_status_t* robot_status);
void user_interface_poll(robot_status_t* robot_status);
void user_interface_display(robot_status_t* robot_status);

#endif
