#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include "funcs.h"

void makeDomain(double ***domptr, int nx, int **nyptr, int nq, double x0)
{
    *nyptr = (int *)malloc(nx * sizeof(int));

    int i, j, q;

    for(i=0; i<nx; i++)
        (*nyptr)[i] = 6*(i+1);

    *domptr = (double **) malloc( nx * sizeof(double *));
    for(i=0; i<nx; i++)
    {
        (*domptr)[i] = (double *) malloc((*nyptr)[i] * nq * sizeof(double));

        double x = (i+0.5)/nx;

        for(j=0; j<(*nyptr)[i]; j++)
        {
            double y = 6 * (j+0.5)/(*nyptr)[i];
            for(q=0; q<nq; q++)
                (*domptr)[i][nq*j+q] = sqrt(x*x + y*y) + q;
        }
    }
}

void freeDomain(double **dom, int nx, int *ny)
{
    int i;
    for(i=0; i<nx; i++)
    {
        free(dom[i]);
        dom[i] = NULL;
    }
    free(dom);
    dom = NULL;
    free(ny);
    ny = NULL;
}

void printDomain(double **dom, int nx, int *ny, int nq)
{
    int i, j, q;
    for(i=0; i<nx; i++)
    {
        printf("%03d:", i);
        for(j=0; j<ny[i]; j++)
        {
            printf(" (%.1lf", dom[i][nq*j]);
            for(q=1; q<nq; q++)
                printf(",%.1lf", dom[i][nq*j+q]);
            printf(")");
        }
        printf("\n");
    }
}


int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Please give me a number of threads.\n");
        return 0;
    }
    
    int oldthreads = omp_get_max_threads();
    int nprocs = omp_get_num_procs();

    printf("Was set to use %d threads\n", oldthreads);
    printf("Have %d processors\n", nprocs);

    int nthreads = (int) strtol(argv[1], NULL, 10);

    omp_set_num_threads(nthreads);

    printf("Now using %d threads\n", nthreads);


    #pragma omp parallel
    printf("Hello world!\n");

    int nq = 5;
    int nx = 1000;
    double **dom;
    int *ny;

    makeDomain(&dom, nx, &ny, nq, x0);
    //printDomain(dom, nx, ny, nq);

    double tot = reduce(dom, nx, ny, nq);
    printf("Total: %.12le\n", tot);

    printf("Source...\n");
    
    double ta = omp_get_wtime();
    source0(dom, nx, ny, nq);
    double tb = omp_get_wtime();
    printf("  None:          %.3lf ms\n", 1e3*(tb-ta));
    
    ta = omp_get_wtime();
    source1(dom, nx, ny, nq);
    tb = omp_get_wtime();
    printf("  Outer-Static:  %.3lf ms\n", 1e3*(tb-ta));
    
    ta = omp_get_wtime();
    source2(dom, nx, ny, nq);
    tb = omp_get_wtime();
    printf("  Outer-Dynamic: %.3lf ms\n", 1e3*(tb-ta));
    
    ta = omp_get_wtime();
    source3(dom, nx, ny, nq);
    tb = omp_get_wtime();
    printf("  Inner-Static:  %.3lf ms\n", 1e3*(tb-ta));
    
    ta = omp_get_wtime();
    source4(dom, nx, ny, nq);
    tb = omp_get_wtime();
    printf("  Inner-Dynamic: %.3lf ms\n", 1e3*(tb-ta));
    
    plm(dom, nx, ny, nq);
    flux(dom, nx, ny, nq);

    tot = reduce(dom, nx, ny, nq);
    printf("Total: %.12le\n", tot);

    freeDomain(dom, nx, ny);

    return 0;
}
