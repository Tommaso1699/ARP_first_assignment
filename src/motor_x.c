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
    char vx [50] ;
    float vxx = 0.0;
    float position =0.0;
    float dt =  0.25;
    float vy = 0.0;
    int fd_1,fd_3;
    char* first_fifo = "/tmp/fifo1";
    char* third_fifo = "/tmp/fifo3";
    mkfifo(first_fifo, 0666);
    mkfifo(third_fifo, 0666);
    char arr1 [50];
    char arr2[50] = "%f";
    while((1)){
    usleep(20000);
    fd_1 = open(first_fifo, O_RDONLY);
    read(fd_1, vx, 50);
    sscanf(vx, arr2 , &vxx);
    close(fd_1);
    if(position>=(-5.9) && position<=50.9){
    position  = position + dt*vxx;
    }
    printf("Position, %f\n", position);
    fd_3 = open(third_fifo, O_WRONLY);
    fflush(stdout);
    sprintf(arr1, "%f", position );
    write(fd_3,arr1,strlen(arr1 +1));
    close(fd_3);

    }
    return 0; 
    
    }