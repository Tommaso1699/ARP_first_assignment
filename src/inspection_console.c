#include "./../include/inspection_utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
float ee_x = 0.0;
float ee_y = 0.0;
long interrupt(const char *process)
{
    char arr[10];
    char *str = "pidof -s ";
    strcpy(str, process);
    const char *cmd = str;
    FILE *command = popen(cmd, "r");
    fgets(arr, 10, command);
    pid_t pid = strtoul(arr, NULL, 10);
    pclose(command);
    return pid;
}
void stop(int signo1)
{
    if (signo1 == SIGUSR1)
    {
        fflush(stdout);
        kill(interrupt("command"), SIGUSR1);
    }
}
void reset(int signo2)
{
    if (signo2 == SIGUSR2)
    {
        fflush(stdout);
        char line1[10];
        char line2[10];
        char line3[10];
        FILE *cmd3 = popen("pidof -s command", "r");
        FILE *cmd1 = popen("pidof -s motor_x", "r");
        FILE *cmd2 = popen("pidof -s motor_z", "r");
        fgets(line1, 10, cmd1);
        fgets(line2, 10, cmd2);
        fgets(line3, 10, cmd3);
        long pid1 = 0;
        long pid2 = 0;
        long pid3 = 0;
        pid1 = strtoul(line1, NULL, 10);
        pid2 = strtoul(line2, NULL, 10);
        pid3 = strtoul(line3, NULL, 10);
        kill(pid3, SIGUSR2);
        kill(pid1, SIGUSR2);
        kill(pid2, SIGUSR2);
    }
}
int main(int argc, char const *argv[])
{
    // Utility variable to avoid trigger resize event on launch
    int first_resize = TRUE;

    // End-effector coordinates

    // Initialize User Interface
    init_console_ui();
    int updateui;
    char *update_ui = "/tmp/update_ui";
    mkfifo(update_ui, 0666);
    float vx = 0.0;
    float vz = 0.0;
    char arr1[50];
    char arr2[50] = "";
    char arr3[50];
    char arr4[50] = "%f";
    char arr5[50];
    char arr6[50] = "%f,%f";
    int motor_x, motor_z;
    char *motor_x_fifo = "/tmp/fifo_motor_x";
    char *motor_z_fifo = "/tmp/fifo_motor_z";
    mkfifo(motor_x_fifo, 0666);
    mkfifo(motor_z_fifo, 0666);
    // Infinite loop
    while (TRUE)
    {
        // Get mouse/resize commands in non-blocking mode...
        int cmd = getch();

        // If user resizes screen, re-draw UI
        if (cmd == KEY_RESIZE)
        {
            if (first_resize)
            {
                first_resize = FALSE;
            }
            else
            {
                reset_console_ui();
            }
        }
        // Else if mouse has been pressed
        else if (cmd == KEY_MOUSE)
        {

            // Check which button has been pressed...
            if (getmouse(&event) == OK)
            {

                // STOP button pressed
                if (check_button_pressed(stp_button, &event))
                {
                    if (signal(SIGUSR1, stop) == SIG_ERR)
                        printf("\ncan't catch SIGINT\n");
                    // signal(SIGUSR1, Handle);
                    kill(getpid(), SIGUSR1);
                }

                // RESET button pressed
                else if (check_button_pressed(rst_button, &event))
                {
                    if (signal(SIGUSR2, reset) == SIG_ERR)
                        printf("\ncan't catch SIGINT\n");
                    kill(getpid(), SIGUSR2);
                }
            }
        }
        updateui = open(update_ui, O_RDONLY);
        read(updateui, arr5, 50);
        sscanf(arr5, arr6, &ee_x, &ee_y);
        close(updateui);

        // Update UI
        update_console_ui(&ee_x, &ee_y);
    }

    // Terminate
    endwin();
    return 0;
}