#include "main.h"

void print_file() {
  int fd = open("story.txt", O_RDONLY);
    
  // get file size
  struct stat buf;
  stat("story.txt", &buf);
  int size = buf.st_size;

  // read file conents
  char contents[size+1];
  read(fd, contents, size);
  contents[size] = 0; // null terminate

  // print contents
  printf("%s", contents); //already a \n at the end of story
}


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
    shmid = shmget( key, sizeof(int), IPC_CREAT | 0644 );
    line_size = (int *) shmat( shmid, 0, 0 );
    *line_size = 0;
    
    // make semaphore
    semid = semget( key, 1, IPC_CREAT | 0644 );
    
    su.val = 1;
    semctl( semid, 0, SETVAL, su );
    
    // make file
    int fd = open("story.txt", O_CREAT | O_TRUNC, 0644);
    close(fd);
  }

  // prints out contents of story
  else if ( strcmp( argv[1], "-v" ) == 0 ) {
    print_file();
  }
  
  // removes the memory, semaphore; print file
  else if ( strcmp( argv[1], "-r" ) == 0 ) {
    // remove memory
    shmid = shmget( key, sizeof(int), 0);
    shmctl( shmid, IPC_RMID, 0 );
    
    // remove semaphore
    semid = semget( key, 1, 0 );
    semctl( semid, 0, IPC_RMID );

    print_file();
  } 

  else {
    printf("Error: not a valid flag! Use -c for creating, -v for viewing, or -r for removing!");
    exit(0);
  }

  return 0;
}
