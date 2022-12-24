#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>

typedef struct
{
  char *path;
  char *arg_list[4];
  pid_t pid;
  time_t timestamp;
} processInfo;

processInfo process_list[] = {
    {"/usr/bin/konsole", {"/usr/bin/konsole", "-e", "./bin/inspection", NULL}, 0},
    {"/usr/bin/konsole", {"/usr/bin/konsole", "-e", "./bin/command", NULL}, 0},
    {"/usr/bin/konsole", {"/usr/bin/konsole", "-e", "./bin/motor_x", NULL}, 0},
    {"/usr/bin/konsole", {"/usr/bin/konsole", "-e", "./bin/motor_z", NULL}, 0},
    /*{"./bin/motor_x", {"", "", "", NULL}, 0},
    {"./bin/motor_z", {"", "", "", NULL}, 0},*/
    {"./bin/real_world", {"", "", "", NULL}, 0}};

int spawn(const char *program, char *arg_list[])
{
  printf("pid: %d\n", getpid());
  pid_t child_pid = fork();

  if (child_pid < 0)
  {
    perror("Error while forking...");
    return -1;
  }

  else if (child_pid != 0)
  {
    return child_pid;
  }

  else
  {
    if (execvp(program, arg_list) < 0)
    {
      perror("Exec failed");
      exit(1);
    }
    return -1;
  }
}

int start(processInfo *process_list, int size)
{
  for (int i = 0; i < size; i++)
  {
    printf("%s\n" ,process_list[i].arg_list[0]);
    process_list[i].pid = spawn(process_list[i].path, process_list[i].arg_list);
    printf("%d : %d %s\n", i, process_list[i].pid, process_list[i].path);
    if (process_list[i].pid == -1)
    {
      perror("Cannot create process");
      return -1;
    }
    process_list[i].timestamp = time(NULL);
  }
  return 0;
}

int stop(processInfo *process_list, int size)
{
  printf("stop %d\n", getpid());
  for (int i = 0; i < size; i++)
  {
    if (process_list[i].pid > 0)
    {
      if (kill(process_list[i].pid, SIGABRT) == -1)
      {
        perror("Error aborting process");
      }
      else
      {
        printf("Process %d terminated successfully\n", process_list[i].pid);
        process_list[i].pid = 0;
      }
    }
  }
  return 0;
}

int main()
{
  struct timeval tv;
  int fd_inspection;
  int retval;
  start(process_list, 5);

  printf("a");
  char *inspection = "/tmp/inspection";
  mkfifo(inspection, 0666);
  char array[200];
  while (1)
  {
    printf("a");
    fd_inspection = open(inspection, O_RDONLY);
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd_inspection, &fds);
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    retval = select(fd_inspection + 1, &fds, NULL, NULL, &tv);
    switch (retval)
    {
    case -1:
      perror("Select inspection error");
      break;
    case 0:
      printf("Inspection timeout");
      break;
    default:
      read(fd_inspection, array, 200);
      printf("Inspection: %s\n", array);
      break;
    }
    close(fd_inspection);
  }

  /*
    int status;
    waitpid(pid_cmd, &status, 0);
    waitpid(pid_insp, &status, 0);

    printf ("Main program exiting with status %d\n", status);
    */

  sleep(1000);

  stop(process_list, 5);
  unlink(inspection);
  return 0;
}