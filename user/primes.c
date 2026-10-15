#pragma GCC diagnostic ignored "-Winfinite-recursion"

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
prime_process(int lfd)
{
  int p;
  if(read(lfd, &p, sizeof(p)) != sizeof(p)){
    close(lfd);
    exit(0);
  }
  printf("prime %d\n", p);

  int fd[2];
  pipe(fd);
  int pid = fork();
  if(pid < 0){
    fprintf(2, "primes: fork failed\n");
    exit(1);
  }

  if(pid == 0){
    close(lfd);
    close(fd[1]);
    prime_process(fd[0]);
  } else {
    close(fd[0]);
    int n;
    while(read(lfd, &n, sizeof(n)) == sizeof(n)){
      if(n % p != 0){
        write(fd[1], &n, sizeof(n));
      }
    }
    close(lfd);
    close(fd[1]);
    wait(0);
    exit(0);
  }
}

int
main(int argc, char *argv[])
{
  int fd[2];
  pipe(fd);
  int pid = fork();
  if(pid < 0){
    fprintf(2, "primes: fork failed\n");
    exit(1);
  }

  if(pid == 0){
    close(fd[1]);
    prime_process(fd[0]);
  } else {
    close(fd[0]);
    for(int i = 2; i <= 35; i++){
      write(fd[1], &i, sizeof(i));
    }
    close(fd[1]);
    wait(0);
    exit(0);
  }
  exit(0);
}
