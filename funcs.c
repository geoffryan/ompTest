#include <math.h>
#include <omp.h>
#include "dom.h"
#include "funcs.h"
#include "prim.h"


void source0(struct Domain *dom)
{
    int i, j;
    int nq = dom->nq;
    for(i=0; i<dom->nx; i++)
    {
        double x = 0.5*(dom->ximh[i] + dom->ximh[i+1]);

        for(j=0; j<dom->ny[i]; j++)
        {
            double y = 0.5*(dom->yjmh[i][j] + dom->yjmh[i][j+1]);
            source_prim(dom->prim[i] + nq*j, x, y, nq);
        }
    }
}


void source1(struct Domain *dom)
{
    int i;
    int nq = dom->nq;
    
#pragma omp parallel for private(i)
    for(i=0; i<dom->nx; i++)
    {
        double x = 0.5*(dom->ximh[i] + dom->ximh[i+1]);

        int j;
        for(j=0; j<dom->ny[i]; j++)
        {
            double y = 0.5*(dom->yjmh[i][j] + dom->yjmh[i][j+1]);
            source_prim(dom->prim[i] + nq*j, x, y, nq);
        }
    }
}



void source2(struct Domain *dom)
{
    int i;
    int nq = dom->nq;
    
#pragma omp parallel for schedule(dynamic, 8) private(i)
    for(i=0; i<dom->nx; i++)
    {
        double x = 0.5*(dom->ximh[i] + dom->ximh[i+1]);
        int j;
        for(j=0; j<dom->ny[i]; j++)
        {
            double y = 0.5*(dom->yjmh[i][j] + dom->yjmh[i][j+1]);
            source_prim(dom->prim[i] + nq*j, x, y, nq);
        }
    }
}


void source3(struct Domain *dom)
{
    int i;
    int nq = dom->nq;
    for(i=0; i<dom->nx; i++)
    {
        double x = 0.5*(dom->ximh[i] + dom->ximh[i+1]);
        
        int j;
#pragma omp parallel for private(j)
        for(j=0; j<dom->ny[i]; j++)
        {
            double y = 0.5*(dom->yjmh[i][j] + dom->yjmh[i][j+1]);
            source_prim(dom->prim[i] + nq*j, x, y, nq);
        }
    }
    
}


void source4(struct Domain *dom)
{
    int i;
    int nq = dom->nq;
    for(i=0; i<dom->nx; i++)
    {
        double x = 0.5*(dom->ximh[i] + dom->ximh[i+1]);

        int j;
#pragma omp parallel for schedule(dynamic) private(j)
        for(j=0; j<dom->ny[i]; j++)
        {
            double y = 0.5*(dom->yjmh[i][j] + dom->yjmh[i][j+1]);
            source_prim(dom->prim[i] + nq*j, x, y, nq);
        }
    }
}


void source_wu1(struct Domain *dom)
{
    int nq = dom->nq;

    int w;
#pragma omp parallel for schedule(dynamic) private(w)
    for(w=0; w<dom->nw; w++)
    {
        int i = dom->work_unit[w].i;
        int ja = dom->work_unit[w].ja;
        int jb = dom->work_unit[w].jb;
        
        double x = 0.5*(dom->ximh[i] + dom->ximh[i+1]);

        int j;
        for(j=ja; j<ja+dom->maxCellsPerWU; j++)
        {
            if(j >= jb)
                break;
            double y = 0.5*(dom->yjmh[i][j] + dom->yjmh[i][j+1]);
            source_prim(&(dom->prim[i][nq*j]), x, y, nq);
        }
    }
}

void plm(struct Domain *dom)
{
}

void flux(struct Domain *dom)
{
}

double reduce(struct Domain *dom)
{
    double sum = 0.0;
    int i, j, q;

    for(i=0; i<dom->nx; i++)
        for(j=0; j<dom->ny[i]; j++)
            for(q=0; q<dom->nq; q++)
                sum += dom->prim[i][dom->nq*j+q];

    return sum;
}
