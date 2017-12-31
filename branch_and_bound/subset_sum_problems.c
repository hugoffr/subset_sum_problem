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

#include "subset_sum_problems.h"


//
// Measure elapsed time (on GNU/Linux)
//

double elapsed_time(void)
{
    static struct timespec last_time,current_time;

    last_time = current_time;
    if(clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&current_time) != 0)  // the first argument could also be CLOCK_REALTIME
        return -1.0; // clock_get_time() failed!!!
    return ((double)current_time.tv_sec - (double)last_time.tv_sec) +
           1.0e-9 * ((double)current_time.tv_nsec - (double)last_time.tv_nsec);
}

int * bnb(int n, const u64 a[], u64 d) //n=size of set || a(n)=set || d=goal || b(n) binary array || aux=index counter
{
    u64 sum = 0ull;
    int * b = malloc(n*sizeof(int));
    int i;
    for(i = 0; i<n;i++) b[i] =  0;

    int aux = 0; //aux=0 normal || aux=1 tests!!!!
    b[aux] = 1;
    while (1)
    {
        if (aux <= n && b[aux] == 1)
        {
            if (sum + a[aux] == d)
            {
                return b;
            }
            else if(sum + a[aux] < d)  sum += a[aux];
            else  b[aux] = 0;
        }
        else
        {
            aux--;
            while (aux > 0 && b[aux] == 0)
            {
                aux--;
                if (aux <= 0) break;
            }
            b[aux] = 0;
            sum = sum - a[aux];
        }
        aux++;
        b[aux] = 1;
    }
}

void solve_using_mim_strategy(subset_sum_problem_t *problem)
{
    // the solution
    double dt;
    int i,j;
    u64 sum;

    printf("solutions for n = %d\n",problem->n);
    // Solve all problems for the value of n; place the solution in the array b
    for(i = 0; i < n_sums; i++)
    {
        (void)elapsed_time();
        int * b = bnb(problem->n,problem->a,problem->sums[i]);

        dt = elapsed_time();

        sum = 0ull;
        for(j = 0; j < problem->n; j++)
            if(b[j] != 0)
                sum += problem->a[j];
        assert(sum == problem->sums[i]);
        printf("  solved [");
        for(j = problem->n - 1; j >= 0; j--)
            printf("%d",b[j]);
        printf("] in %.09fs\n",dt);
        free(b);
    }
}

int main(void)
{

    int i;


    for(i = 0; i < n_problems; i++)
        solve_using_mim_strategy(&subset_sum_problems[i]);
    return 0;
}

