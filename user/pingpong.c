#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p1[2], p2[2];
  char buf = 'X';

  if(pipe(p1) < 0 || pipe(p2) < 0){
    fprintf(2, "pingpong: pipe failed\n");
    exit(1);
  }

  int pid = fork();
  if(pid < 0){
    fprintf(2, "pingpong: fork failed\n");
    exit(1);
  }

  if(pid == 0){
    close(p1[1]);
    close(p2[0]);

    if(read(p1[0], &buf, 1) != 1){
      fprintf(2, "pingpong: child read failed\n");
      exit(1);
    }
    printf("%d: received ping\n", getpid());

    if(write(p2[1], &buf, 1) != 1){
      fprintf(2, "pingpong: child write failed\n");
      exit(1);
    }

    close(p1[0]);
    close(p2[1]);
    exit(0);
  } else {
    close(p1[0]);
    close(p2[1]);

    if(write(p1[1], &buf, 1) != 1){
      fprintf(2, "pingpong: parent write failed\n");
      exit(1);
    }

    if(read(p2[0], &buf, 1) != 1){
      fprintf(2, "pingpong: parent read failed\n");
      exit(1);
    }
    printf("%d: received pong\n", getpid());

    wait(0);

    close(p1[1]);
    close(p2[0]);
    exit(0);
  }
}
