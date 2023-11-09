#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "../../lab3/src/utils.h"

#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

struct MultyArgs {
  int *array;
  int begin;
  int end;
  int mod;
};

int Multy(const struct MultyArgs *args) {
  pthread_mutex_lock(&mut);
  int multy = 1;
  // TODO: your code here 
  for(int i=args->begin;i<args->end;i++ ) {
    if (args->array[i] % args->mod == 0) { 
        args->array[i] = 1;
    }
    multy *= args->array[i] % args->mod;
  }
  //sum = sum % args->mod;
  printf("Part fact  %d\n", multy);
  pthread_mutex_unlock(&mut);
  return multy;
}

void *ThreadSum(void *args) {
  struct MultyArgs *multy_args = (struct MultyArgs *)args;
  return (void *)(size_t)Multy(multy_args);
}

int main(int argc, char **argv) {
  /*
   *  TODO:
   *  threads_num by command line arguments
   *  array_size by command line arguments
   *	seed by command line arguments
   */

  uint32_t k = 0;
  uint32_t pnum = 0;
  uint32_t mod = 0;

  while (true)
  {
    int current_optind = optind ? optind : 1;
    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);
    if (c == -1) break;
    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            k = atoi(optarg);
            // your code here
            // error handling
            if (k <= 0) {
              printf("k is a positive number\n");
              return 1;
            }
            break;
          case 1:
            pnum = atoi(optarg);
            // your code here
            // error handling
            if (pnum <= 0) {
              printf("pnum is a positive number\n");
              return 1;
            }
            break;
          case 2:
            mod = atoi(optarg);
            // your code here
            // error handling
            if (mod <= 0) {
              printf("mod is a positive number\n");
              return 1;
            }
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
    
      
      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }
  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (k == 0 || pnum == 0 || mod == 0) {
    printf("Usage: %s -k num --pnum=num --mod=num \n",
           argv[0]);
    return 1;
  }


  pthread_t threads[pnum];

  int *array = malloc(sizeof(int) * (k));
  for(int i=1;i<k+1; i++){
    array[i-1] = i;
  }

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  struct MultyArgs args[pnum];
  for (uint32_t i = 0; i < pnum; i++) {
    args[i].array = array;
    args[i].begin = i * k / pnum;
    args[i].end = (i + 1) * k / pnum ;
    args[i].mod = mod;
    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args[i])) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  int total_multy = 1;
  for (uint32_t i = 0; i < pnum; i++) {
    int multy = 1;
    pthread_join(threads[i], (void **)&multy);
    total_multy *= multy;
  }
  if (total_multy % mod == 0) total_multy = 1;
  printf("Total fact %d\n", total_multy % mod);



  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;
  printf("Elapsed time: %fms\n", elapsed_time);

  return 0;
}
