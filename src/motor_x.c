#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
float position = 0.0;
float vxx = 0.0; 
float dt = 0.25;
int flag =0;
void reset(int signo){
    if(signo==SIGUSR2){
       flag =1;
    }
}
int main(int argc, char const *argv[])
{
    FILE *motorx;                       // creating variable
    //motorx = fopen("motor_x.log", "w"); // creating log file
   //  fprintf(motorx,"PID of process: %d\n", getpid()); //writing to log file
    // fflush(motorx); //write all of the buffered data to it's destination
    // fclose(motorx); //closing log file
    char vx[50];                     // creating array                 // creating variable
                // creating variable
                     // creating variable
    int motor_x_1, motor_x_2;                  // creating variable
    char *motor_x_fifo = "/tmp/fifo_motor_x"; // creating variable
    char *switch_motor_signals_fifo = "/tmp/fifo_motor_x_to_switching_motor_signals"; // creating variable
    mkfifo(motor_x_fifo, 0666);        // making a FIFO special file
    mkfifo(switch_motor_signals_fifo, 0666);        // making a FIFO special file
    char arr1[50];                   // creating array
    char arr2[50] = "%f";            // creating array
    if (signal(SIGUSR2, reset) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    while (1)
    {                                      // while loop
        usleep(20000);                     // suspend execution for microsecond intervals
        motor_x_1 = open(motor_x_fifo, O_RDONLY); // opening FIFO
        read(motor_x_1, vx, 50);                // read to buffer
        sscanf(vx, arr2, &vxx);            // read formatted input
        close(motor_x_1);                // closing FIFO
        if (position >= (-5.9) && position <= 50.9)
        {                                   // constraints on x axis
            position = position + dt * vxx; // computing position
           // motorx = fopen("motor_x.log", "a");
            // fprintf(motorx, "Position, %f\n", position);
            // fflush(motorx);
            // fclose(motorx);
        }

        printf("Position, %f\n", position);
        //?
        motor_x_2 = open(switch_motor_signals_fifo, O_WRONLY); // opening FIFO
        fflush(stdout);                    // write all of the buffered data to it's destination
        sprintf(arr1, "%f", position);
        write(motor_x_2, arr1, strlen(arr1 + 1));
        close(motor_x_2); // closing FIFO
    }
    
    return 0;
}