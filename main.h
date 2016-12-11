#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

//Automatically detect OS and 
//define semun accordingly
#if ! defined(__FreeBSD__) && ! defined(__OpenBSD__) && \
  ! defined(__sgi) && ! defined(__APPLE__)
union semun { 
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
};
#endif
