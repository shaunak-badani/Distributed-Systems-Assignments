#include<bits/stdc++.h>
#include<limits.h>

using namespace std;

typedef struct edge {
	int dest;
	unsigned long long int weight;
}edge;

void print_graph(vector<edge> v[], int V) {
	for(int i = 1 ; i <= V ; i++) {
		cout << "Vertex " << i << " : ";
		for(int k = 0 ; k < v[i].size() ; k++) {
			cout << "(" << v[i][k].dest << ", " << v[i][k].weight << ") ";
		}
		cout << endl;
	}
}

typedef struct vertex {
	long long int d; // shortest path weights combined
	long long int pi; // parent vertex
}vertex;

typedef struct queue_struct{
	long long int d; // shortest path weights combined	
	long long int s; // source vertex
}qs;

bool compare(qs a, qs b){
	return a.d > b.d ;
}

void print_shortest_path(vertex v[], int n) {
	for(long long int i = 1 ; i <= n ; i++) {
		cout << i << " " << v[i].d << endl;
	}
}

void djikstra(vector<edge> adj[], int no_of_vertices, int S) {
	// initialize single source shortest path
	bool visited[no_of_vertices + 1];
	vertex V[no_of_vertices + 1];
	for(int i = 1 ; i <= no_of_vertices ; i++) {
		visited[i] = false;
		V[i].d = LLONG_MAX;
		V[i].pi = -1;
	}
	V[S].d = 0;
	priority_queue <qs, vector<qs>, function<bool(qs, qs)> > pq(compare);
	qs source, queue_min, temp;
	source.d = 0;
	source.s = 1;
	long long int u, v, w;
	pq.push(source);
	while(!pq.empty()) {
		queue_min = pq.top();
		u = queue_min.s;
		if(visited[u]) {
			pq.pop();
			continue;
		}
		visited[u] = true;
		for(long long int k = 0 ; k < adj[u].size() ; k++){
			v = adj[u][k].dest;
			w = adj[u][k].weight;
			if(V[v].d > V[u].d + w) {
				V[v].d = V[u].d + w;
				V[v].pi = u;

				// updating the queue
				temp.d = V[v].d;
				temp.s = v;
				pq.push(temp);
			}
		} 
	}
	print_shortest_path(V, no_of_vertices);
}

int main() {
	int V, E;
	cin >> V >> E;
	vector<edge> adj[V + 1];
	int u, v, w; 
	edge tmp;
	for(int edge = 0 ; edge < E ; edge++) {
		cin >> u >> v >> w;
		tmp.dest = v;
		tmp.weight = w;
		adj[u].push_back(tmp);
	}
	int s;
	cin >> s;
	djikstra(adj, V, s);
	cout << s << endl;
	return 0;
}