#include <time.h>

#include "API.h"
#include "pblc_data.ac"
#include "prvt_edward.ac"

#define VERBOSE

#define BENCHMARK_SAMPLE_SIZE 10

#ifdef __i386
extern __inline__ uint64_t rdtsc(void) {
  uint64_t x;
  __asm__ volatile("rdtsc" : "=A"(x));
  return x;
}
#elif defined __amd64
extern __inline__ uint64_t rdtsc(void) {
  uint64_t a, d;
  __asm__ volatile("rdtsc" : "=a"(a), "=d"(d));
  return (d << 32) | a;
}
#endif

int double_compare (const void * a, const void * b)
{
  double da = *(double*) a;
  double db = *(double*) b;
  return (da > db) - (da < db);
}

int llong_compare (const void * a, const void * b)
{
  long long la = *(long long*) a;
  long long lb = *(long long*) b;
  return (la > lb) - (la < lb);
}

int main() {
  anonymous_credential_start();
  int i;
  uint8_t pres[DEMO_PRES_BYTELENGTH];
  uint8_t msg_in[SIZE_MESS_S8];
  uint8_t msg_out[SIZE_MESS_S8];
  time_t tmp = time(NULL);
  unsigned long timestamp = difftime(tmp, 0);
  msg_in[0] = timestamp & 0xff;
  msg_in[1] = timestamp >> 8 & 0xff;
  msg_in[2] = timestamp >> 16 & 0xff;
  msg_in[3] = timestamp >> 24 & 0xff;
  msg_in[4] = timestamp >> 32 & 0xff;
  msg_in[5] = timestamp >> 40 & 0xff;
  msg_in[6] = timestamp >> 48 & 0xff;
  for (i = 7; i < SIZE_MESS_S8; i++) {
    msg_in[i] = 0;
  }
  uint64_t start_rdtsc, end_rdtsc;
  clock_t start_time, end_time;
  long long cpu_cycles_used;
  long long cycles_used[BENCHMARK_SAMPLE_SIZE];
  long long median_cycles;
  double cpu_time_used;
  double time_used[BENCHMARK_SAMPLE_SIZE];
  double median_time;

  long long sample_cycles_mean = 0;
  double sample_time_mean = 0;
  printf("%d calls to get_presentation_token()...\n",
         BENCHMARK_SAMPLE_SIZE);
  for (int j = 0; j < BENCHMARK_SAMPLE_SIZE; j++) {
    start_time = clock();
    start_rdtsc = rdtsc();
    get_presentation_token(pres, demo_cred_edward, msg_in);
    end_rdtsc = rdtsc();
    end_time = clock();
    cpu_cycles_used = (long long)end_rdtsc - (long long)start_rdtsc;
    cpu_time_used =
        ((double)end_time - (double)start_time) / (double) CLOCKS_PER_SEC;
    sample_cycles_mean += cpu_cycles_used;
    sample_time_mean += cpu_time_used;
    time_used[j] = cpu_time_used;
    cycles_used[j] = cpu_cycles_used;
#ifdef VERBOSE
    printf("%2d. ", j + 1);
    printf("CPU cycles: %11llu. ", cpu_cycles_used);
    printf("Time: %f seconds.\n", cpu_time_used);
#endif
  }
  sample_cycles_mean /= BENCHMARK_SAMPLE_SIZE;
  sample_time_mean /= BENCHMARK_SAMPLE_SIZE;
  printf("> Mean CPU time used: %f seconds (%llu cycles).\n", sample_time_mean, sample_cycles_mean);
  qsort(time_used, BENCHMARK_SAMPLE_SIZE, sizeof(double), double_compare);
  qsort(cycles_used, BENCHMARK_SAMPLE_SIZE, sizeof(long long), llong_compare);
  if(BENCHMARK_SAMPLE_SIZE%2==0) { 
      median_time = (time_used[BENCHMARK_SAMPLE_SIZE/2]+time_used[BENCHMARK_SAMPLE_SIZE/2-1])/2;  
      median_cycles = (cycles_used[BENCHMARK_SAMPLE_SIZE/2]+cycles_used[BENCHMARK_SAMPLE_SIZE/2-1])/2;  
  } else {  
      median_time = time_used[BENCHMARK_SAMPLE_SIZE/2];
      median_cycles = cycles_used[BENCHMARK_SAMPLE_SIZE/2];
  }
  printf("> Median CPU time used: %f seconds (%llu cycles).\n", median_time, median_cycles);
  printf("> Min CPU time used: %f seconds (%llu cycles).\n", time_used[0], cycles_used[0]);
  for (i = 0; i < SIZE_MESS_S8; i++) {
    msg_out[i] = pres[i];
  }
  for (i = 0; i < SIZE_MESS_S8; i++) {
    if (msg_out[i] != msg_in[i]) {
      return EXIT_FAILURE;
    }
  }

  sample_cycles_mean = 0;
  sample_time_mean = 0;
  printf("\n%d calls to verify_presentation_token()...\n",
         BENCHMARK_SAMPLE_SIZE);
  for (int j = 0; j < BENCHMARK_SAMPLE_SIZE; j++) {
    start_time = clock();
    start_rdtsc = rdtsc();
    verify_presentation_token(pres, demo_public_keys);
    end_rdtsc = rdtsc();
    end_time = clock();
    cpu_cycles_used = (long long)end_rdtsc - (long long)start_rdtsc;
    cpu_time_used =
        ((double)end_time - (double)start_time) / (double) CLOCKS_PER_SEC;
    sample_cycles_mean += cpu_cycles_used;
    sample_time_mean += cpu_time_used;
    time_used[j] = cpu_time_used;
    cycles_used[j] = cpu_cycles_used;
#ifdef VERBOSE
    printf("%2d. ", j + 1);
    printf("CPU cycles: %11llu. ", cpu_cycles_used);
    printf("Time: %f seconds.\n", cpu_time_used);
#endif
  }
  sample_cycles_mean /= BENCHMARK_SAMPLE_SIZE;
  sample_time_mean /= BENCHMARK_SAMPLE_SIZE;
  printf("> Mean CPU time used: %f seconds (%llu cycles).\n", sample_time_mean, sample_cycles_mean);
  qsort(time_used, BENCHMARK_SAMPLE_SIZE, sizeof(double), double_compare);
  qsort(cycles_used, BENCHMARK_SAMPLE_SIZE, sizeof(long long), llong_compare);
  if(BENCHMARK_SAMPLE_SIZE%2==0) { 
      median_time = (time_used[BENCHMARK_SAMPLE_SIZE/2]+time_used[BENCHMARK_SAMPLE_SIZE/2-1])/2;  
      median_cycles = (cycles_used[BENCHMARK_SAMPLE_SIZE/2]+cycles_used[BENCHMARK_SAMPLE_SIZE/2-1])/2;  
  } else {  
      median_time = time_used[BENCHMARK_SAMPLE_SIZE/2];
      median_cycles = cycles_used[BENCHMARK_SAMPLE_SIZE/2];
  }
  printf("> Median CPU time used: %f seconds (%llu cycles).\n", median_time, median_cycles);
  printf("> Min CPU time used: %f seconds (%llu cycles).\n", time_used[0], cycles_used[0]);
  anonymous_credential_stop();
  return EXIT_SUCCESS;
}
