import java.rmi.registry.LocateRegistry; 
import java.rmi.registry.Registry;  

public class Client {  
   private Client() {}  
   public static void main(String[] args) {  
        try {  
            Registry registry = LocateRegistry.getRegistry(null); 
            Grapher stub = (Grapher) registry.lookup("Grapher"); 
            stub.add_graph("2ce29ea", 5);  
            stub.add_edge("2ce29ea", 1, 2, 3);
            stub.add_edge("2ce29ea", 2, 4, 4);
            stub.print_graph("2ce29ea");  
        } catch (Exception e) {
         System.err.println("Client exception: " + e.toString()); 
         e.printStackTrace(); 
        } 
    } 
}