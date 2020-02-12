import Graph_Modules.MST;
import java.util.*;

class Edge {
   Integer v;
   Integer w;
   Edge(Integer v, Integer w) {
      this.v = v;
      this.w = w;
   }
}

public class test {
    public test() {}
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        int V = input.nextInt();
        int E = input.nextInt();
        int u, v, w;
        List<List<Edge> > a = new ArrayList<List<Edge> >(V + 1);
        for(int i = 0 ; i <= E ; i++) {
            a.add(new ArrayList<Edge>());
        }
        for(int i = 0 ; i < E ; i++) {
            u = input.nextInt();
            v = input.nextInt();
            w = input.nextInt();
            a.get(u).add(new Edge(v, w));
        }


        MST.get_mst();
    }
}