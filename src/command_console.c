#include "./../include/command_utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<signal.h>
#include <time.h>

float number_x =0.0;
float number_z =0.0;

void stop(int signo){
    if(signo==SIGUSR1){
       number_x = 3.0;
       number_z = 3.0;
    }
}

void reset(int signo){
    if(signo==SIGUSR2){
       
       number_x = 2.0;
       number_z = 2.0;
    }
}

int logging(char* log)
{
    char array[200];
    char *inspection = "/tmp/inspection";
    int fd_log = open(inspection, O_WRONLY);
    memset(array, 0, sizeof(array));
    sprintf(array, "%ld; %d; %s\n", time(NULL), getpid(), log);
    write(fd_log, array, strlen(array));
    close(fd_log);
}

int main(int argc, char const *argv[])
{
    FILE *command_console; // creating variable
    // command_console = fopen("command_console.log", "w"); //creating log file
    // fprintf(command_console,"PID of process: %d\n", getpid()); //writing to log file
    // fflush(command_console); //write all of the buffered data to it's destination
    // fclose(command_console); //closing log file
    int motor_x, motor_z;
    char *motor_x_fifo = "/tmp/fifo_motor_x";
    char *motor_z_fifo = "/tmp/fifo_motor_z";
    mkfifo(motor_x_fifo, 0666);
    mkfifo(motor_z_fifo, 0666);
    char arr1[50];
    char arr2[50];
    // Utility variable to avoid trigger resize event on launch
    int first_resize = TRUE;

    // Initialize User Interface
    init_console_ui();
    if (signal(SIGUSR2, reset) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
        if (signal(SIGUSR1, stop) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    // Infinite loop
    while (TRUE)
    {
        motor_x = open(motor_x_fifo, O_WRONLY);
            fflush(stdout);
            sprintf(arr1, "%f", number_x );
            write(motor_x,arr1,strlen(arr1 +1));
            close(motor_x);
            usleep(20000);
            motor_z = open(motor_z_fifo, O_WRONLY);
            fflush(stdout);
            sprintf(arr2, "%f", number_z );
            write(motor_z,arr2,strlen(arr2 +1));
            close(motor_z);
            usleep(20000);
        // Get mouse/resize commands in non-blocking mode...
        int cmd = getch();
        // If user resizes screen, re-draw UI
        if (cmd == KEY_RESIZE)
        {
            if (first_resize)
            {
                first_resize = FALSE;
            }
            else
            {
                reset_console_ui();
            }
        }
        // Else if mouse has been pressed
        else if (cmd == KEY_MOUSE)
        {

            // Check which button has been pressed...
            if (getmouse(&event) == OK)
            {
                // Vx++ button pressed
                if(check_button_pressed(vx_incr_btn, &event)) {
                    
                   number_x = 1.0;
                   // fprintf(command_console, "Vx increase button pressed\n");
                   // fflush(command_console);
                    //fclose(command_console);
                    
                }

                // Vx-- button pressed
                else if(check_button_pressed(vx_decr_btn, &event)) {
                  number_x = 2.0;
                  //fprintf(command_console, "Vx decrease button pressed\n");
                  //fflush(command_console);
                  //fclose(command_console);
                  
                }

                // Vx stop button pressed
                else if(check_button_pressed(vx_stp_button, &event)) {
               number_x = 3.0;
               // fprintf(command_console, "Vx stop button pressed\n");
               // fflush(command_console);
                //fclose(command_console);
                }

                // Vz++ button pressed
                else if(check_button_pressed(vz_incr_btn, &event)) {
                
                   number_z = 1.0;
                   //fprintf(command_console, "Vz increase button pressed\n");
                   //fflush(command_console);
                  // fclose(command_console);
                    
                }

                // Vz-- button pressed
                else if(check_button_pressed(vz_decr_btn, &event)) {
                
                    number_z = 2.0;
                    //fprintf(command_console, "Vz decrease button pressed\n");
                   // fflush(command_console);
                   // fclose(command_console);
                    
                   
                }

                // Vz stop button pressed
                else if(check_button_pressed(vz_stp_button, &event)) {
                    number_z = 3.0;
                   // fprintf(command_console, "Vz stop button pressed\n");
                    //fflush(command_console);
                   // fclose(command_console);
                    
                    
                }
            }
        }

        refresh();
    }
    // Terminate
    logging(" command ");
    endwin();
    return 0;
}