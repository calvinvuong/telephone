#include "main.h"

int main() {
  // DO NOT FORGET TO NULL TERMINATE
  int key = ftok( "control.c", 1337 );

  int shmid;
  int semid;
  int *memptr;
  int line_size;

  semid = semget( key, 1, 0 );
  printf("May take a while if there is another process accessing the file...\n\n");
  
  // update semaphore; prevent other processes from accessing file HERE DOWN
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;
  semop(semid, &sb, 1); // down semaphore

  // get last line size
  shmid = shmget( key, sizeof(int), 0644 );
  if (shmid < 0) { // fail if shm doesn't exist
    printf("error: %d - %s\n", errno, strerror(errno));
    exit(0);
  }

  memptr = (int *) shmat( shmid, 0, 0 );
  line_size = *memptr;

  // read last line
  int fd = open("story.txt", O_RDONLY);
  lseek(fd, line_size * -1, SEEK_END);
  char contents[line_size+1];
  read(fd, contents, line_size);
  contents[line_size] = 0;

  close(fd);
  printf("Old Line: %s\n", contents);
  
  // prompt user
  printf("New Line: ");
  char input[10000];
  fgets(input, sizeof(input), stdin);

  // write line to file
  fd = open("story.txt", O_WRONLY | O_APPEND);
  write(fd, input, strlen(input));
  close(fd);
  sb.sem_op = 1;
  semop(semid, &sb, 1); // up semaphore
  
  // update shared memory
  *memptr = strlen(input);
  
  return 0;
}
