#include <bits/stdc++.h>
#include <limits.h>
#define ll long long int

using namespace std;

void print_adj(vector<pair<ll, ll>> adj[], ll V) {
	for(ll u = 1 ; u <= V ; u++) {
		cout << u << ": ";
		for(ll j = 0 ; j < adj[u].size() ; j++ ) {
			cout << "(" << adj[u][j].first << "," << adj[u][j].second <<") "; 
		}
		cout << endl;
	}
}

void print_distancearray(ll* d, ll V) {
	for(ll i = 1 ; i <= V ; i++) {
		cout << d[i] << " ";
	}
	cout << endl;
}

ll* bellman_ford(vector<pair<ll, ll>> adj[], ll V, ll source) {
	ll* d = (ll *)malloc(sizeof(ll) * (V + 1));
	ll v, w;
	for(ll i = 1 ; i <= V ; i++) {
		d[i] = LLONG_MAX;
	}
	d[source] = 0;
	// print_distancearray(d, V);
	for(ll nokia_case_cover = 0 ; nokia_case_cover < V ; nokia_case_cover++) {
		// because this variable is as useless as nokia case cover, get it?
		// don't @ me
		// kbye

		// Iterating through all edges V - 1 times
		for(ll u = 1 ; u <= V ; u++) {
			for(ll i = 0 ; i < adj[u].size() ; i++) {
				v = adj[u][i].first;
				w = adj[u][i].second;
				if(d[v] >= d[u] + w){
					d[v] = d[u] + w;
				}
			}
		}
	}

	return d;
}

int main() {
	long long int V, E, u, v, w, source;
	cin >> V >> E;
	vector<pair<ll, ll>> adj[V + 1]; // first -> vertex number, second -> weight
	ll *shit;
	for(int i = 0 ; i < E ; i++) {
		cin >> u >> v >> w;
		adj[u].push_back(make_pair(v, w));
		// adj[v].push_back(make_pair(u, w));
	}
	// print_adj(adj, V);
	cin >> source;
	shit = bellman_ford(adj, V, source);
	print_distancearray(shit, V);
}