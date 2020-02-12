import java.rmi.Remote; 
import java.rmi.RemoteException;  

public interface Grapher extends Remote {  
   class Edge {
      Integer v;
      Integer w;
      public Edge(Integer v, Integer w) {
         this.v = v;
         this.w = w;
      }
   }
   
   public void add_graph(String graph_identifier, int n) throws RemoteException;
   public void add_edge(String graph_identifier, int u, int v, int w) throws RemoteException;
   public void get_mst(String graph_identifier) throws RemoteException;
   public void print_graph(String graph_identifier) throws RemoteException;
} 