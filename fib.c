#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

const int MAX = 13;

static void doFib (int n, int doPrint);

/*
 * unix_error - unix-style error routine.
 */
inline static void unix_error (char *msg)
{
  fprintf (stdout, "%s: %s\n", msg, strerror (errno));
  exit (1);
}

int main (int argc, char **argv)
{
  int arg;
  int print = 1;

  if (argc != 2)
    {
      fprintf (stderr, "Usage: fib <num>\n");
      exit (-1);
    }

  arg = atoi (argv[1]);
  if (arg < 0 || arg > MAX)
    {
      fprintf (stderr, "number must be between 0 and %d\n", MAX);
      exit (-1);
    }

  doFib (arg, print);


  return 0;
}

/*
 * Recursively compute the specified number. If print is
 * true, print it. Otherwise, provide it to my parent process.
 *
 * NOTE: The solution must be recursive and it must fork
 * a new child for each call. Each process should call
 * doFib() exactly once.
 */
static void doFib (int n, int doPrint) {
    if(n==0 || n==1){
      if(doPrint){
        printf("%d\n", n);
      }
      exit(n); //this is our fib number
    } 
    printf("PID %d computing fib(%d)\n", getpid(), n);

    pid_t left = fork();
    if (left<0){
      perror("fork failed");
      exit(1);
    }else if(left==0){
      doFib(n-1,0);
    }

    pid_t right = fork();
    if(right<0){
      perror("fork failed");
    } else if(right ==0){
      doFib(n-2,0);
    }

    int status1,status2;
    waitpid(left,&status1,0);
    waitpid(right,&status2,0);

    int val1 = WEXITSTATUS(status1);
    int val2 = WEXITSTATUS(status2);

    int res = val1+val2;
    if (doPrint){
      printf("%d\n",res);
    }
    exit(res);





}
