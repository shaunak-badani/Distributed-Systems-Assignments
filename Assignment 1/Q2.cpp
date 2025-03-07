/* MPI Program Template */

#include <stdio.h>
#include <string.h>
#include <bits/stdc++.h>
#include <limits.h>
#include "mpi.h"
#include <fstream> 
#include <iostream>

#define ll long long int

using namespace std;

/* UTIL FUNCTIONS */

void print_adj(ll adj[][1000], ll V) {
    for(ll u = 1 ; u <= V ; u++) {
        cout << u << ": ";
        for(ll v = 1 ; v <= V ; v++ ) {
            if(adj[u][v])
                cout << "(" << v << "," << adj[u][v] <<") "; 
        }
        cout << endl;
    }
}

void print_distancearray(ll* d, ll V, ll source) {
    for(ll i = 1 ; i <= V ; i++) {
        if(i != source)
            cout << i << " " << d[i] << endl;
    }
}

void redirect_input_output(char* input_filename, char* output_filename) {
    fstream out_file, in_file; 
    // fstream in_file;
    // char* file_name = argv[1];
    // cout << file_name;
    in_file.open(input_filename, ios::in);
    out_file.open(output_filename, ios::out); 

    // Backup streambuffers of  cout 
    streambuf* stream_buffer_cout = cout.rdbuf(); 
    streambuf* stream_buffer_cin = cin.rdbuf(); 
  
    // Get the streambuffer of the file 
    streambuf* output_buffer = out_file.rdbuf(); 
    streambuf* input_buffer = in_file.rdbuf(); 
  
    // Redirect cout to file 
    cout.rdbuf(output_buffer); 
    cin.rdbuf(input_buffer);
  
    // Redirect cout back to screen 
    // cout.rdbuf(stream_buffer_cout); 
    // cout << "This line is written to screen" << endl; 
  
    out_file.close(); 
    in_file.close(); 
    // return 0; 
}


/* END UTIL FUNCTIONS */

/* BELLMAN FORD ALGO AND HELPER FUNCTIONS */

// void initalize_bf(vector<ll>& d, ll source, ll V) {
//     for(ll i = 0 ; i < V ; i++ )
//         d.push_back(LLONG_MAX);
//     d[source] = 0;
// }

ll* parallelized_bellman_ford(ll adj[][1000], ll V, ll source, ll start, ll end, MPI_Comm world) {
    ll* d = (ll *)malloc(sizeof(ll) * (V + 1));
    ll v, w;
    for(ll i = 1 ; i <= V ; i++) {
        d[i] = LLONG_MAX;
    }
    d[source] = 0;
    for(ll nokia_case_cover = 0 ; nokia_case_cover < V ; nokia_case_cover++) {
        // because this variable is as useless as a nokia case cover, get it?
        // don't @ me
        // kbye

        // Iterating through all edges V - 1 times
        for(ll u = start ; u <= end ; u++) {
            for(ll v = 1 ; v <= V ; v++) {
                if(!adj[u][v]) continue;
                w = adj[u][v];
                if(d[u]!=LLONG_MAX && d[v] >= d[u] + w){
                    d[v] = d[u] + w;
                }
            }
        }
        // print_distancearray(d, V);
        MPI_Allreduce(MPI_IN_PLACE, &d[1], (int)V, MPI_LONG_LONG, MPI_MIN, world);
    }

    return d;
}

/* END HELPER FUNCTIONS */

int main( int argc, char **argv ) {
    int rank, numprocs;

    /* start up MPI */
    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &numprocs );
    int root_process = 0;
    
    /*synchronize all processes*/
    MPI_Barrier( MPI_COMM_WORLD );
    double tbeg = MPI_Wtime();

    /* write your code here */

    ll V, E, u, v, w, source, adj[1000][1000];

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

    if(rank == 0) {
        
        cin >> V >> E;

        // initializing the adjacency matrix
        for(ll i = 1 ; i <= V ; i++) {
            for(ll j = 1 ; j <= V ; j++)
                adj[i][j] = 0;
        }
        ll *shit;
        for(int i = 0 ; i < E ; i++) {
            cin >> u >> v >> w;
            adj[u][v] = w;
            adj[v][u] = w;
        }
        cin >> source;
        // print_adj(adj, V);
        vector<ll> d;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&V, 1, MPI_LONG_LONG, root_process, MPI_COMM_WORLD);
    MPI_Bcast(&adj[0][0], (1000) * (1000), MPI_LONG_LONG, root_process, MPI_COMM_WORLD);
    ll start, end;
    ll avg = V / numprocs;
    for(ll i = 0 ; i < numprocs ; i++) {
        if(i == rank) {
            start = i * avg + 1;
            end = (i + 1) * avg;
        }
    }
    if(rank == numprocs - 1) 
        end = V;
    ll *d;

    d = parallelized_bellman_ford(adj, V, source, start, end, MPI_COMM_WORLD);
    if(rank == 0) {
        print_distancearray(d, V, source);
         
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