#include <stdio.h>
#include <string.h>
#include <bits/stdc++.h>
#include "mpi.h"

#define send_data_tag 2001
#define return_data_tag 2002
#define max_rows 10000

using namespace std;


/* QUICKSORT CODE */

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Returns the position of the pivot
int partition(int arr[], int l, int r) {
    // choosing pivot to be the last element
    int pivot = arr[r];
    int i = l - 1;
    for(int j = l ; j < r ; j++) {
        if(arr[j] <= pivot) {
            i += 1;
            swap(&arr[i], &arr[j]);
        }
    }
    i += 1;
    swap(&arr[i], &arr[r]);
    return i;
}

void quick_sort(int arr[], int l, int r) {
    if( l > r ) { return; }

    int q = partition(arr, l, r);
    quick_sort(arr, l, q - 1);
    quick_sort(arr, q + 1, r);
}

/* END QUICKSORT CODE */

int main( int argc, char **argv ) {
    int rank, p, ierr, rows_receive;
    int arr[max_rows];

    /* start up MPI */
    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &p );
    
    /*synchronize all processes*/
    MPI_Barrier( MPI_COMM_WORLD );
    double tbeg = MPI_Wtime();

    /* write your code here */

    int root_process = 0;
    int n, w, l, r, rows_send;

    MPI_Status status;

    // define w = n / p =  #no_of_elements / #processors

    if(rank == root_process) {
        cin >> n;
        int arr[max_rows];
        for(int i = 0 ; i < n ; i++) { cin >> arr[i]; }

        w = n / p;

        for(int id = 1 ; id < p ; id++) {
            l = w * id;
            r = w * ( id + 1 ) - 1;

            if(r >= n) {
                r = n - 1;
            }

            rows_send = r - l + 1;

            ierr = MPI_Send( &rows_send, 1 , MPI_INT, id, send_data_tag, MPI_COMM_WORLD);
            ierr = MPI_Send( &arr[l], rows_send, MPI_INT, id, send_data_tag, MPI_COMM_WORLD);
        }
        rows_receive = w;
    }
    else {
        ierr = MPI_Recv( &rows_receive, 1, MPI_INT, root_process, send_data_tag, MPI_COMM_WORLD, &status);
          
        ierr = MPI_Recv( &arr, rows_receive, MPI_INT, root_process, send_data_tag, MPI_COMM_WORLD, &status);

    }

    quick_sort(arr, 0, rows_receive - 1);
    // printf("Numbers received by %d are : \n", rank);
    // for(int i = 0 ; i < rows_receive ; i++) {
    //     printf("%d ", arr[i]);
    // }   
    // printf("\n");

    int local_regular_samples[max_rows / (p*p)], spacing;

    spacing = rows_receive / p;

    int i, j;
    for(i = 0, j = 0 ; i < rows_receive ; j++, i += spacing) {
        local_regular_samples[j] = arr[i];
    }

    printf("For rank %d : ", rank);
    for(int l = 0 ; l < j; l++) {
        printf("%d ", local_regular_samples[l]);
    }
    printf("\n");

    int regular_samples[max_rows / p];

    int no_regular_samples;


    // gathering number of regular samples from all processors
    MPI_Reduce(&j, &no_regular_samples, 1, MPI_INT, MPI_SUM, root_process, MPI_COMM_WORLD);
    MPI_Gather( local_regular_samples, j, MPI_INT, regular_samples, j, MPI_INT, root_process, MPI_COMM_WORLD); 

    int pivots[max_rows / p], no_of_pivots;

    if(rank == root_process) {
        quick_sort(regular_samples, 0, no_regular_samples - 1);
        
        // for(int i = 0 ; i < no_regular_samples ; i++) {
        //     printf("%d ", regular_samples[i]);
        // }
  
	    int no_sorted_samples;

	    int sorted_sample_spacing = no_regular_samples / p;
	    for(i = sorted_sample_spacing , no_sorted_samples = 0 ; i < no_regular_samples ; i+= sorted_sample_spacing, no_sorted_samples++ ) {
	    	pivots[no_sorted_samples] = regular_samples[i];
	    }

	    // printf("sorted regular sample : ");
	    // for(i = 0 ; i < no_sorted_samples ; i++) {
	    // 	printf("%d ", pivots[i]);
	    // }

	    for(int id = 1 ; id < p ; id++ ) {
            ierr = MPI_Send( &no_sorted_samples, 1 , MPI_INT, id, send_data_tag, MPI_COMM_WORLD);
	    }
	    no_of_pivots = no_sorted_samples;
	}
	else {
    	ierr = MPI_Recv( &no_of_pivots, 1, MPI_INT, root_process, send_data_tag, MPI_COMM_WORLD, &status);		
	}
    MPI_Bcast(&pivots, no_of_pivots, MPI_INT, root_process, MPI_COMM_WORLD);
    printf("%d\n", rank);	
	for(i = 0 ; i < no_of_pivots ; i++) {
		printf("%d ", pivots[i]);
	}
	printf("\n");

    /* Code ends here */


    MPI_Barrier( MPI_COMM_WORLD );
    double elapsedTime = MPI_Wtime() - tbeg;
    double maxTime;
    MPI_Reduce( &elapsedTime, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD );
    if ( rank == 0 ) {
        printf( "Total time (s): %f\n", maxTime );
    }

    /* shut down MPI */
    MPI_Finalize();
    return 0;
}
