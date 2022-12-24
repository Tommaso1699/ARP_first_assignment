#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
float position = 0.0;
int flag = 0;
void reset(int signo)
{
    if (signo == SIGUSR2)
    {
        flag = 1;
    }
}
int main(int argc, char const *argv[])
{
    FILE *motorz; // creating variable
    // motorz = fopen("motor_z.log", "w"); //creating log file
    // fprintf(motorz,"PID of process: %d\n", getpid()); //writing to log file
    // fflush(motorz); //write all of the buffered data to it's destination
    // fclose(motorz); //closing log file
    char vz[50];     // creating array
    float vzz = 0.0; // creating variable
                     // creating variable
    float dt = 0.25;                                                                  // creating variable
    int motor_z_1, motor_z_2;                                                         // creating variable
    char *motor_z_fifo = "/tmp/fifo_motor_z";                                         // creating variable
    char *switch_motor_signals_fifo = "/tmp/fifo_motor_z_to_switching_motor_signals"; // creating variable
    mkfifo(motor_z_fifo, 0666);                                                       // making a FIFO special file
    mkfifo(switch_motor_signals_fifo, 0666);                                          // making a FIFO special file
    char arr1[50];                                                                    // creating array
    char arr2[50] = "%f";                                                             // creating array
    if (signal(SIGUSR2, reset) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    while ((1))
    {                                               // while loop
        usleep(20000);                              // suspend execution for microsecond intervals
        motor_z_1 = open(motor_z_fifo, O_RDONLY);   // opening FIFO
        read(motor_z_1, vz, 50);                    // read to buffer
        sscanf(vz, arr2, &vzz);                     // read formatted input
        close(motor_z_1);                           // closing FIFO
        if (position >= (-5.9) && position <= 15.9) // constraints on x axis
        {
            position = position + dt * vzz; // computing position
            // motorz = fopen("motor_z.log", "a");
            // fprintf(motorz, "Position, %f\n", position);
            // fflush(motorz);
            // fclose(motorz);
        }
        //?
        motor_z_2 = open(switch_motor_signals_fifo, O_WRONLY);
        fflush(stdout);
        sprintf(arr1, "%f", position);
        write(motor_z_2, arr1, strlen(arr1 + 1));
        close(motor_z_2);
    }
    return 0;
}
