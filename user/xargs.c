#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
  char buf[512];
  char *nargv[MAXARG];
  int nargc = 0;

  for(int i = 1; i < argc; i++){
    nargv[nargc++] = argv[i];
  }

  char c;
  int index = 0;
  while(read(0, &c, 1) == 1){
    if(c == '\n'){
      buf[index] = 0;
      nargv[nargc++] = buf;
      nargv[nargc] = 0;

      if(fork() == 0){
        exec(nargv[0], nargv);
        exit(0);
      }
      wait(0);
      nargc = argc - 1;
      index = 0;
    } else {
      if(index < sizeof(buf) - 1){
        buf[index++] = c;
      }
    }
  }
  exit(0);
}
