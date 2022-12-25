#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include <time.h>

float position =0.0;
float vzz = 0.0; 
float dt = 0.025;
float speed_z =0.0;
void reset(int signo){
    if(signo==SIGUSR2){
      speed_z = 0.0;
    }
}

int logging(char* log)
{
    char array[200];
    char *inspection = "/tmp/inspection";
    int fd_log = open(inspection, O_RDWR);
    memset(array, 0, sizeof(array));
    sprintf(array, "%ld;%s;%s", time(NULL), "motor_z", log);
    write(fd_log, array, strlen(array)+1);
    close(fd_log);
}

int main(int argc, char const *argv[])
{
    FILE *motorz; //creating variable
    //motorz = fopen("motor_z.log", "w"); //creating log file
    //fprintf(motorz,"PID of process: %d\n", getpid()); //writing to log file
    //fflush(motorz); //write all of the buffered data to it's destination
    //fclose(motorz); //closing log file
    char vz [50] ; //creating array
    float vzz = 0.0; //creating variable
     //creating variable
    int motor_z_1, motor_z_2; //creating variable
    char* motor_z_fifo = "/tmp/fifo_motor_z"; //creating variable
    char* switch_motor_signals_fifo = "/tmp/fifo_motor_z_to_switching_motor_signals"; //creating variable
    //mkfifo(motor_z_fifo, 0666); //making a FIFO special file
    //mkfifo(switch_motor_signals_fifo, 0666); //making a FIFO special file
    char arr1 [50]; //creating array
    char arr2[50] = "%f"; //creating array
     if (signal(SIGUSR2, reset) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    while((1)){ //while loop
     usleep(20000);                     // suspend execution for microsecond intervals
        motor_z_1 = open(motor_z_fifo, O_RDONLY); // opening FIFO
        read(motor_z_1, vz, 50);                // read to buffer
        sscanf(vz, arr2, &vzz);            // read formatted input
        close(motor_z_1);                // closing FIFO

        if(vzz ==1){
            speed_z = speed_z +0.005;
        }
        if(vzz==2){
            speed_z = speed_z -0.005;
        }
        if(vzz==3){
            speed_z = 0.0;
        }
        if(position>=0.0 && position<=9.0){
        position = position + dt * speed_z;
        }
        if(position<=0){
            position =0.0;
            speed_z=0.0;
        }
        if(position>=9){
            position =9.0;
            speed_z=0.0;
        }
    //?
    motor_z_2 = open(switch_motor_signals_fifo, O_WRONLY);
    fflush(stdout);
    sprintf(arr1, "%f", position );
    write(motor_z_2,arr1,strlen(arr1 +1));
    close(motor_z_2);
    logging("motor_z");
    usleep(20000);
    }
    unlink(switch_motor_signals_fifo);
    unlink(motor_z_fifo);
    return 0;
}
