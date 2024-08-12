#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
prime_child(int lp[2])
{
  int prime;      
  close(lp[1]);
  if(read(lp[0], &prime, sizeof(prime)) == 0){
    close(lp[0]);
    exit(0);
  }
  printf("prime %d\n", prime);

  int rp[2];
  if(pipe(rp) < 0){
    fprintf(2, "pipe failed\n");
    exit(1);
  }

  int n;
  int pid = fork();
  if(pid < 0){
    fprintf(2, "fork failed\n");
    exit(1);
  } else if(pid > 0){
    close(rp[0]);
    while(read(lp[0], &n, sizeof(n)) != 0){
      if(n % prime != 0){
        write(rp[1], &n, sizeof(n));
      }
    }
    close(lp[0]);
    close(rp[1]);
    wait(0);
    exit(0);
  } else {
    prime_child(rp);
    exit(0);
  }
}

int
main(int argc, char *argv[])
{
  int p[2];
  if(pipe(p) < 0){
    fprintf(2, "pipe failed\n");
    exit(1);
  }
  
  int pid = fork();
  if(pid < 0){
    fprintf(2, "fork failed\n");
    exit(1);
  } else if(pid > 0){
    close(p[0]);  
    int i;
    for(i = 2; i <= 35; i++){
      write(p[1], &i, sizeof(i));
    }
    close(p[1]);
    wait(0);
    exit(0);
  } else {
    prime_child(p);
    exit(0);
  }
}