#include <stdio.h>
#include <unistd.h>

int main() {
  printf("Parent pid is %d\n", getpid());
  int pid= fork();
  if(pid==0){
    printf("child Process child id is %d\n",getpid());
  }else if(pid>0){
    printf("parent process pid is %d\n",getpid());
  }else{
    perror("Error");
  }
}
