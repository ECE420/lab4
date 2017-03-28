#define LAB4_EXTEND

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "Lab4_IO.h"
#include "timer.h"

#define EPSILON 0.00001
#define DAMPING_FACTOR 0.85

int main (int argc, char* argv[]){
    struct node *nodehead;
    int nodecount;
    int *num_in_links, *num_out_links;
    double *r, *r_pre;
    int i, j;
    double damp_const;
    int npes, myrank;
    double *buf;
    double start, end;

    if (get_node_stat(&nodecount, &num_in_links, &num_out_links)) return 254;    
    // Calculate the result
    if (node_init(&nodehead, num_in_links, num_out_links, 0, nodecount)) return 254;


    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    

    r = malloc(nodecount * sizeof(double));
    r_pre = malloc(nodecount * sizeof(double));
    buf = malloc(nodecount * sizeof(double));
    for ( i = 0; i < nodecount; ++i)
        r[i] = 1.0 / nodecount;
    damp_const = (1.0 - DAMPING_FACTOR) / nodecount;

    // Only measure the time using the first process
    if (myrank == 0)
        GET_TIME(start);
    // CORE CALCULATION
    do{
        vec_cp(r, r_pre, nodecount);
        for ( i = 0; i < nodecount; ++i){
            // split the calculation by the rank of process
    	    if ( i % npes == myrank) {
                r[i] = 0;
                for ( j = 0; j < nodehead[i].num_in_links; ++j)
                    r[i] += r_pre[nodehead[i].inlinks[j]] / num_out_links[nodehead[i].inlinks[j]];
                r[i] *= DAMPING_FACTOR;
                r[i] += damp_const;
    	       	buf[i] = r[i];
    	    }
            // For the rest values, just leave it as 0
    	    else {
        		buf[i] = 0;
    	    }
        }
        // Use allreduce to sum up the result
    	MPI_Allreduce(buf, r, nodecount, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    } while(rel_error(r, r_pre, nodecount) >= EPSILON);


    if (myrank == 0)
        GET_TIME(end);

    if (myrank == 0)
	Lab4_saveoutput(r, nodecount, end-start);

    MPI_Finalize();
    // post processing
    node_destroy(nodehead, nodecount);
    free(num_in_links); free(num_out_links);

}
