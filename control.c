#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>

 // declaration required on linux
union semun {  
  int val;
  struct semid_ds *buf;
  unsigned short *array; 
  struct seminfo *__buf;
};

int main(int argc, char *argv[]) {
  
  int shmid;
  int *line_size;
  int semid;
  int key = ftok( "control.c", 1337 );
  int sc;
  union semun su;

  // creates semaphore and set value to arg2
  if ( strcmp( argv[1], "-c" ) == 0 ) {
    // make shared memory; set to 0
    shmid = shmget( key, sizeof(int), IPC_CREAT | IPC_EXCL | 0644 );
    line_size = shmat( shmid, 0, 0 );
    *line_size = 0;
    
    // make semaphore
    semid = semget( key, 1, IPC_CREAT | IPC_EXCL | 0644 );
    if ( semid >= 0 ) {
      su.val = 1;
      sc = semctl( semid, 0, SETVAL, su );
    }
    else
      printf("semaphore already exists\n");

    // make file
    int fd = open("story.txt", O_CREAT | O_TRUNC | 0644);
    close(fd);
  }

  // prints out value of the semaphore
  else if ( strcmp( argv[1], "-v" ) == 0 ) {
    semid = semget( key, 1, 0 );
    sc = semctl( semid, 0, GETVAL );
    printf("semaphore value: %d\n", sc);
  }
  
  // removes the semaphore
  else if ( strcmp( argv[1], "-r" ) == 0 ) {
    semid = semget( key, 1, 0 );
    sc = semctl( semid, 0, IPC_RMID );
    printf("semaphore removed: %d\n", sc);
  }

  return 0;
}
