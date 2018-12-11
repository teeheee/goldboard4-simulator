#include "user_interface.h"

WINDOW *win;
WINDOW *win_up;

void user_interface_init(robot_status_t* robot_status){
	for(int i = 0; i < 16; i++){
		if( i < 4 ){
			robot_status->motor_speed[i] = 0;
			robot_status->ultrasonic_sensors[i] = 0;
		}
		robot_status->line_sensors[i] = 0;
	}
	robot_status->angle = 0;
	robot_status->abort = 0;
	raw();
	noecho();
  initscr();			/* Start curses mode 		*/
  win = newwin(LINES-5, COLS, 5, 0);
	scrollok(win, 1);
  win_up = newwin(5, COLS, 0, 0);
}

void user_interface_display(robot_status_t* robot_status){
	wclear(win_up);
	mvwprintw(win_up,0,0,"motor_speed:");
	for(int i = 0; i < 4; i++){
		mvwprintw(win_up,0,20+10*i,"%d", robot_status->motor_speed[i]);
	}
	mvwprintw(win_up,1,0,"line_sensors:");
	for(int i = 0; i < 16; i++){
		mvwprintw(win_up,1,20+2*i,"%d",robot_status->line_sensors[i]);
	}
	mvwprintw(win_up,2,0,"ultrasonic_sensors:");
	for(int i = 0; i < 4; i++){
		mvwprintw(win_up,2,20+10*i,"%.2f",robot_status->ultrasonic_sensors[i]);
	}
	mvwprintw(win_up,3,0,"kompass:");
	mvwprintw(win_up,3,20,"%d",robot_status->angle);
	for (int j = 0;  j < COLS;  j++)
		mvwaddch(win_up, 4,j,ACS_HLINE);
	wrefresh(win_up);

	if(!uart_is_empty(robot_status->uart))
	{
		char c = uart_getc(robot_status->uart);
		if(c!='\r'){
			waddch(win, c);
			wrefresh(win);
		}
	}
}
