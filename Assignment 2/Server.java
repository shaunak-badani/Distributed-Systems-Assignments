import java.rmi.registry.Registry; 
import java.rmi.registry.LocateRegistry; 
import java.rmi.RemoteException; 
import java.rmi.server.UnicastRemoteObject; 

public class Server extends GrapherMethods { 
   public Server() {} 
   public static void main(String args[]) { 
      try { 
         // Instantiating the implementation class 
         GrapherMethods obj = new GrapherMethods(); 
    
         // Exporting the object of implementation class  
         // (here we are exporting the remote object to the stub) 
         Grapher stub = (Grapher) UnicastRemoteObject.exportObject(obj, 0);  
         
         // Binding the remote object (stub) in the registry 
         Registry registry = LocateRegistry.getRegistry(); 
         
         registry.bind("Grapher", stub);  
         System.err.println("Server ready"); 
      } catch (Exception e) { 
         System.err.println("Server exception: " + e.toString()); 
         e.printStackTrace(); 
      } 
   } 
} 