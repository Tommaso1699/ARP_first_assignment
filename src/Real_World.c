#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

int generate_random(int l, int r)
{
    int rand_num = (rand() % (r - l + 1)) + l;
    return rand_num;
}

int main(int argc, char const *argv[])
{
    char *motor_x = "/tmp/fifo_motor_x_to_switching_motor_signals";
    mkfifo(motor_x, 0666);
    char *motor_z = "/tmp/fifo_motor_z_to_switching_motor_signals";
    mkfifo(motor_z, 0666);
    char *updateui = "/tmp/update_ui";
    mkfifo(updateui, 0666);
    int motorx, motorz, update_ui;
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
    struct timeval tv;

    while (1)
    {
        usleep(20000);
        fd_set fds;
        int maxfd;
        int res;
        int retval;
        motorx = open(motor_x, O_RDWR);
        motorz = open(motor_z, O_RDWR);
        FD_ZERO(&fds);
        FD_SET(motorx, &fds);
        FD_SET(motorz, &fds);
        maxfd = motorx > motorz ? motorx : motorz;
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        retval = select(maxfd + 1, &fds, NULL, NULL, &tv);
        switch (retval)
        {
        case -1:
            perror("select()");
            return -1;
        case 0:
            // no data received
            close(motorx);
            close(motorz);
            break;
        case 2:
            fflush(stdout);
            int rand_num = generate_random(0, 1);
            if (rand_num == 0)
            {
                read(motorx, px, 80);
                sscanf(px, format_string, &posx);
                close(motorx);
                close(motorz);
            }
            else
            {
                read(motorz, py, 80);
                sscanf(py, format_string, &posy);
                close(motorx);
                close(motorz);
            }
            break;

        case 1:
            fflush(stdout);
            if (FD_ISSET(motorx, &fds) != 0)
            {
                fflush(stdout);
                res = read(motorx, px, 80);
                sscanf(px, format_string, &posx);
                close(motorx);
                close(motorz);
            }
            else if (FD_ISSET(motorz, &fds) != 0)
            {
                fflush(stdout);
                res = read(motorz, py, 80);
                sscanf(py, format_string, &posy);
                close(motorx);
                close(motorz);
            }
            break;
        }
        //  printf("posx:%f,posy:%f\n", posx, posy);
        update_ui = open(updateui, O_RDWR);
        sprintf(realpos, "%f,%f", posx, posy);
        write(update_ui, realpos, strlen(realpos) + 1);
        close(update_ui);
    }

    return 0;
}