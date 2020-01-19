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
int partition(vector<int>& arr, int l, int r) {
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

void quick_sort(vector<int>& arr, int l, int r) {
    if( l > r ) { return; }

    int q = partition(arr, l, r);
    quick_sort(arr, l, q - 1);
    quick_sort(arr, q + 1, r);
}

/* END QUICKSORT CODE */

/* VECTOR SEND AND RECEIVE WRAPPERS */

int send_vector(vector<int>& v, int start_pos, int no_of_elements, int rank, MPI_Comm world) {
    int ierr = MPI_Send( &no_of_elements, 1 , MPI_INT, rank, send_data_tag, world);
    ierr = MPI_Send( &v[start_pos], no_of_elements, MPI_INT, rank, send_data_tag, world);   
    return ierr;
}

vector<int> receive_vector(int rank, MPI_Comm world) {
    vector<int> v;
    int rows_receive, ierr;
    MPI_Status status;
    ierr = MPI_Recv( &rows_receive, 1, MPI_INT, rank, send_data_tag, world, &status);
    v.resize(rows_receive);
    ierr = MPI_Recv( &v[0], rows_receive, MPI_INT, rank, send_data_tag, world, &status);
    return v;
}

/* END WRAPPERS */

int main( int argc, char **argv ) {
    int rank, p, ierr, rows_receive;
    vector<int> arr;

    /* start up MPI */
    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &p );
    
    /*synchronize all processes*/
    MPI_Barrier( MPI_COMM_WORLD );
    double tbeg = MPI_Wtime();

    /* write your code here */

    int root_process = 0;
    int n, w, l, r, rows_send, input;

    MPI_Status status;

    // define w = n / p =  #no_of_elements / #processors

    if(rank == root_process) {
        cin >> n;
        // int arr[max_rows];
        for(int i = 0 ; i < n ; i++) { 
            cin >> input;
            arr.push_back(input);
        }

        w = n / p;

        for(int id = 1 ; id < p ; id++) {
            l = w * id;
            r = w * ( id + 1 ) - 1;

            if(r >= n) {
                r = n - 1;
            }

            rows_send = r - l + 1;

            // send_vector(vector, starting position,
            //              no of rows to send, rank of process to send, world)
            send_vector(arr, l, rows_send, id, MPI_COMM_WORLD);
        }
        rows_receive = w;
    }
    else {
        arr = receive_vector(root_process, MPI_COMM_WORLD);
    }

    printf("Numbers received by %d are : \n", rank);
    for(int i = 0 ; i < arr.size() ; i++) {
        printf("%d ", arr[i]);
    }   
    quick_sort(arr, 0, rows_receive - 1);

    printf("\n");

    // gathering number of regular samples from all processors
 //    MPI_Reduce(&j, &no_regulra_samples, 1, MPI_INT, MPI_SUM, root_process, MPI_COMM_WORLD);
 //    MPI_Gather(local_regular_samples, j, MPI_INT, regular_samples, j, MPI_INT, root_process, MPI_COMM_WORLD); 

 //    int pivots[max_rows / p], no_of_pivots;

 //    if(rank == root_process) {
 //        // quick_sort(regular_samples, 0, no_regular_samples - 1);
	// }

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
