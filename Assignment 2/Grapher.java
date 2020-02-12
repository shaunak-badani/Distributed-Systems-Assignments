import java.rmi.Remote; 
import java.rmi.RemoteException;  

public interface Grapher extends Remote {  
   
   public void add_graph(String graph_identifier, int n) throws RemoteException;
   public void add_edge(String graph_identifier, int u, int v, int w) throws RemoteException;
   public void get_mst(String graph_identifier) throws RemoteException;
   public void print_graph(String graph_identifier) throws RemoteException;
} 