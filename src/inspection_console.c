#include "./../include/inspection_utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
int main(int argc, char const *argv[])
{
    // Utility variable to avoid trigger resize event on launch
    int first_resize = TRUE;

    // End-effector coordinates
    float ee_x; 
    float ee_y = 0.0;

    // Initialize User Interface 
    init_console_ui();
    int fd_1, fd_2;
    char* third_fifo = "/tmp/fifo3";
    mkfifo(third_fifo, 0666);
    char* fourth_fifo = "/tmp/fifo4";
    mkfifo(fourth_fifo, 0666);
    float vx = 0.0;
    float vz = 0.0;
    char arr1 [50];
    char arr2[50] = "%f";
    char arr3 [50];
    char arr4[50] = "%f";
    // Infinite loop
    while(TRUE)
	{	
        // Get mouse/resize commands in non-blocking mode...
        int cmd = getch();

        // If user resizes screen, re-draw UI
        if(cmd == KEY_RESIZE) {
            if(first_resize) {
                first_resize = FALSE;
            }
            else {
                reset_console_ui();
            }
        }
        // Else if mouse has been pressed
        else if(cmd == KEY_MOUSE) {

            // Check which button has been pressed...
            if(getmouse(&event) == OK) {

                // STOP button pressed
                if(check_button_pressed(stp_button, &event)) {
                    mvprintw(LINES - 1, 1, "STP button pressed");
                    refresh();
                    sleep(1);
                    for(int j = 0; j < COLS; j++) {
                        mvaddch(LINES - 1, j, ' ');
                    }
                }

                // RESET button pressed
                else if(check_button_pressed(rst_button, &event)) {
                    mvprintw(LINES - 1, 1, "RST button pressed");
                    refresh();
                    sleep(1);
                    for(int j = 0; j < COLS; j++) {
                        mvaddch(LINES - 1, j, ' ');
                    }
                }
            }
        }
       /* fd_1 = open(third_fifo, O_RDONLY);
        read(fd_1, arr1, 50);
        sscanf(arr1, arr2 , &ee_x);
        close(fd_1);*/

        fd_2 = open(fourth_fifo, O_RDONLY);
        read(fd_2, arr3, 50);
        sscanf(arr3, arr4 , &ee_y);
        close(fd_2);
        
        // Update UI
        update_console_ui(&ee_x, &ee_y);
	}

    // Terminate
    endwin();
    return 0;
}
