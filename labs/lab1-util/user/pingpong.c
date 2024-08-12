#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p[2][2];
  if(pipe(p[0]) < 0 || pipe(p[1]) < 0){
    fprintf(2, "pipe failed\n");
    exit(1);
  }

  char buf[] = {'1'};

  int pid = fork();
  if(pid < 0){
    fprintf(2, "fork failed\n");
    exit(1);
  } else if(pid == 0){
    close(p[0][1]);
    if(read(p[0][0], buf, 1) <= 0){
      fprintf(2, "child: read failed\n");
      exit(1);
    } else {
      printf("%d: received ping\n", getpid());
      close(p[0][0]);
    }
    close(p[1][0]);
    write(p[1][1], buf, 1);
    close(p[1][1]);
  } else {
    close(p[0][0]);
    write(p[0][1], buf, 1);
    close(p[0][1]);
    if(read(p[1][0], buf, 1) <= 0){
      fprintf(2, "parent: read failed\n");
      exit(1);
    } else {
      printf("%d: received pong\n", getpid());
      close(p[1][0]);
    }
    close(p[1][1]);
  }
  exit(0);
}