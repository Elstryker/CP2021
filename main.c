#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "papi.h"
 

#include "bucketSort_parallel.h"
#include <time.h>





void (*func)(int args, char *argv[] );

#define NUM_EVENTS 4
int Events[NUM_EVENTS] = { PAPI_TOT_INS, PAPI_L1_DCM, PAPI_L2_DCM, PAPI_L3_TCM};

// PAPI counters' values
long long values[NUM_EVENTS], min_values[NUM_EVENTS];

// number of times the function is executed and measured
#define NUM_RUNS 1

int main_buckets(int argc, char *argv[])
{
    int size;
    int buckNum;
    if(argc != 2 && argc != 3) {
        size = 100;
        buckNum = 10;
    }
    else if(argc == 3) {
        size = atoi(argv[1]);
        buckNum = atoi(argv[2]);
    }
    else {
        size = atoi(argv[1]);
        buckNum = 10;
    }

    int numbers[size];

    srand(time(NULL));

    
    for(int i = 0; i < size; i++) {
        int r = rand() % 10000;
        numbers[i] = r;
    }

    // print(numbers,size);

    bucketSort(numbers,size,buckNum);

    // printf("Sorted Array: ");
    // print(numbers,size);

    return 0;
}

int main (int argc, char *argv[]) {
  long long start_usec, end_usec, elapsed_usec, min_usec=0L;
  int  i, run;
  //int *a;
  int num_hwcntrs = 0;

  /*if (!verify_command_line (argc, argv, &m_size, &version)) {
	return 0;
  }
  total_elements = m_size;*/


  fprintf (stdout, "\nSetting up PAPI...");
  // Initialize PAPI 
  PAPI_library_init (PAPI_VER_CURRENT);

  /* Get the number of hardware counters available */
  if ((num_hwcntrs = PAPI_num_counters()) <= PAPI_OK)  {
    fprintf (stderr, "PAPI error getting number of available hardware counters!\n");
    return 0;
  }
  fprintf(stdout, "done!\nThis system has %d available counters.\n\n", num_hwcntrs);

  // We will be using at most NUM_EVENTS counters
  if (num_hwcntrs >= NUM_EVENTS) {
    num_hwcntrs = NUM_EVENTS;
  } else {
    fprintf (stderr, "Error: there aren't enough counters to monitor %d events!\n", NUM_EVENTS);
    return 0;
  }


 


  // warmup caches
  fprintf (stdout, "Warming up caches...");
  main_buckets(argc,argv);
  fprintf (stdout, "done!\n");

  for (run=0 ; run < NUM_RUNS ; run++) { 
   //fprintf (stderr, "\nrun=%d - Computing C = A * B...", run);

   // use PAPI timer (usecs) - note that this is wall clock time
   // for process time running in user mode -> PAPI_get_virt_usec()
   // real and virtual clock cycles can also be read using the equivalent
   // PAPI_get[real|virt]_cyc()
   start_usec = PAPI_get_real_usec();

   /* Start counting events */
   if (PAPI_start_counters(Events, num_hwcntrs) != PAPI_OK) {
     fprintf (stderr, "PAPI error starting counters!\n");
     return 0;
   }

   main_buckets(argc, argv);

   /* Stop counting events */
   if (PAPI_stop_counters(values, NUM_EVENTS) != PAPI_OK) {
     fprintf (stderr, "PAPI error stoping counters!\n");
     return 0;
   }

   end_usec = PAPI_get_real_usec();
   fprintf (stderr, "done!\n");

   elapsed_usec = end_usec - start_usec;

   if ((run==0) || (elapsed_usec < min_usec)) {
      min_usec = elapsed_usec;
      for (i=0 ; i< NUM_EVENTS ; i++) min_values[i] = values [i];
   }

  } // end runs
  printf ("\nWall clock time: %lld usecs\n", min_usec);
  
  // output PAPI counters' values
  for (i=0 ; i< NUM_EVENTS ; i++) {
	  char EventCodeStr[PAPI_MAX_STR_LEN];

	  if (PAPI_event_code_to_name(Events[i], EventCodeStr) == PAPI_OK) {
		fprintf (stdout, "%s = %lld\n", EventCodeStr, min_values[i]);
	  } else {
		fprintf (stdout, "PAPI UNKNOWN EVENT = %lld\n", min_values[i]);
	  }
  }

#if NUM_EVENTS >1
  // evaluate CPI and Texec here
  if ((Events[0] == PAPI_TOT_CYC) && (Events[1] == PAPI_TOT_INS)) {
    float CPI = ((float) min_values[0]) / ((float) min_values[1]);
    fprintf (stdout, "CPI = %.2f\n", CPI);
  }
#endif
  

  printf ("\nThat's all, folks\n");
  return 0;
}





