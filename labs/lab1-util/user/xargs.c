#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

// readline from stdin
char *
readline()
{
  char *buf = malloc(MAXARG * sizeof(char));
  char *p = buf;
  while(read(0, p, 1) > 0){
    if(*p == '\n'){
      *p = '\0';
      return buf;
    }
    p++;
  }
  if(p != buf){
    return buf;
  }
  free(buf);
  return 0;
}

int
main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "Usage: xargs cmd [args...]\n");
    exit(1);
  }

  char *cmd = argv[1];
  char **args = malloc(argc * sizeof(char *));
  for(int i = 1; i < argc; i++){
    args[i-1] = argv[i];
  }

  char *line;
  while((line = readline()) != 0){
    int pid = fork();
    if(pid < 0){
      fprintf(2, "fork failed\n");
      exit(1);
    } else if(pid == 0){
      args[argc-1] = line;
      exec(cmd, args);
    } else {
      wait(0);
    }
    free(line);
  }
  free(args);
  exit(0);
}