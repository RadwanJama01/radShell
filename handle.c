#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include "util.h"


/*
 * First, print out the process ID of this process.
 *
 * Then, set up the signal handler so that ^C causes
 * the program to print "Nice try.\n" and continue looping.
 *
 * Finally, loop forever, printing "Still here\n" once every
 * second.
 */
void handle_sigint(int sig){
  write(STDOUT_FILENO,"nice try.\n",10);

}
int main(int argc, char **argv)
{
  printf("PID: %d\n",getpid());
  //we need to create a signal handler that makes control C makes the program say nice try
  struct sigaction sa;
  sa.sa_handler = &handle_sigint;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);
  struct timespec req, rem;
  if (sigaction(SIGINT,&sa,NULL)==-1){
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  req.tv_sec = 1;
  req.tv_nsec = 0;
  while(1){
    printf("still here\n");
    while(nanosleep(&req,&rem)==-1 && errno == EINTR){
      req = rem;
    }
    req.tv_sec = 1;
    req.tv_nsec = 0;
  }



  
}


