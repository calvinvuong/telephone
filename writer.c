#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/stat.h>


 // declaration required on linux

union semun {  
  int val;
  struct semid_ds *buf;
  unsigned short *array; 
  struct seminfo *__buf;
};


int main() {
  // DO NOT FORGET TO NULL TERMINATE
  int key = ftok( "control.c", 1337 );
  int sc;
  // read last line
  int shmid;
  int *memptr;
  int line_size;

  // get last line size
  shmid = shmget( key, sizeof(int), 0644 );
  memptr = (int *) shmat( shmid, 0, 0 );
  line_size = *memptr;

  printf("%d\n", line_size);
  // read last line
  int fd = open("story.txt", O_RDONLY);
  printf("%d\n", lseek(fd, line_size * -1, SEEK_END));
  char contents[line_size+1];
  read(fd, contents, line_size);
  contents[line_size] = 0;

  close(fd);
  printf("%s\n", contents);

  // prompt user
  //  char *input;
  //fgets(&input, 

  // write line to file


  // update shared memory



  return 0;
}
