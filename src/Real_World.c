#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
int generate_random(int l, int r)
{
    int rand_num = (rand()%(r-l+1))+l;
    return rand_num;
}
int main(int argc, char const *argv[])
{
char *myfifo = "/tmp/fifo3";
mkfifo(myfifo,0666);
char *myfifo2 = "/tmp/fifo4";
mkfifo(myfifo2,0666);
char *myfifo3 = "/tmp/fifo5";
mkfifo(myfifo3,0666);
char vel[80];
char px[80];
char py[80];
char realpos[80];
char format_string[80] = "%f";
char format_string_2[80] = "%f,%f";
float posx, posy;
posx = 0.0;
posy = 0.0;
float vx = 0;
while(1){
    usleep(20000);
    fd_set fds;
    struct timeval tv;
    int maxfd;
    int res;
    int retval,m,n;
    char buf[256];
    int fd1 = open(myfifo, O_RDWR);
    int fd2 = open(myfifo2, O_RDWR);
    int fd3;
    FD_ZERO(&fds);
    FD_SET(fd1, &fds);
    FD_SET(fd2, &fds);
    tv.tv_sec = 0.1;
    tv.tv_usec =0;
    maxfd = fd1>fd2?fd1:fd2;
    retval  = select(maxfd +1, &fds, NULL, NULL, NULL);
    switch(retval){
        case -1:
        perror("select()");
        return -1;
        case 2:
        fflush(stdout);
        int rand_num = generate_random(0,1);
        if(rand_num ==0){
            read(fd1,px,80);
            sscanf(px,format_string,&posx);
            close(fd1);
            close(fd2);

        }
        else{
            read(fd2,py,80);
            sscanf(py,format_string,&posy);
            close(fd1);
            close(fd2);
        }
        break;

        case 1:
        fflush(stdout);
        if(FD_ISSET(fd1,&fds)!=0){
            fflush(stdout);
            res = read(fd1,px,80);
            sscanf(px,format_string,&posx);
            close(fd1);
            close(fd2);

        }
        else if(FD_ISSET(fd2,&fds)!=0){
            fflush(stdout);
            res = read(fd2,py,80);
            sscanf(py,format_string,&posy);
            close(fd1);
            close(fd2);

        }
        break;


    }
    printf("posx:%f,posy:%f\n", posx, posy);
    fd3 = open(myfifo3,O_RDWR);
    sprintf(realpos, "%f,%f", posx, posy);
    write(fd3, realpos, strlen(realpos)+1);
    close(fd3);


}   

    return 0;
}