#include <math.h>
#include <omp.h>
#include "funcs.h"
#include "prim.h"


void source0(double **dom, int nx, int *ny, int nq)
{
    int i, j;
    for(i=0; i<nx; i++)
    {
        double x = (i + 0.5) / nx;

        for(j=0; j<ny[i]; j++)
        {
            double y = (6.0*(j+0.5))/ny[i];
            source_prim(dom[i] + nq*j, x, y, nq);
        }
    }
}


void source1(double **dom, int nx, int *ny, int nq)
{
    int i;
    
#pragma omp parallel for private(i)
    for(i=0; i<nx; i++)
    {
        double x = (i + 0.5) / nx;
        int j;

        for(j=0; j<ny[i]; j++)
        {
            double y = (6.0*(j+0.5))/ny[i];
            source_prim(dom[i] + nq*j, x, y, nq);
        }
    }
}



void source2(double **dom, int nx, int *ny, int nq)
{
    int i;
    
#pragma omp parallel for schedule(dynamic, 8) private(i)
    for(i=0; i<nx; i++)
    {
        double x = (i + 0.5) / nx;
        int j;

        for(j=0; j<ny[i]; j++)
        {
            double y = (6.0*(j+0.5))/ny[i];
            source_prim(dom[i] + nq*j, x, y, nq);
        }
    }
}


void source3(double **dom, int nx, int *ny, int nq)
{
    int i;
    
    for(i=0; i<nx; i++)
    {
        double x = (i + 0.5) / nx;
        int j;

#pragma omp parallel for private(j)
        for(j=0; j<ny[i]; j++)
        {
            double y = (6.0*(j+0.5))/ny[i];
            source_prim(dom[i] + nq*j, x, y, nq);
        }
    }
}


void source4(double **dom, int nx, int *ny, int nq)
{
    int i;
    
    for(i=0; i<nx; i++)
    {
        double x = (i + 0.5) / nx;
        int j;

#pragma omp parallel for schedule(dynamic) private(j)
        for(j=0; j<ny[i]; j++)
        {
            double y = (6.0*(j+0.5))/ny[i];
            source_prim(dom[i] + nq*j, x, y, nq);
        }
    }
}

void plm(double **dom, int nx, int *ny, int nq)
{
}

void flux(double **dom, int nx, int *ny, int nq)
{
}

double reduce(double **dom, int nx, int *ny, int nq)
{
    double sum = 0.0;
    int i, j, q;

    for(i=0; i<nx; i++)
        for(j=0; j<ny[i]; j++)
            for(q=0; q<nq; q++)
                sum += dom[i][nq*j+q];

    return sum;
}
