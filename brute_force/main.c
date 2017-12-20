//
// AED, October-November 2017
//
// STUDENT NAME Hugo Fernando Ferreira de Resende
// STUDENT NAME Francisco Manuel Ferreira Resende
//
// Solution of the subset sum problems
//
// Compile with the command "cc -Wall -O2 -march=native subset_sum_problems.c -lm"
//

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include "subset_sum_problems.h"

double elapsed_time(void)
{
  static struct timespec last_time,current_time;

  last_time = current_time;
  if(clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&current_time) != 0)  // the first argument could also be CLOCK_REALTIME
    return -1.0; // clock_gettime() failed!!!
  return ((double)current_time.tv_sec - (double)last_time.tv_sec) +
         1.0e-9 * ((double)current_time.tv_nsec - (double)last_time.tv_nsec);
}

int * bf(unsigned int n,u64 a[n],u64 sum)
{
    unsigned int i,mask;
    u64 s;
    assert(n <= 56);
    mask = 1;
	int * ans = malloc((1<<n)*sizeof(int));
    do
        {
            s = 0ull;
            for(i = 0; i < n; i++)
			{
                if(((mask >> i) & 1) != 0)
                    {
                        s += a[i];
                    }
			}
			if(s == sum) 
			{
				for(i = 0; i < n; i++) 
				{
					ans[i] = ((mask>>i)&1);
				}
				return ans;
			}
			mask++;
        }
    while(mask < (1 << n));
}

void solve_using_bf_strategy(subset_sum_problem_t *problem)
{
  double dt;
  int i,j;
  u64 sum;

  printf("solutions for n = %d\n",problem->n);

  for(i = 0;i < n_sums;i++)
  {
    (void)elapsed_time();
	int * b = bf(problem->n,problem->a,problem->sums[i]);
    dt = elapsed_time();
    sum = 0ull;
    for(j = 0;j < problem->n;j++)
      if(b[j] != 0)
        sum += problem->a[j];
    assert(sum == problem->sums[i]);
    printf("  solved [");
    for(j = problem->n - 1;j >= 0;j--)
      printf("%d",b[j]);
    printf("] in %.03fs\n",dt);
  }
}

int main(void)
{
  int i;

  for(i = 0;i < n_problems;i++)
    solve_using_bf_strategy(&subset_sum_problems[i]);
  return 0;
}