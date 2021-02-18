#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "dom.h"
#include "funcs.h"


double runAnalysisAll(struct Domain *dom, void (*func)(struct Domain *),
                      char filename[])
{
    int maxthreads = omp_get_max_threads();

    double times[maxthreads];

    int niter = 10;
    int i;

    int ncpu;
    for(ncpu=1; ncpu <= maxthreads; ncpu++)
    {
        omp_set_num_threads(ncpu);
        double ta = omp_get_wtime();
        for(i=0; i<niter; i++)
            func(dom);
        double tb = omp_get_wtime();
        times[ncpu] = (tb-ta) / niter;
    }

    printf("   Writing %s\n", filename);

    FILE *f = fopen(filename, "w");
    for(ncpu=1; ncpu <= maxthreads; ncpu++)
        fprintf(f, "%02d  %.8le\n", ncpu, times[ncpu]);
    fclose(f);
    
    double tot = reduce(dom);

    return tot;
}


double runAnalysisSingle(struct Domain *dom, void (*func)(struct Domain *),
                         char label[], int nthreads)
{
    int i;
    int niter = 10;

    omp_set_num_threads(nthreads);
    double ta = omp_get_wtime();
    for(i=0; i<niter; i++)
        func(dom);
    double tb = omp_get_wtime();
    double time = (tb-ta) / niter;

    printf("   %s: %.6lf ms\n", label, time*1000);
    
    double tot = reduce(dom);

    return tot;
}


double runAnalysis(struct Domain *dom, void (*func)(struct Domain *),
                   char filename[], int nthreads)
{
    
    double tot;

    if(nthreads > 0)
        tot = runAnalysisSingle(dom, func, filename, nthreads);
    else
        tot = runAnalysisAll(dom, func, filename);

    return tot;
}


int main(int argc, char *argv[])
{
    int nthreads = 0;
    if(argc > 1)
        nthreads = (int) strtol(argv[1], NULL, 10);

    int nq = 6;
    int nx = 1000;
    double xa = 0.0;
    double xb = 1.0;

    struct Domain dom = DOM_EMPTY;
    struct Domain dom_od = DOM_EMPTY;
    struct Domain dom_is = DOM_EMPTY;
    struct Domain dom_wu08 = DOM_EMPTY;
    struct Domain dom_wu16 = DOM_EMPTY;
    struct Domain dom_wu32 = DOM_EMPTY;
    struct Domain dom_wu64 = DOM_EMPTY;

    printf("Making domains...\n");
    makeDomain(&dom, nx, nq, xa, xb, GRID_LIN, 16);
    makeDomain(&dom_od, nx, nq, xa, xb, GRID_LIN, 16);
    makeDomain(&dom_is, nx, nq, xa, xb, GRID_LIN, 16);
    makeDomain(&dom_wu08, nx, nq, xa, xb, GRID_LIN, 16);
    makeDomain(&dom_wu16, nx, nq, xa, xb, GRID_LIN, 16);
    makeDomain(&dom_wu32, nx, nq, xa, xb, GRID_LIN, 32);
    makeDomain(&dom_wu64, nx, nq, xa, xb, GRID_LIN, 64);

    double tot, tot_od, tot_is, tot_wu08, tot_wu16, tot_wu32, tot_wu64;

    printf("Running Analysis...\n");
    if(nthreads > 0)
    {
        tot = runAnalysis(&dom, source0,
                                 "serial", nthreads);
        tot_od = runAnalysis(&dom_od, source2,
                                    "outer", nthreads);
        tot_is = runAnalysis(&dom_is, source3,
                                    "inner", nthreads);
        tot_wu08 = runAnalysis(&dom_wu08, source_wu1,
                                      "wu_08", nthreads);
        tot_wu16 = runAnalysis(&dom_wu16, source_wu1,
                                      "wu_16", nthreads);
        tot_wu32 = runAnalysis(&dom_wu32, source_wu1,
                                      "wu_32", nthreads);
        tot_wu64 = runAnalysis(&dom_wu64, source_wu1,
                                      "wu_64", nthreads);
    }
    else
    {
        tot = runAnalysis(&dom, source0,
                                 "times_lin_1000_serial.txt", nthreads);
        tot_od = runAnalysis(&dom_od, source2,
                                    "times_lin_1000_outer.txt", nthreads);
        tot_is = runAnalysis(&dom_is, source3,
                                    "times_lin_1000_inner.txt", nthreads);
        tot_wu08 = runAnalysis(&dom_wu08, source_wu1,
                                      "times_lin_1000_wu_08.txt", nthreads);
        tot_wu16 = runAnalysis(&dom_wu16, source_wu1,
                                      "times_lin_1000_wu_16.txt", nthreads);
        tot_wu32 = runAnalysis(&dom_wu32, source_wu1,
                                      "times_lin_1000_wu_32.txt", nthreads);
        tot_wu64 = runAnalysis(&dom_wu64, source_wu1,
                                      "times_lin_1000_wu_64.txt", nthreads);
    }

    printf("Total:       %.12le\n", tot);
    printf("Total outer: %.12le\n", tot_od);
    printf("Total inner: %.12le\n", tot_is);
    printf("Total wu 08: %.12le\n", tot_wu08);
    printf("Total wu 16: %.12le\n", tot_wu16);
    printf("Total wu 32: %.12le\n", tot_wu32);
    printf("Total wu 64: %.12le\n", tot_wu64);


    freeDomain(&dom);
    freeDomain(&dom_od);
    freeDomain(&dom_is);
    freeDomain(&dom_wu08);
    freeDomain(&dom_wu16);
    freeDomain(&dom_wu32);
    freeDomain(&dom_wu64);

    return 0;
}
