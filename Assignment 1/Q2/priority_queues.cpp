#include<bits/stdc++.h>

using namespace std;

typedef struct vertex {
	int d; // shortest path weights combined
	int pi; // parent vertex
}vertex;

bool compare(vertex a, vertex b){
	return a.d > b.d ;
}

int main() {
	priority_queue <vertex, vector<vertex>, function<bool(vertex, vertex)> > pq(compare);
	set<pair<int, int>> spt;
	spt.insert(make_pair(5, 10));
	spt.insert(make_pair(5, 11));
	spt.insert(make_pair(7, 10));
	spt.insert(make_pair(8, 10));
	spt.insert(make_pair(9, 10));
	spt.insert(make_pair(10, 10));
	spt.insert(make_pair(11, 10));
	for(auto i = spt.begin() ; i != spt.end() ; i++ ){
		cout << i->first << " " << i->second << endl;
	}
	vertex edge;
	edge.d = 10;
	edge.pi = 5;
	pq.push(edge);
	edge.d = 30;
	pq.push(edge);
	edge.d = 50;
	pq.push(edge);
	edge.d = 5;
	pq.push(edge);
	edge.d = 20;
	pq.push(edge);
	while(!pq.empty()) {
		edge = pq.top();
		cout << edge.d << " ";
		pq.pop();
	}
	cout << endl;
}