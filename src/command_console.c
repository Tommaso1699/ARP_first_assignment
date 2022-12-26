#include "./../include/command_utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>

float number_x = 0.0; // initializing variable
float number_z = 0.0; // initializing variable
void Stop_button(int sig)
{ // function that is activating after receiving signal
    if (sig == SIGUSR1)
    {
        number_x = 3.0;
        number_z = 3.0;
    }
}
void Reset_button(int sig)
{ // function that is activating after receiving signal
    if (sig == SIGUSR2)
    {
        number_x = 2.0;
        number_z = 2.0;
    }
}

int logging(char *log)
{
    char array[200];
    char *inspection = "/tmp/inspection";
    int fd_log = open(inspection, O_RDWR);
    memset(array, 0, sizeof(array));
    sprintf(array, "%ld;%s;%s", time(NULL), "command", log);
    write(fd_log, array, strlen(array) + 1);
    close(fd_log);
}

int main(int argc, char const *argv[])
{
    FILE *command_console; // creating variable
    // command_console = fopen("command_console.log", "w"); //creating log file
    // fprintf(command_console,"PID of process: %d\n", getpid()); //writing to log file
    // fflush(command_console); //write all of the buffered data to it's destination
    // fclose(command_console); //closing log file
    int motor_x, motor_z;                     // creating variable
    char *motor_x_fifo = "/tmp/fifo_motor_x"; // creating variable
    char *motor_z_fifo = "/tmp/fifo_motor_z"; // creating variable
    mkfifo(motor_x_fifo, 0666);               // make a FIFO special file (a named pipe)
    mkfifo(motor_z_fifo, 0666);               // make a FIFO special file (a named pipe)
    char arr1[50];                            // creating array
    char arr2[50];                            // creating array
    int first_resize = TRUE;                  // utility variable to avoid trigger resize event on launch

    init_console_ui(); // Initialize User Interface
    if (signal(SIGUSR1, Stop_button) == SIG_ERR)
    { // in case of error
        printf("\nCannot catch SIGUSR1\n");
    }
    if (signal(SIGUSR2, Reset_button) == SIG_ERR)
    { // in case of error
        printf("\nCannot catch SIGUSR2\n");
    }

    while (TRUE) // Infinite loop
    {
        motor_x = open(motor_x_fifo, O_WRONLY); // opening FIFO
        fflush(stdout);                         // write all of the buffered data to it's destination
        sprintf(arr1, "%f", number_x);          // content of number_x is stored as a C string in the arr1
        write(motor_x, arr1, strlen(arr1 + 1)); // writing from arr1 to motor_x
        close(motor_x);                         // closing FIFO
        usleep(20000);                          // suspend execution for microsecond intervals
        motor_z = open(motor_z_fifo, O_WRONLY); // opening FIFO
        fflush(stdout);                         // write all of the buffered data to it's destination
        sprintf(arr2, "%f", number_z);          // content of number_z is stored as a C string in the arr2
        write(motor_z, arr2, strlen(arr2 + 1)); // writing from arr2 to motor_z
        close(motor_z);                         // closing FIFO
        usleep(20000);                          // suspend execution for microsecond intervals
        number_x = 0;
        number_z = 0;
        int cmd = getch(); // Get mouse/resize commands in non-blocking mode...

        if (cmd == KEY_RESIZE)
        { // If user resizes screen, re-draw UI
            if (first_resize)
            {
                first_resize = FALSE;
            }
            else
            {
                reset_console_ui();
            }
            logging("resize screen");
        }

        else if (cmd == KEY_MOUSE)
        { // Else if mouse has been pressed

            if (getmouse(&event) == OK)
            { // Check which button has been pressed...

                if (check_button_pressed(vx_incr_btn, &event))
                { // Vx++ button pressed

                    number_x = 1.0;
                    // fprintf(command_console, "Vx increase button pressed\n");
                    // fflush(command_console);
                    // fclose(command_console);
                    logging("Vx increase button pressed");
                }

                else if (check_button_pressed(vx_decr_btn, &event))
                { // Vx-- button pressed
                    number_x = 2.0;
                    // fprintf(command_console, "Vx decrease button pressed\n");
                    // fflush(command_console);
                    // fclose(command_console);
                    logging("Vx decrease button pressed");
                }

                // Vx stop button pressed
                else if (check_button_pressed(vx_stp_button, &event))
                {
                    number_x = 3.0;
                    // fprintf(command_console, "Vx stop button pressed\n");
                    // fflush(command_console);
                    // fclose(command_console);
                    logging("Vx stop button pressed");
                }

                else if (check_button_pressed(vz_incr_btn, &event))
                { // Vz++ button pressed

                    number_z = 1.0;
                    // fprintf(command_console, "Vz increase button pressed\n");
                    // fflush(command_console);
                    // fclose(command_console);
                    logging("Vz increase button pressed");
                }

                else if (check_button_pressed(vz_decr_btn, &event))
                { // Vz-- button pressed

                    number_z = 2.0;
                    // fprintf(command_console, "Vz decrease button pressed\n");
                    // fflush(command_console);
                    // fclose(command_console);
                    logging("Vz decrease button pressed");
                }

                else if (check_button_pressed(vz_stp_button, &event))
                { // Vz stop button pressed
                    number_z = 3.0;
                    // fprintf(command_console, "Vz stop button pressed\n");
                    // fflush(command_console);
                    // fclose(command_console);
                    logging("Vz stop button pressed");
                }
            }
        }
        else
        {
            logging("");
        }

        refresh();
    }

    endwin(); // Terminate
    return 0;
}