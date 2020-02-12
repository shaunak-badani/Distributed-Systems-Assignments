import java.util.*;
import java.lang.*; 
import java.io.*;

class SortByWeight implements Comparator<Edge> {
    public int compare(Edge a, Edge b) { return a.w - b.w; }
}

class DSU {
    int par[], comp_size[];
    DSU(int n) { 
        this.par = new int[n + 1]; 
        this.comp_size = new int[n + 1];
        for(int i = 1 ; i <= n ; i++) {
            this.par[i] = i;
            this.comp_size[i] = 1;
        }
    }

    public int parent(int u) {
        if(this.par[u] == u) return u;
        this.par[u] = this.parent(this.par[u]);
        return this.par[u];
    } 

    public void join(int u, int v) {
        int x = this.parent(u);
        int y = this.parent(v);
        if(x == y) return;
        
        if(this.comp_size[x] < this.comp_size[y]) {
            this.comp_size[y] += this.comp_size[x];
            this.par[x] = y;
        }
        else {
            this.comp_size[x] += this.comp_size[y];
            this.par[y] = x;
        }
    }

    public boolean cycle(int u, int v) {
        return this.parent(u) == this.parent(v);
    }

}



public class MST {    

    public static List<Edge> get_mst(List<List<Edge>> graph) {

        List<Edge> edges = new ArrayList<Edge>();
        for(int i = 1; i < graph.size() ; i++) {
            // System.out.print(i + ": ");
            List<Edge> v = graph.get(i);
            for(Edge e : v) {
                edges.add(e);
                // System.out.print("(" + e.v + "," + e.w + ") ");
            }
            // System.out.println();
        }
        Collections.sort(edges, new SortByWeight());
        List<Edge> mst = new ArrayList<Edge>();
        int no_of_edges = edges.size();
        int no_of_vertices = graph.size() - 1;

        DSU d = new DSU(no_of_vertices);

        for(int i = 0, k = 0; i < no_of_edges && k < no_of_vertices - 1; i++ ) {
            Edge e = edges.get(i);
            if(!d.cycle(e.u, e.v)) {
                mst.add(e);
                d.join(e.u, e.v);
                k++;
            }
        }

        return mst;
    }

    public static long mst_weight(List<Edge> mst) {
        long sum = 0;
        for(Edge e : mst) { sum += e.w; }
        return sum;
    }
}