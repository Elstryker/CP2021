#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

#include "papi.h"
#include "radixSort.h"
#include "bucketSort.h"
#include <time.h>

static int verify_command_line (int argc, char *argv[], int *m_size, int *version);
static void print_usage (char *msg);
static int alloc_matrix (int **m, int m_size);
static int free_matrix (int **m);


void (*func)(int args, char *argv[] );

// PAPI events to monitor
#define NUM_EVENTS 6
int Events[NUM_EVENTS] = { PAPI_TOT_CYC, PAPI_TOT_INS, PAPI_VEC_SP, PAPI_L1_DCM};
// PAPI counters' values
long long values[NUM_EVENTS], min_values[NUM_EVENTS];
int retval, EventSet=PAPI_NULL;

// number of times the function is executed and measured
#define NUM_RUNS 5
    int not_main(int argc, char const *argv[])
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

        printf("Original array: ");
        print(numbers,size);
        // int numbers[] = {123, 8, 40, 67, 298, 76, 8, 93, 30, 77, 43, 345, 157, 187, 265, 3, 36, 300, 40, 50, 86};

        bucketSort(numbers,size,buckNum);

        printf("Sorted Array: ");
        print(numbers,size);

        return 0;
    }

int main (int argc, char *argv[]) {
    long long start_usec, end_usec, elapsed_usec, min_usec=0L;
    int m_size, total_elements, version, i, run;
    int num_hwcntrs = 0;
    int *a;
    char ** args;

    if (!verify_command_line (argc, argv, &m_size, &version)) {
        return 0;
    }
    total_elements = m_size;
    
    fprintf (stdout, "Square matrices have %d rows for a total of %d elements!\n", m_size, total_elements);
    
    fprintf (stdout, "\nSetting up PAPI...");
    // Initialize PAPI
    retval = PAPI_library_init(PAPI_VER_CURRENT);
    if (retval != PAPI_VER_CURRENT) {
        fprintf(stderr,"PAPI library init error!\n");
        return 0;
    }
    
    /* create event set */
    if (PAPI_create_eventset(&EventSet) != PAPI_OK) {
        fprintf(stderr,"PAPI create event set error\n");
        return 0;
    }
    
    /* Get the number of hardware counters available */
    if ((num_hwcntrs = PAPI_num_hwctrs()) <= PAPI_OK)  {
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
    
    if (PAPI_add_events(EventSet,Events,NUM_EVENTS) != PAPI_OK)  {
        fprintf(stderr,"PAPI library add events error!\n");
        return 0;
    }

    // ini A matrix
    fprintf (stdout, "Initializing Matrix A...");
    if (!alloc_matrix (&a, total_elements)) return 0;
    fprintf (stdout, "done!\n");
    
    
    
    // warmup caches
    fprintf (stdout, "Warming up caches...");
    func (total_elements, argv);
    fprintf (stdout, "done!\n");
    
    for (run=0 ; run < NUM_RUNS ; run++) {
        fprintf (stdout, "run=%d - Computing C = A * B...", run);
        
        // use PAPI timer (usecs) - note that this is wall clock time
        // for process time running in user mode -> PAPI_get_virt_usec()
        // real and virtual clock cycles can also be read using the equivalent
        // PAPI_get[real|virt]_cyc()
        start_usec = PAPI_get_real_usec();
        
        /* Start counting events */
        if (PAPI_start(EventSet) != PAPI_OK) {
            fprintf (stderr, "PAPI error starting counters!\n");
            return 0;
        }
        
        func (total_elements, a);
        
        /* Stop counting events */
        if (PAPI_stop(EventSet,values) != PAPI_OK) {
            fprintf (stderr, "PAPI error stoping counters!\n");
            return 0;
        }
        
        end_usec = PAPI_get_real_usec();
        fprintf (stdout, "done!\n");
        
        elapsed_usec = end_usec - start_usec;
        
        if ((run==0) || (elapsed_usec < min_usec)) {
            min_usec = elapsed_usec;
            for (i=0 ; i< NUM_EVENTS ; i++) min_values[i] = values [i];
        }
        
    } // end runs
    fprintf (stdout,"\nWall clock time: %lld usecs\n", min_usec);
    
    // output PAPI counters' values
    for (i=0 ; i< NUM_EVENTS ; i++) {
        char EventCodeStr[PAPI_MAX_STR_LEN];
        
        if (PAPI_event_code_to_name(Events[i], EventCodeStr) == PAPI_OK) {
            fprintf (stdout, "%s = %lld\n", EventCodeStr, min_values[i]);
        } else {
            fprintf (stdout, "PAPI UNKNOWN EVENT = %lld\n", min_values[i]);
        }
    }
    
    free_matrix (&a);
   
    fprintf (stdout,"\nThat's all, folks\n");
    return 0;
}

int verify_command_line (int argc, char *argv[], int *total_elements, int *version) {
    int val;
    

    val = atoi (argv[1]);
    
    if (val <= 0) {
        print_usage ((char *)"The array size must be a positive integer!");
        return 0;
    }
    else {
        *total_elements = argc;
    }
    
  func = &not_main;
    
    return 1;
}

void print_usage (char *msg) {
    fprintf (stderr, "Command Line Error! %s\n", msg);
    fprintf (stderr, "Usage:\tgemm <matrix size> <version>\n\n");
}


int alloc_matrix (int **m, int N) {
    
    *m = (int *) malloc (N*sizeof(int));
    if (!(*m)) {
        print_usage ((char *)"Could not allocate memory for matrix!");
        return 0;
    }
    return 1;
}

int free_matrix (int **m) {
    free (*m);
    *m = NULL;
    return 1;
}



