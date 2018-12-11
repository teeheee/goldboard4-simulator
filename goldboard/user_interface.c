#include "user_interface.h"


void user_interface_init(robot_status_t* robot_status){
	robot_status->serial_output_stream = fmemopen(robot_status->buffer, SERIAL_BUFFER_SIZE, "rw");
	for(int i = 0; i < 16; i++){
		if( i < 4 ){
			robot_status->motor_speed[i] = 0;
			robot_status->ultrasonic_sensors[i] = 0;
		}
		robot_status->line_sensors[i] = 0;
	}
	robot_status->angle = 0;
	robot_status->abort = 0;
	memset(robot_status->buffer, 0, SERIAL_BUFFER_SIZE);
	initscr();			/* Start curses mode 		*/
	raw();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();
}


char buff[255];
void user_interface_display(robot_status_t* robot_status){
	printw("Type any character to see it in bold\n");
	int ch = getch();			/* If raw() hadn't been called
					 * we have to press enter before it
					 * gets to the program 		*/
	if(ch == KEY_F(1)){		/* Without keypad enabled this will */
		printw("F1 Key pressed");/*  not get to us either	*/
					/* Without noecho() some ugly escape
					 * charachters might have been printed
					 * on screen			*/
		robot_status->abort = 1;
	}
	else
	{	
		FILE* stream = robot_status->serial_output_stream;
		printw(fgets(buff, 255, stream));
	}
	refresh();			/* Print it on to the real screen */
}
