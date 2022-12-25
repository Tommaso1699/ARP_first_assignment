#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>

#define PROCESS_TIMEOUT 15 // timeout in seconds
#define array_len(a) sizeof(a) / sizeof(a[0])

FILE *logfile;
char *inspection = "/tmp/inspection";

typedef struct
{
  char *path;
  char *arg_list[4];
  char *process_name;
  pid_t pid;
  time_t timestamp;
} processInfo;

processInfo process_list[] = {
    {"/usr/bin/konsole", {"/usr/bin/konsole", "-e", "./bin/inspection", NULL}, "inspection", 0, 0},
    {"/usr/bin/konsole", {"/usr/bin/konsole", "-e", "./bin/command", NULL}, "command", 0, 0},
    {"./bin/motor_x", {"", "", "", NULL}, "motor_x", 0, 0},
    {"./bin/motor_z", {"", "", "", NULL}, "motor_z", 0, 0},
    {"./bin/real_world", {"", "", "", NULL}, "real_world", 0, 0}};

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
    printf("%s\n", process_list[i].arg_list[0]);
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

void hoist_exit(int signo)
{
  if(signo == SIGINT)
  {
    stop(process_list, array_len(process_list));
    unlink(inspection);
    fclose(logfile);
  }
}


int check_timeout(processInfo *process_list, int size)
{
  time_t now = time(NULL);
  printf("Processes status at %ld\n", now);
  for (int i = 0; i < size; i++)
  {
    if (process_list[i].pid > 0)
    {
      if (difftime(now, process_list[i].timestamp) >= PROCESS_TIMEOUT)
      {
        // printf("%d, timeout\n", process_list[i].pid);
        kill(process_list[i].pid, SIGKILL);
        int status;
        waitpid(process_list[i].pid, &status, 0);
        printf("%d, status: %d\n", process_list[i].pid, status);
        // restart process
        start(process_list + i, 1);
      }
      else
      {
        printf("%d: %s: %f\n", process_list[i].pid, process_list[i].process_name, difftime(now, process_list[i].timestamp));
      }
    }
  }
  return 0;
}

int update_timeout(processInfo *process_list, int size, char *msg_name, time_t msg_time)
{
  time_t now = time(NULL);
  //printf("check %ld\n", now);
  for (int i = 0; i < size; i++)
  {
    if (strcmp(process_list[i].process_name, msg_name) == 0)
    {
      process_list[i].timestamp = msg_time;
      break;
    }
  }
  return 0;
}

int msg_handler(char *buff)
{
  time_t msg_time;
  char msg_name[20];
  char msg_log[200];
  char *tmp = strtok(buff, ";");
  if(tmp != NULL)
  {
    sscanf(tmp, "%ld", &msg_time);
    tmp = strtok(NULL, ";");
  }
  else
    return 0;
  
  if(tmp != NULL)
  {
    strcpy(msg_name, tmp);
    tmp = strtok(NULL, ";");
  }
  else
    return 0;
  
  if(tmp != NULL)
  {
    strcpy(msg_log, tmp);
    tmp = strtok(NULL, ";");
  }
  else
    return 0;

  if (strlen(msg_log) > 0)
  {
    printf("write to log: %s\n", msg_log);
    fprintf(logfile, "Time: %ld, Process: %s, Log message: %s\n", msg_time, msg_name, msg_log);
    fflush(logfile);
  }
    update_timeout(process_list, array_len(process_list), msg_name, msg_time);

  return 0;
}

int main()
{
  struct timeval tv;
  int fd_inspection;
  int retval;
  
  start(process_list, array_len(process_list));
  mkfifo(inspection, 0666);
  char array[300];
  //logfile = fopen("logs.txt", "a+");
  logfile = fopen("logs.txt", "w+");


  printf("aaa\n");
  while (1)
  {
    fd_inspection = open(inspection, O_RDWR);
    //printf("open %d\n", fd_inspection);
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd_inspection, &fds);
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    retval = select(fd_inspection + 1, &fds, NULL, NULL, &tv);
    printf("%d\n", retval);
    switch (retval)
    {
    case -1:
      perror("Select inspection error");
      break;
    case 0:
      check_timeout(process_list, array_len(process_list));
      break;
    default:
      memset(array, 0, sizeof(array));
      int r = read(fd_inspection, array, 300);
      if (r > 0)
      {
        //printf("Inspection: %d [%s]\n", r, array);
        msg_handler(array);        
      }
      check_timeout(process_list, array_len(process_list));

      break;
    }
    close(fd_inspection);
  }

  hoist_exit(6);
  return 0;
}