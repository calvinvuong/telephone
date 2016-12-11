#include "main.h"

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
