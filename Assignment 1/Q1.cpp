#include <stdio.h>
#include <string>
#include <bits/stdc++.h>
#include <math.h>
#include <fstream>
#include "mpi.h"

#define send_data_tag 2001
#define return_data_tag 2002
#define max_rows 10000

using namespace std;

void print_vector(vector<int>& v) {
    for(vector<int>::iterator i = v.begin() ; i != v.end() ; i++) cout << *i << " " ;
    cout << endl;
}

/* BEGIN K WAY MERGE CODE */

typedef struct node {
    unsigned long long int u;
    int array; // array element from which it was picked
    int index; // 
}node;

int parent(int i) { return (i - 1) / 2 ; }

int left(int i) { return 2*i + 1; }

int right(int i) { return 2*i + 2; }

void min_heapify(vector<node>& v, int i) {
    int l = left(i);
    int r = right(i);
    int smaller;
    if(l < v.size() && v[l].u < v[i].u) { smaller = l; }
    else { smaller = i; }

    if(r < v.size() && v[r].u < v[smaller].u) { smaller = r; } 

    if(smaller != i) {
        swap(v[i], v[smaller]);
        min_heapify(v, smaller);
    } 
}

void build_heap(vector<node>& v) {

    int parent_last = parent(v.size() - 1);
    for(int i = parent_last ; i >= 0; i--) {
        min_heapify(v, i);
    }
}

void pop_root(vector<node>& heap) {
    heap[0] = heap.back();
    heap.pop_back();
    min_heapify(heap, 0);
}

vector<int> k_merge(vector<int> v[], int no_of_vectors) {
    // Enter first elements of all vectors into heap
    vector<node> heap;
    int total_size = 0, array_index, i;
    node element;

    for(int i = 0 ; i < no_of_vectors ; i++) {
        element.u = v[i][0];
        element.array = i;
        element.index = 0;
        heap.push_back(element);
        v[i].erase(v[i].begin());
        total_size += v[i].size();
    }

    // Make the heap
    build_heap(heap);

    vector<int> sorted;
    while(!heap.empty()) {
        sorted.push_back(heap[0].u);
        array_index = heap[0].array;
        i = heap[0].index;
        node min;
        min.u = ULLONG_MAX;
        total_size = 0;
        for(int i = 0 ; i < no_of_vectors ; i++) {
            if(!v[i].size()) continue;
            if(min.u > v[i][0]) {
                min.u = v[i][0];
                min.array = i;
                min.index = 0;
            }
            total_size += v[i].size();
        }
        if(total_size > 0){
            v[min.array].erase(v[min.array].begin());
            heap.push_back(min);
        }
        pop_root(heap);
        total_size--; 
    }
    return sorted;
}

/* END MERGE CODE */

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

/* INPUT OUTPUT HANDLERS */

vector<int> input_output_from_file(char* input_file, char* output_file) {
    ifstream in(input_file);
    streambuf *input_buffer = cin.rdbuf();
    cin.rdbuf(in.rdbuf());

    ofstream out(output_file);
    streambuf *coutbuf = cout.rdbuf();
    cout.rdbuf(out.rdbuf()); 
}

/* END HANDLERS*/

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

    /* CODE BEGINS HERE */

    int root_process = 0;
    int n, w, l, r, rows_send, input;

    MPI_Status status;

    // define w = n / p =  #no_of_elements / #processors
    if(argc != 3) {
        if(rank == root_process) cout << "Usage : mpirun -np NO_OF_CPUS OBJECT_FILE INPUT_FILENAME OUTPUT_FILENAME" << endl;

        /* BEGIN END SEQUENCE*/
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
        /* END END SEQUENCE*/
    }

    /* BEGIN REDIRECTION OF INPUT OUTPUT */
    char* input_filename = argv[1];
    char* output_filename = argv[2]; 
    fstream out_file, in_file; 
    in_file.open(input_filename, ios::in);
    out_file.open(output_filename, ios::out); 

    streambuf* stream_buffer_cout = cout.rdbuf(); 
    streambuf* stream_buffer_cin = cin.rdbuf(); 

    streambuf* output_buffer = out_file.rdbuf(); 
    streambuf* input_buffer = in_file.rdbuf(); 
  
    cout.rdbuf(output_buffer); 
    cin.rdbuf(input_buffer);

    /* END REDIRECTION OF COUT TO FILE AND CIN FROM FILE */

    if(rank == root_process) {
        // input_output_from_file(argv[1], argv[2]);
        while(cin >> input) {
            arr.push_back(input);
        }

        std::string line;
            while(std::getline(std::cin, line))  //input from the file in.txt
            {
                std::cout << line << "\n";   //output to the file out.txt
            }

        vector<int> send_vectors[p];
        n = arr.size();
        w = n / p;

        for(int id = 0 ; id < p ; id++) {
            l = w * id;
            r = w * ( id + 1 ) - 1;

            if(r >= n) {
                r = n - 1;
            }

            rows_send = r - l + 1;

            vector<int> temp(arr.begin() + l, arr.begin() + l + rows_send);
            send_vectors[id] = temp;
        }
        // adding remaining elements to every vector
        for(int k = (n / p) * p ; k < n ; k++ ) {
            send_vectors[k % p].push_back(arr[k]);
        }

        for(int id = 1 ; id < p ; id++) {
            send_vector(send_vectors[id], 0, send_vectors[id].size(), id, MPI_COMM_WORLD);
        }
        rows_receive = w;
        arr = send_vectors[root_process];
    }
    else {
        arr = receive_vector(root_process, MPI_COMM_WORLD);
    }

    quick_sort(arr, 0, arr.size() - 1);

    // gathering number of regular samples from all processors
    if(rank != root_process) {
        send_vector(arr, 0, arr.size(), root_process, MPI_COMM_WORLD);
    }
    else {
        vector<int> sorted_arrays[p];
        sorted_arrays[0] = arr;
        for(int id = 1 ; id < p ; id++ ){
            sorted_arrays[id] = receive_vector(id, MPI_COMM_WORLD);
        }
        vector<int> sorted = k_merge(sorted_arrays, p);
        print_vector(sorted);
    }

    /* RESET BUFFER STREAMS */
    cout.rdbuf(stream_buffer_cout); 
    out_file.close(); 
    in_file.close(); 
    /* END RESET */

    /* CODE ENDS HERE */


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
