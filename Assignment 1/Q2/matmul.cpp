#include <bits/stdc++.h>
long long int adj[10000][10000];

using namespace std;

void print_adjacency_matrix(long long int V) {
	cout << "  ";
	for(int i = 1 ; i <= V ; i++) {
		cout << i << " ";
	}
	cout << endl;
	for(int i = 1 ; i <= V ; i++) {
		cout << i << " "; 
		for(int j = 1 ; j <= V ; j++) {
			cout << adj[i][j] << " ";
		}
		cout << endl;
	}
}

void matmul(long long int V) {
	long long int temp[V+1][V+1];
	for(int i = 1 ; i <= V ; i++ ) {
		for(int j = 1 ; j <= V ;j++) {
			temp[i][j] = 0;
			for(int k = 1 ; k <= V ; k++) {
				temp[i][j] += adj[i][k] * adj[k][j];
			}
		}
	}

	for(int i = 1 ; i <= V ; i++) {
		for(int j = 1 ; j <= V ; j++) {
			adj[i][j] = temp[i][j];
		}
	}
}

int main() {
	long long int V, E, u, v, w;
	cin >> V >> E;

	for(int i = 0 ; i < E ; i++) {
		cin >> u >> v >> w;
		adj[u][v] = w;
		// adj[v][u] = w;
	} 
	print_adjacency_matrix(V);
	matmul(V);
	matmul(V);
	print_adjacency_matrix(V);
}