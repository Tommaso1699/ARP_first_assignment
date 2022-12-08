#include "./../include/command_utilities.h"
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
    float vx = 0.0 ;
    float vz = 0.0;
    int fd_1, fd_2;
    char* first_fifo = "/tmp/fifo1";
    char* second_fifo = "/tmp/fifo2";
    mkfifo(first_fifo, 0666);
    mkfifo(second_fifo, 0666);
    char arr1 [50];

    char arr2 [50];
    // Utility variable to avoid trigger resize event on launch
    int first_resize = TRUE;

    // Initialize User Interface 
    init_console_ui();
    
    // Infinite loop
    while(TRUE)
	{	
        /*fd_1 = open(first_fifo, O_WRONLY);
            fflush(stdout);
            sprintf(arr1, "%f", vx );
            write(fd_1,arr1,strlen(arr1 +1));
            close(fd_1);
            usleep(50000);*/
            fd_2 = open(second_fifo, O_WRONLY);
            fflush(stdout);
            sprintf(arr2, "%f", vz );
            write(fd_2,arr2,strlen(arr2 +1));
            close(fd_2);
            usleep(50000);
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

                // Vx++ button pressed
                if(check_button_pressed(vx_incr_btn, &event)) {
                    vx = vx  + 0.1;
                    usleep(5000);
                }

                // Vx-- button pressed
                else if(check_button_pressed(vx_decr_btn, &event)) {
                  vx = vx  - 0.1;
                  usleep(5000);
                }

                // Vx stop button pressed
                else if(check_button_pressed(vx_stp_button, &event)) {
                vx = 0.0;
                usleep(5000);
                }

                // Vz++ button pressed
                else if(check_button_pressed(vz_incr_btn, &event)) {
                   vz = vz  + 0.1;
                   usleep(5000);
                    
                }

                // Vz-- button pressed
                else if(check_button_pressed(vz_decr_btn, &event)) {
                    vz = vz  - 0.1;
                    usleep(5000);
                   
                }

                // Vz stop button pressed
                else if(check_button_pressed(vz_stp_button, &event)) {
                    vz = 0.0;
                    usleep(5000);
                    
                }
            }
        }

        refresh();
	}
    // Terminate
    endwin();
    return 0;
}
