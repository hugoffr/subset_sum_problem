//
// AED, October-November 2017
//
// STUDENT NAME Hugo Fernando Ferreira de Resende
// STUDENT NAME Francisco Manuel Ferreira Resende
// ...
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


//
// Measure elapsed time (on GNU/Linux)
//

double elapsed_time(void)
{
    static struct timespec last_time,current_time;

    last_time = current_time;
    if(clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&current_time) != 0)  // the first argument could also be CLOCK_REALTIME
        return -1.0; // clock_gettime() failed!!!
    return ((double)current_time.tv_sec - (double)last_time.tv_sec) +
           1.0e-9 * ((double)current_time.tv_nsec - (double)last_time.tv_nsec);
}

static int f(const void *a, const void *b)
{
	
	if ( *(u64*)a <  *(u64*)b) return -1;
	if ( *(u64*)a== *(u64*)b) return 0;
	if ( *(u64*)a >  *(u64*)b) return 1;
}


u64 ** sums(unsigned int n,u64 a[n])
{
    unsigned int i,mask;
    u64 s;
    assert(n <= 30);
    mask = 1;

	u64 ** ans = malloc((1<<n)*sizeof(u64*)); //u64 ans[1<<n][2];
	ans[0] = malloc(2*(1<<n) * sizeof(u64));
	for(i = 1; i < (1<<n); i++){
	ans[i] = ans[0] + i*2;
	}
	
    do
        {
            s = 0ull;
            for(i = 0; i < n; i++)
                if(((mask >> i) & 1) != 0)
                    {
                        s += a[i];
                    }
            ans[mask-1][0] = s;
			ans[mask-1][1] = (u64) mask;
			mask++;
        }
    while(mask < (1 << n));
	qsort(ans[0],(1<<n), 2*sizeof(u64),f);
	return ans;
}

int * mim(int ln1,u64 ** a1,int ln2,u64 ** a2,u64 s)   // meet in the middle srategy u64 a2[1<<ln2][2]
{
	int k;
    int i = 0;
    int j = (1<<ln2) - 1;
    u64 mask = 1;
	while(i < 1<<ln1)
	{
		while(j >= 0)
		{
			if(a1[i][0] + a2[j][0] > s)
			{
				j--;
			}
			else if( a1[i][0] + a2[j][0] < s)
			{
				i++;
			}
			else if(a1[i][0]+ a2[j][0] == s)
			{
				int * ans = malloc((ln1+ln2)*sizeof(int));//int ans[ln1 + ln2];
				for(k = 0;k<ln1; k++) ans[k] = ((mask & a1[i][1]>>k) != 0 );
				for(k = 0;k<ln2; k++) ans[k+ln1] = ((mask & a2[j][1]>>k) != 0 );
				return ans;
			}
		}
	}
}

void solve_using_mim_strategy(subset_sum_problem_t *problem)
{
    int b[max_n]; // the solution
    double dt;
    int i,j;
    u64 sum;

    printf("solutions for n = %d\n",problem->n);
    // pre-processing
    (void)elapsed_time();

    unsigned int ln1 = (problem->n)/2;
    unsigned int ln2 = problem->n-((problem->n)/2);

	u64 a1[ln1], a2[ln2];

	for(i = 0;i<ln1;i++) a1[i]=problem->a[i];
	for(i = 0;i<ln2;i++) a2[i]=problem->a[i+ln1];
	u64 ** s1 = sums(ln1,a1);
	u64 ** s2 = sums(ln2,a2);
    dt = elapsed_time();

    printf("  preprocessing done in %.03fs\n",dt);
    // Solve all problems for the value of n; place the solution in the array b
    for(i = 0; i < n_sums; i++)
	{
            (void)elapsed_time();
			int * b =  mim(ln1, s1,ln2,s2,problem->sums[i]);
			
            dt = elapsed_time();

            sum = 0ull;
            for(j = 0; j < problem->n; j++)
                if(b[j] != 0)
                    sum += problem->a[j];
            assert(sum == problem->sums[i]);
            printf("  solved [");
            for(j = problem->n - 1; j >= 0; j--)
                printf("%d",b[j]);
            printf("] in %.03fs\n",dt);
	}
    // cleanup code
	free(s1[0]);
	free(s1);
	free(s2[0]);
	free(s2);
}

int main(void)
{
    int i;
    for(i = 0; i < n_problems; i++)
        solve_using_mim_strategy(&subset_sum_problems[i]);
    return 0;
}