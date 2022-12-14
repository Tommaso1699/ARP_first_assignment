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
    char vz [50] ;
    float vzz = 0.0;
    float position =0.0;
    float dt =  0.25;
    int fd_2, fd_4;
    char* second_fifo = "/tmp/fifo2";
    char* fourth_fifo = "/tmp/fifo4";
    mkfifo(second_fifo, 0666);
    mkfifo(fourth_fifo, 0666);
    char arr1 [50];
    char arr2[50] = "%f";
    while((1)){
    usleep(20000);
   fd_2 = open(second_fifo, O_RDONLY);
    read(fd_2, vz, 50);
    sscanf(vz, arr2 , &vzz);
    close(fd_2);
    if(position>=(-5.9) && position<=15.9)
    position  = position + dt*vzz;
    printf("Position, %f\n", position);
    fd_4 = open(fourth_fifo, O_WRONLY);
    fflush(stdout);
    sprintf(arr1, "%f", position );
    write(fd_4,arr1,strlen(arr1 +1));
    close(fd_4);
    }
    return 0;
}
    