#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

float position = 0.0;    // initializing variable
float number_x;          // initializing variable
float dt = 0.025;        // initializing variable
float speed_x = 0.0;     // initializing variable
int flag = 0;            // initializing variable
int flag2 = 0;           // initializing variable
float error = 0.0000005; // fake error
void Reset_button(int sig)
{ // function that is activating after receiving signal
    if (sig == SIGUSR2)
    {
        flag = 1;
    }
}
void Stop_button(int sig)
{ // function that is activating after receiving signal
    if (sig == SIGUSR1)
    {
        speed_x = 0.0;
        flag2 = 1;
    }
}

int logging(char *log)
{
    char array[200];
    char *inspection = "/tmp/inspection";
    int fd_log = open(inspection, O_RDWR);
    memset(array, 0, sizeof(array));
    sprintf(array, "%ld;%s;%s", time(NULL), "motor_x", log);
    write(fd_log, array, strlen(array) + 1);
    close(fd_log);
}

int main(int argc, char const *argv[])
{
    FILE *motorx; // creating variable
    // motorx = fopen("motor_x.log", "w"); // creating log file
    //  fprintf(motorx,"PID of process: %d\n", getpid()); //writing to log file
    // fflush(motorx); //write all of the buffered data to it's destination
    // fclose(motorx); //closing log file
    char arr1[50];                                                                    // creating array
    char arr2[50] = "%f";                                                             // creating array
    char arr3[50];                                                                    // creating array
    int motor_x_1, motor_x_2;                                                         // creating variable
    char *motor_x_fifo = "/tmp/fifo_motor_x";                                         // creating variable
    char *switch_motor_signals_fifo = "/tmp/fifo_motor_x_to_switching_motor_signals"; // creating variable
    // mkfifo(motor_x_fifo, 0666);                                                       // making a FIFO special file
    // mkfifo(switch_motor_signals_fifo, 0666);                                          // making a FIFO special file

    if (signal(SIGUSR2, Reset_button) == SIG_ERR) // in case of error
        printf("\nCannot catch SIGUSR2\n");
    if (signal(SIGUSR1, Stop_button) == SIG_ERR) // in case of error
        printf("\nCannot catch SIGUSR1\n");

    while (1)
    {                                             // while loop                            // suspend execution for microsecond intervals
        motor_x_1 = open(motor_x_fifo, O_RDONLY); // opening FIFO
        read(motor_x_1, arr1, 50);                // read to arr1 from motor_x_1
        sscanf(arr1, arr2, &number_x);            // read formatted input
        close(motor_x_1);
        usleep(20000);
        if (number_x == 1 || number_x == 2)
        {
            flag2 = 0;
        }
        if (flag == 1)
        {
            number_x = 2;
            flag2 = 0;
        }
        if (flag2 == 1)
        {
            number_x = 3;
            flag = 0;
        } // closing FIFO
        if (number_x == 1)
        { // increasing speed
            speed_x = speed_x + 0.5;
        }
        if (number_x == 2)
        { // decreasing speed
            speed_x = speed_x - 0.5;
        }
        if (number_x == 3)
        { // stop movement

            speed_x = 0.0;
        }
        if (position >= 0.0 && position <= 40.0)
        { // changing position
            position = position + (dt * speed_x) + error;
        }
        if (position <= 0)
        { // minimum boundary
            position = 0.0;
            speed_x = 0.0;
            flag = 0;
        }
        if (position >= 40)
        { // maximum boundary
            position = 40.0;
            speed_x = 0.0;
        }

        // printf("Position, %f\n", position);
        //?
        motor_x_2 = open(switch_motor_signals_fifo, O_WRONLY); // opening FIFO
        fflush(stdout);                                        // write all of the buffered data to it's destination
        sprintf(arr3, "%f", position);                         // content of position is stored as a C string in the arr1
        write(motor_x_2, arr3, strlen(arr3 + 1));              // writing from arr1 to motor_x_2
        close(motor_x_2);                                      // closing FIFO
        logging(arr3);
        usleep(20000); // suspend execution for microsecond intervals
    }

    return 0;
}
