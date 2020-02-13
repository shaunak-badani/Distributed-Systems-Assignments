import java.util.*;

public class GrapherMethods implements Grapher {  
   static Map<String, List<List<Edge> > > graph_list;

   public GrapherMethods() {
      // Collection of graphs
      graph_list = new HashMap<String, List<List<Edge> > >();

      // ArrayList of graphs. Each element has a (string -> identifier) + graph
      // graph is represented using list of edges
      // arraylist[i] -> arraylist of edges connected to it.
   }
   
   public String add_graph(String graph_identifier, int n) {
      List<List<Edge> > new_graph = new ArrayList<List<Edge> >(n + 1);
      for(int i = 0 ; i <= n ; i++) { 
         new_graph.add(new ArrayList<Edge>());
      }
      graph_list.put(graph_identifier, new_graph);
      return "Added graph : " + graph_identifier;
   }  

   public void print_graph(String graph_identifier) {
      List<List<Edge> > a = graph_list.get(graph_identifier);
      if(a == null) {
         System.out.println("Graph does not exist.");
         return;
      }
      for(Integer i = 1 ; i < a.size() ; i++) {
         System.out.print(i + ": ");
         List<Edge> vertex = a.get(i);
         for(Edge e : vertex) {
            System.out.print(e.v + ", " + e.w + " ");
         }
         System.out.println();
      }
   }

   public String add_edge(String graph_identifier, int u, int v, int w) {
      List<List<Edge> > graph_req = graph_list.get(graph_identifier);

      // Adding edge to u
      Edge new_edge = new Edge(u, v, w);
      try {
         graph_req.get(u).add(new_edge);
      } catch(Exception e) {
         return "Graph requested not found";
      }

      // Success
      return "Added edge to " + graph_identifier + " from " + u + " to " + v + " with weight " + w;
   }

   public String get_mst(String graph_identifier) {
      List<List<Edge> > graph_req;
      try {
         graph_req = graph_list.get(graph_identifier);
      } catch(Exception e) {
         return "Graph requested not found.";
      }
      List<Edge> mst = MST.get_mst(graph_req);


      // If no of edges in mst != no of vertices - 1, mst not found.      
      if(mst.size() != graph_req.size() - 2)
         return "-1";
      long weight = MST.mst_weight(mst);
      return Long.toString(weight);
   }

} 