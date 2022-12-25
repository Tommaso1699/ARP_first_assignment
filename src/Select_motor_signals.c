#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
int random_func(int x, int y) //generate random number
{
    int number = (rand()%(y-x+1))+x;
    return number;
}
int main(int argc, char const *argv[]) //main
{
char *motor_x = "/tmp/fifo_motor_x_to_switching_motor_signals"; //initializing variable
mkfifo(motor_x,0666); // make a FIFO special file (a named pipe)
char *motor_z = "/tmp/fifo_motor_z_to_switching_motor_signals"; //initializing variable
mkfifo(motor_z,0666); // make a FIFO special file (a named pipe)
char *updateui = "/tmp/update_ui"; //initializing variable
mkfifo(updateui,0666); // make a FIFO special file (a named pipe)
int motorx, motorz, update_ui; //creating variables
char arr1[80]; //creating array
char arr2[80]; //creating array
char position[80]; //creating array
char arr3[80] = "%f"; //creating array
char arr4[80] = "%f,%f"; //creating array
float x = 0.0; //initialazing variable
float y = 0.0; //initialazing variable
while(1){ //infinite loop
    usleep(20000); // suspend execution for microsecond intervals
    fd_set fds; //creating variable
    FD_ZERO(&fds); //clear a set
    motorx = open(motor_x, O_RDWR); // opening FIFO
    motorz = open(motor_z, O_RDWR); // opening FIFO
    FD_SET(motorx, &fds); //add a given file descriptor from a set
    FD_SET(motorz, &fds); //add a given file descriptor from a set
    int stream = motorx>motorz?motorx:motorz; //check which FIFO is sending data 
    int choose = select(stream +1, &fds, NULL, NULL, NULL); //monitor multiple file descriptors, waiting until one or more of the file descriptors become ready"
    switch(choose){ //switch case
        case 2: //if both FIFO are sending data, choose randomly first
        int rand_num = random_func(0,1);
        if(rand_num == 0){
            read(motorz,arr2,80);
            sscanf(arr2,arr3,&y);
            close(motorx);
            close(motorz);

        }
        else{
            read(motorx,arr1,80);
            sscanf(arr1,arr3,&x);
            close(motorx);
            close(motorz);   
        }
        break;

        case 1: //if only one FIFO is sending data
        fflush(stdout);
        if(FD_ISSET(motorx,&fds)!=0){
            fflush(stdout);
            read(motorx,arr2,80);
            sscanf(arr2,arr3,&x);
            close(motorx);
            close(motorz);

        }
        else if(FD_ISSET(motorz,&fds)!=0){
            fflush(stdout);
            read(motorz,arr2,80);
            sscanf(arr2,arr3,&y);
            close(motorx);
            close(motorz);

        }
        break;
    case -1: //in case of an error
        perror("Error");
        break;
    }
    update_ui = open(updateui,O_RDWR); // opening FIFO
    sprintf(position, arr4, x, y); // content of x and y is stored as a C string in the position
    write(update_ui, position, strlen(position)+1); // writing from position to update_ui
    close(update_ui); //closing FIFO


}   

    return 0;
}