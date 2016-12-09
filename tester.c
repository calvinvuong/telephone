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

union semun {  
  int val;
  struct semid_ds *buf;
  unsigned short *array; 
  struct seminfo *__buf;
};


int main() {

  int shmid;
  int *shmval;
  int semid;
  int key = ftok("control.c", 1337);
  union semun su;

  shmid = shmget( key, sizeof(int), 0 );
  shmval = (int *) shmat( shmid, 0, 0 );

  printf("shared memory value: %d\n", *shmval);
  
  semid = semget( key, 1, 0 );
  printf("semaphore value: %d\n", semctl(semid, 0, GETVAL));

  return 0;
}
