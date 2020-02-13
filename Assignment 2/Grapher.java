import java.rmi.Remote; 
import java.rmi.RemoteException;  

public interface Grapher extends Remote {
   
   public String add_graph(String graph_identifier, int n) throws RemoteException;
   public String add_edge(String graph_identifier, int u, int v, int w) throws RemoteException;
   public String get_mst(String graph_identifier) throws RemoteException;
   public void print_graph(String graph_identifier) throws RemoteException;
} 