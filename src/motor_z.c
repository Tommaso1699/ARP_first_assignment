#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

float position = 0.0;     // initializing variable
float number_z;           // initializing variable
float dt = 0.025;         // initializing variable
float speed_z = 0.0;      // initializing variable
int flag = 0;             // initializing variable
int flag2 = 0;            // initializing variable
float error = 0.00000005; // fake error
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
        speed_z = 0.0;
        flag2 = 1;
    }
}

int logging(char *log)
{
    char array[200];
    char *inspection = "/tmp/inspection";
    int fd_log = open(inspection, O_RDWR);
    memset(array, 0, sizeof(array));
    sprintf(array, "%ld;%s;%s", time(NULL), "motor_z", log);
    write(fd_log, array, strlen(array) + 1);
    close(fd_log);
}

int main(int argc, char const *argv[])
{
    FILE *motorz; // creating variable
    // motorz = fopen("motor_z.log", "w"); //creating log file
    // fprintf(motorz,"PID of process: %d\n", getpid()); //writing to log file
    // fflush(motorz); //write all of the buffered data to it's destination
    // fclose(motorz); //closing log file
    char arr1[50];                                                                    // creating array
    char arr2[50] = "%f";                                                             // creating array
    char arr3[50];                                                                    // creating array
    int motor_z_1, motor_z_2;                                                         // creating variable
    char *motor_z_fifo = "/tmp/fifo_motor_z";                                         // creating variable
    char *switch_motor_signals_fifo = "/tmp/fifo_motor_z_to_switching_motor_signals"; // creating variable
    // mkfifo(motor_z_fifo, 0666);                                                       // making a FIFO special file
    // mkfifo(switch_motor_signals_fifo, 0666);                                          // making a FIFO special file

    if (signal(SIGUSR2, Reset_button) == SIG_ERR) // in case of error
        printf("\nCannot catch SIGUSR2\n");
    if (signal(SIGUSR1, Stop_button) == SIG_ERR) // in case of error
        printf("\nCannot catch SIGUSR1\n");
    while ((1))
    {                                             // while loop
        usleep(20000);                            // suspend execution for microsecond intervals
        motor_z_1 = open(motor_z_fifo, O_RDONLY); // opening FIFO
        read(motor_z_1, arr1, 50);                // read from motor_z_1 to arr1
        sscanf(arr1, arr2, &number_z);            // read formatted input
        close(motor_z_1);                         // closing FIFO
        if (number_z == 1 || number_z == 2)
        {
            flag2 = 0;
        }
        if (flag == 1)
        {
            number_z = 2;
            flag2 = 0;
        }
        if (flag2 == 1)
        {
            number_z = 3;
            flag = 0;
        }

        if (number_z == 1)
        { // increasing speed
            speed_z = speed_z + 0.25;
        }
        if (number_z == 2)
        { // decreasing speed
            speed_z = speed_z - 0.25;
        }
        if (number_z == 3)
        { // stop movement
            speed_z = 0.0;
        }
        if (position >= 0.0 && position <= 9.0)
        { // changing position
            position = position + (dt * speed_z) + error;
        }
        if (position <= 0)
        { // minimum boundary
            position = 0.0;
            speed_z = 0.0;
            flag = 0;
        }
        if (position >= 9)
        { // maximum boundary
            position = 9.0;
            speed_z = 0.0;
        }
        //?
        motor_z_2 = open(switch_motor_signals_fifo, O_WRONLY); // opening FIFO
        fflush(stdout);                                        // write all of the buffered data to it's destination
        sprintf(arr3, "%f", position);                         // content of position is stored as a C string in the arr1
        write(motor_z_2, arr3, strlen(arr3 + 1));              // writing from arr1 to motor_z_2
        close(motor_z_2);                                      // closing FIFO
        logging(arr3);
        usleep(20000); // suspend execution for microsecond intervals
    }
    unlink(switch_motor_signals_fifo);
    unlink(motor_z_fifo);
    return 0;
}
