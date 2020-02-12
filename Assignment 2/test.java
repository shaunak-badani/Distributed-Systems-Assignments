import java.util.*;

// class Edge {
//    Integer v;
//    Integer w;
//    Edge(Integer v, Integer w) {
//       this.v = v;
//       this.w = w;
//    }
// }

public class test {
    public test() {}
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        int V = input.nextInt();
        int E = input.nextInt();
        int u, v, w;
        List<List<Edge> > a = new ArrayList<List<Edge> >(V + 1);
        for(int i = 0 ; i <= V ; i++) {
            a.add(new ArrayList<Edge>());
        }
        for(int i = 0 ; i < E ; i++) {
            u = input.nextInt();
            v = input.nextInt();
            w = input.nextInt();
            a.get(u + 1).add(new Edge(u + 1, v + 1, w));
        }
        List<Edge> mst = MST.get_mst(a);
        System.out.println(MST.mst_weight(mst));

    }
}