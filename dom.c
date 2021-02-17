#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "dom.h"

void makeDomain(struct Domain *dom, int nx, int nq, double xa, double xb,
                int gridflag, int maxCellsPerWorkUnit)
{
    dom->nx = nx;
    dom->ny = (int *)malloc(nx * sizeof(int));
    dom->nq = nq;
    dom->xa = xa;
    dom->xb = xb;
    dom->ya = 0.0;
    dom->yb = 6.0;

    dom->ximh = (double *) malloc( (nx+1) * sizeof(double *));
    dom->yjmh = (double **) malloc( nx * sizeof(double *));
    dom->prim = (double **) malloc( nx * sizeof(double *));

    int i, j, q;
    int nw = 0;

    dom->ximh[0] = xa;

    for(i=0; i<nx; i++)
    {
        double xp, dx;
        if(gridflag == GRID_LOG)
            xp = dom->xa * pow(dom->xb / dom->xa, (i+1.0)/nx);
        else
            xp = dom->xa + (i+1) * (dom->xb - dom->xa)/nx;

        dx = xp - dom->ximh[i];

        dom->ximh[i+1] = xp;
        dom->ny[i] = (int) ((dom->yb - dom->ya) * xp/dx);

        nw += dom->ny[i]/maxCellsPerWorkUnit + 1;
    }

    dom->nw = nw;
    
    dom->work_unit = (struct WorkUnit *)malloc(nw * sizeof(struct WorkUnit));
    int w = 0;

    for(i=0; i<nx; i++)
    {
        dom->yjmh[i] = (double *) malloc((dom->ny[i]+1) * sizeof(double));
        dom->prim[i] = (double *) malloc(dom->ny[i] * nq * sizeof(double));

        double x = 0.5*(dom->ximh[i] + dom->ximh[i+1]);

        dom->yjmh[i][0] = dom->ya;

        for(j=0; j<dom->ny[i]; j++)
        {
            double yp = dom->ya + (j+1) * (dom->yb - dom->ya)/dom->ny[i];
            dom->yjmh[i][j+1] = yp;
            double y = 0.5*(dom->yjmh[i][j] + dom->yjmh[i][j+1]);

            for(q=0; q<nq; q++)
                dom->prim[i][nq*j+q] = sqrt(x*x + y*y) + q;
        }

        for(j = 0; j < dom->ny[i]; j += maxCellsPerWorkUnit)
        {
            int jb = j + maxCellsPerWorkUnit;
            if(jb > dom->ny[i])
                jb = dom->ny[i];
            
            dom->work_unit[w].i = i;
            dom->work_unit[w].ja = j;
            dom->work_unit[w].jb = jb;
            
            w++;
        }
        
        /*
        int nwi = dom->ny[i]/maxCellsPerWorkUnit + 1;
        if(nwi > 1)
        {
            int jb = (dom->work_unit[w-2].ja + dom->work_unit[w-1].jb) / 2;
            dom->work_unit[w-2].jb = jb;
            dom->work_unit[w-1].ja = jb;
        }
        */
    }
}

void copyDomain(struct Domain *domIn, struct Domain *domOut)
{
    freeDomain(domOut);

    domOut->nx = domIn->nx;
    domOut->nq = domIn->nq;
    domOut->nw = domIn->nw;
    domOut->xa = domIn->xa;
    domOut->xb = domIn->xb;
    domOut->ya = domIn->ya;
    domOut->yb = domIn->yb;

    int nq = domIn->nq;

    domOut->ny = (int *)malloc(domOut->nx * sizeof(int));
    domOut->ximh = (double *)malloc((domOut->nx+1) * sizeof(double));
    domOut->yjmh = (double **)malloc(domOut->nx * sizeof(double *));
    domOut->prim = (double **)malloc(domOut->nx * sizeof(double *));
    domOut->work_unit = (struct WorkUnit *)malloc(domOut->nw
                                                  * sizeof(struct WorkUnit));

    int i, j, q;

    domOut->ximh[0] = domIn->ximh[0];

    for(i=0; i<domOut->nx; i++)
    {
        domOut->ny[i] = domIn->ny[i];
        domOut->ximh[i+1] = domIn->ximh[i+1];
    }

    for(i=0; i<domOut->nx; i++)
    {
        domOut->yjmh[i] = (double *)malloc((domOut->ny[i]+1) * sizeof(double));
        domOut->prim[i] = (double *)malloc(domOut->ny[i] * nq
                                           * sizeof(double));
        domOut->yjmh[i][0] = domIn->yjmh[i][0];

        for(j=0; j<domOut->ny[i]; j++)
        {
            domOut->yjmh[i][j+1] = domIn->yjmh[i][j+1];
            for(q=0; q<nq; q++)
                domOut->prim[i][nq*j+q] = domIn->prim[i][nq*j+q];
        }
    }

    int w;
    for(w=0; w<domOut->nw; w++)
        domOut->work_unit[w] = domIn->work_unit[w];
}

void freeDomain(struct Domain *dom)
{
    int i;
    if(dom->yjmh != NULL)
    {
        for(i=0; i<dom->nx; i++)
        {
            if(dom->yjmh[i] != NULL)
            {
                free(dom->yjmh[i]);
                dom->yjmh[i] = NULL;
            }
        }
        free(dom->yjmh);
        dom->yjmh = NULL;
    }

    if(dom->prim != NULL)
    {
        for(i=0; i<dom->nx; i++)
        {
            if(dom->prim[i] != NULL)
            {
                free(dom->prim[i]);
                dom->prim[i] = NULL;
            }
        }
        free(dom->prim);
        dom->prim = NULL;
    }

    if(dom->ny != NULL)
    {
        free(dom->ny);
        dom->ny = NULL;
    }

    if(dom->ximh != NULL)
    {
        free(dom->ximh);
        dom->ximh = NULL;
    }

    if(dom->work_unit != NULL)
    {
        free(dom->work_unit);
        dom->work_unit = NULL;
    }

    dom->nx = 0;
    dom->nw = 0;
}

void printDomain(struct Domain *dom)
{
    int i, j, q;
    int nq = dom->nq;
    for(i=0; i<dom->nx; i++)
    {
        printf("%03d:", i);
        for(j=0; j<dom->ny[i]; j++)
        {
            printf(" (%.1lf", dom->prim[i][nq*j]);
            for(q=1; q<nq; q++)
                printf(",%.1lf", dom->prim[i][nq*j+q]);
            printf(")");
        }
        printf("\n");
    }
}

void printDomainShort(struct Domain *dom)
{
    int i;

    printf("%d\n", dom->nx);

    printf("%.1lf", dom->ximh[0]);
    for(i=1; i<=dom->nx; i++)
        printf(" %.1lf", dom->ximh[i]);
    printf("\n");

    printf("%d", dom->ny[0]);
    for(i=1; i<dom->nx; i++)
        printf(" %d", dom->ny[i]);
    printf("\n");

    printf("(%d,%d,%d)", dom->work_unit[0].i, dom->work_unit[0].ja,
          dom->work_unit[0].jb);
    for(i=1; i<dom->nw; i++)
        printf(" (%d,%d,%d)", dom->work_unit[i].i, dom->work_unit[i].ja,
              dom->work_unit[i].jb);
    printf("\n");

}
