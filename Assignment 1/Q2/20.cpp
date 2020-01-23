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
	if(v[n].d == LLONG_MAX) {
		cout << -1 << endl;
		return;
	}
	vector<int> path;
	for(long long int i = n ; i != -1 ; i = v[i].pi) {
		path.push_back(i);
	}
	for(long long int i = path.size() - 1 ; i >= 0 ; i-- ) {
		cout << path[i] << " ";
	}
	cout << endl;
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
	// set<pair<long long int, long long int>> pq;
	qs source, queue_min, temp;
	source.d = 0;
	source.s = 1;
	pq.push(source);
	// pq.insert(make_pair(0, 1));
	long long int u, v, w;
	while(!pq.empty()) {
		queue_min = pq.top();
		u = queue_min.s;
		// u = pq.begin()->second;
		// pq.erase(pq.begin());
		pq.pop();
		visited[u] = true;
		for(long long int k = 0 ; k < adj[u].size() ; k++){
			v = adj[u][k].dest;
			w = adj[u][k].weight;
			if((V[v].d > V[u].d + w)) {
				V[v].d = V[u].d + w;
				V[v].pi = u;

				// updating the queue
				temp.d = V[v].d;
				temp.s = v;
				pq.push(temp);
				// pq.insert(make_pair(V[v].d, v));
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
		tmp.dest = u;
		adj[v].push_back(tmp);
	}
	djikstra(adj, V, 1);
	return 0;
}