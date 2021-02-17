#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "dom.h"
#include "funcs.h"



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

    int nq = 6;
    int nx = 1000;
    double xa = 0.0;
    double xb = 1.0;
    int maxCellsPerWorkUnit = 16;

    struct Domain dom = DOM_EMPTY;
    struct Domain dom1 = DOM_EMPTY;
    struct Domain dom2 = DOM_EMPTY;
    struct Domain dom3 = DOM_EMPTY;
    struct Domain dom4 = DOM_EMPTY;
    struct Domain dom_wu1 = DOM_EMPTY;

    printf("Making domain...\n");
    makeDomain(&dom, nx, nq, xa, xb, GRID_LIN, maxCellsPerWorkUnit);
    //printDomainShort(&dom);

    printf("Copying domains...\n");
    copyDomain(&dom, &dom1);
    copyDomain(&dom, &dom2);
    copyDomain(&dom, &dom3);
    copyDomain(&dom, &dom4);
    copyDomain(&dom, &dom_wu1);
    
    /*
    printDomainShort(&dom1);
    printDomainShort(&dom2);
    printDomainShort(&dom3);
    printDomainShort(&dom4);
    printDomainShort(&dom_wu1);
    */

    double tot = reduce(&dom);
    double tot1 = reduce(&dom1);
    double tot2 = reduce(&dom2);
    double tot3 = reduce(&dom3);
    double tot4 = reduce(&dom4);
    double tot_wu1 = reduce(&dom_wu1);
    printf("Total:  %.12le\n", tot);
    printf("Total1: %.12le\n", tot1);
    printf("Total2: %.12le\n", tot2);
    printf("Total3: %.12le\n", tot3);
    printf("Total4: %.12le\n", tot4);
    printf("Total_wu1: %.12le\n", tot_wu1);

    printf("Source...\n");
    
    double ta = omp_get_wtime();
    source0(&dom);
    double tb = omp_get_wtime();
    printf("  None:          %.3lf ms\n", 1e3*(tb-ta));

    
    ta = omp_get_wtime();
    source1(&dom1);
    tb = omp_get_wtime();
    printf("  Outer-Static:  %.3lf ms\n", 1e3*(tb-ta));
    
    ta = omp_get_wtime();
    source2(&dom2);
    tb = omp_get_wtime();
    printf("  Outer-Dynamic: %.3lf ms\n", 1e3*(tb-ta));
    
    ta = omp_get_wtime();
    source3(&dom3);
    tb = omp_get_wtime();
    printf("  Inner-Static:  %.3lf ms\n", 1e3*(tb-ta));
    
    ta = omp_get_wtime();
    source4(&dom4);
    tb = omp_get_wtime();
    printf("  Inner-Dynamic: %.3lf ms\n", 1e3*(tb-ta));
    
    ta = omp_get_wtime();
    source_wu1(&dom_wu1);
    tb = omp_get_wtime();
    printf("  Work-Unit:     %.3lf ms\n", 1e3*(tb-ta));

    tot = reduce(&dom);
    tot1 = reduce(&dom1);
    tot2 = reduce(&dom2);
    tot3 = reduce(&dom3);
    tot4 = reduce(&dom4);
    tot_wu1 = reduce(&dom_wu1);
    printf("Total:  %.12le\n", tot);
    printf("Total:  %.12le\n", tot1);
    printf("Total:  %.12le\n", tot2);
    printf("Total:  %.12le\n", tot3);
    printf("Total:  %.12le\n", tot4);
    printf("Total:  %.12le\n", tot_wu1);

    freeDomain(&dom);
    freeDomain(&dom1);
    freeDomain(&dom2);
    freeDomain(&dom3);
    freeDomain(&dom4);
    freeDomain(&dom_wu1);

    return 0;
}
