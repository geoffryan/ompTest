#ifndef DOM_H
#define DOM_H

#include <stdlib.h>

enum{GRID_LIN, GRID_LOG};

struct WorkUnit
{
    int i;
    int ja;
    int jb;
};

struct Domain
{
    int nx;
    int *ny;
    int nq;
    double xa;
    double xb;
    double ya;
    double yb;
    double *ximh;
    double **yjmh;
    double **prim;

    struct WorkUnit *work_unit;
    int nw;
};

static const struct Domain DOM_EMPTY = {0, NULL, 0, 0.0, 0.0, 0.0, 0.0,
                                        NULL, NULL, NULL,
                                        NULL, 0};

void makeDomain(struct Domain *dom, int nx, int nq, double xa, double xb,
                int gridflag, int maxCellsPerWorkUnit);
void copyDomain(struct Domain *domIn, struct Domain *domOut);
void freeDomain(struct Domain *dom);
void printDomain(struct Domain *dom);
void printDomainShort(struct Domain *dom);

#endif
