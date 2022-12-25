#include "./../include/inspection_utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<signal.h>
float ee_x = 0.0; //initializing variable
float ee_y = 0.0; //initializing variable
void Stop_button(int sig)
{
    if (sig == SIGUSR1)
    {
        char arr1[10]; //creating array
        char arr2[10]; //creating array
        char arr3[10]; //creating array
        FILE *pid1 = popen("pidof -s motor_x", "r"); //pipe stream to a process
        FILE *pid2 = popen("pidof -s motor_z", "r"); //pipe stream to a process
        FILE *pid3 = popen("pidof -s command", "r"); //pipe stream to a process
        fgets(arr1, 10, pid1); //read a limited number of characters from a given file stream source into an array of characters
        fgets(arr2, 10, pid2); //read a limited number of characters from a given file stream source into an array of characters
        fgets(arr3, 10, pid3); //read a limited number of characters from a given file stream source into an array of characters
        long motor_x_pid = strtoul(arr1, NULL, 10); //convert a string to an unsigned long integer
        long motor_z_pid = strtoul(arr2, NULL, 10); //convert a string to an unsigned long integer
        long command_pid = strtoul(arr3, NULL, 10); //convert a string to an unsigned long integer
        kill(motor_x_pid, SIGUSR1); //killing process
        kill(motor_z_pid, SIGUSR1); //killing process
        kill(command_pid, SIGUSR1); //killing process
        
        
    }
}
void Reset_button(int sig)
{
    if (sig == SIGUSR2)
    {
        char arr1[10]; //creating array
        char arr2[10]; //creating array
        char arr3[10]; //creating array
        FILE *pid1 = popen("pidof -s motor_x", "r"); //pipe stream to a process
        FILE *pid2 = popen("pidof -s motor_z", "r"); //pipe stream to a process
        FILE *pid3 = popen("pidof -s command", "r"); //pipe stream to a process
        fgets(arr1, 10, pid1); //read a limited number of characters from a given file stream source into an array of characters
        fgets(arr2, 10, pid2); //read a limited number of characters from a given file stream source into an array of characters
        fgets(arr3, 10, pid3); //read a limited number of characters from a given file stream source into an array of characters
        long motor_x_pid = strtoul(arr1, NULL, 10); //convert a string to an unsigned long integer
        long motor_z_pid = strtoul(arr2, NULL, 10); //convert a string to an unsigned long integer
        long command_pid = strtoul(arr3, NULL, 10); //convert a string to an unsigned long integer
        kill(motor_x_pid, SIGUSR2); //killing process
        kill(motor_z_pid, SIGUSR2); //killing process
        kill(command_pid, SIGUSR2); //killing process
    }
}
int main(int argc, char const *argv[])
{
    
    int first_resize = TRUE; // Utility variable to avoid trigger resize event on launch

    init_console_ui(); // Initialize User Interface
    int updateui; //creating variable
    char *update_ui = "/tmp/update_ui"; //initializing variable
    mkfifo(update_ui,0666); // make a FIFO special file (a named pipe)
    char arr1[50]; //creating array
    char arr2[50] = "%f,%f"; //creating array
    int motor_x, motor_z; //creating variables
    char* motor_x_fifo = "/tmp/fifo_motor_x"; //initializing variable
    char* motor_z_fifo = "/tmp/fifo_motor_z"; //initializing variabl
    mkfifo(motor_x_fifo, 0666); // make a FIFO special file (a named pipe)
    mkfifo(motor_z_fifo, 0666); // make a FIFO special file (a named pipe)
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
                 if (signal(SIGUSR1, Stop_button) == SIG_ERR)
                        printf("\nCannot catch SIGUSR1\n");
                    kill(getpid(), SIGUSR1);
                }

                // RESET button pressed
                else if(check_button_pressed(rst_button, &event)) {
                       if (signal(SIGUSR2, Reset_button) == SIG_ERR)
                        printf("\nCannot catch SIGUSR2\n");
                    kill(getpid(), SIGUSR2);
                    
                }
            }
        }
        updateui = open(update_ui, O_RDONLY); // opening FIFO
        read(updateui, arr1 , 50); // read to arr1 from updateui
        sscanf(arr1, arr2 ,&ee_x, &ee_y); //read formatted input
        close(updateui); //closing FIFO

        
        // Update UI
        update_console_ui(&ee_x, &ee_y);
	}

    // Terminate
    endwin();
    return 0;
}