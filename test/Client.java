import java.rmi.registry.LocateRegistry; 
import java.rmi.registry.Registry;  
// import java.rmi.*;
import java.io.*;

public class Client {  
    static Registry registry;
    static Grapher stub;

    public static void HandleInput(String input){
        String[] inputs = input.split(" ");
        String cmd = inputs[0];
        String graph_id = inputs[1];
        switch(cmd) {
            case "add_graph":
                if(inputs.length != 3)
                    System.out.println("Usage : add_graph <graph_id> <no_of_nodes>");
                int no_of_nodes;
                no_of_nodes = Integer.parseInt(inputs[2]); 
                // Assuming a number has been input and 
                // Parsing it does not throw an exception
                try {
                    Client.stub.add_graph(graph_id, no_of_nodes);
                } catch (Exception e) {
                    System.out.println("Error while calling add graph : " + e.toString()); 
                }
                break;
            case "add_edge":
                int u, v, w;
                if(inputs.length != 5)
                    System.out.println("Usage : add_edge <graph_id> <u> <v> <w>");
                u = Integer.parseInt(inputs[2]);
                v = Integer.parseInt(inputs[3]);
                w = Integer.parseInt(inputs[4]);
                // Assuming a number has been input and 
                // Parsing it does not throw an exception
        
                try {
                    Client.stub.add_edge(graph_id, u, v, w);
                } catch (Exception e) {
                    System.out.println("Error while calling add graph : " + e.toString()); 
                }
                break;
            case "get_mst":
                if(inputs.length != 2) 
                    System.out.println("Usage: get_mst <graph_id>");
                
                try {
                    System.out.println(Client.stub.get_mst(graph_id));
                } catch (Exception e) {
                    System.out.println("Error while calling add graph : " + e.toString()); 
                }
                break;
            default:
                System.out.println("Wrong command. Please try again.");
        }
    }

   public static void main(String[] args) {  
        if(args.length != 2) {
            System.out.println("Usage: java Client <server-ip> <port>");
            return;
        }
        try {
            String server_ip = args[0];
            int port = Integer.parseInt(args[1]);
            Client.registry = LocateRegistry.getRegistry(server_ip, port); 
            Client.stub = (Grapher) registry.lookup("Grapher");
        } catch (Exception e) {
            System.err.println("Client exception: " + e.toString()); 
            e.printStackTrace(); 
        }
        String input;
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        
        try {
            while ((input = br.readLine()) != null) {
                Client.HandleInput(input);
            }
        } catch (Exception e) {
            System.err.println("Client exception: " + e.toString()); 
            e.printStackTrace();
        }
    } 
}