import java.rmi.registry.LocateRegistry; 
import java.rmi.registry.Registry;  
import java.io.*;

public class Client {  
    static Registry registry;
    static Grapher stub;

    public static String HandleInput(String input){
        String[] inputs = input.split(" ");
        String cmd = inputs[0];
        String graph_id = inputs[1];
        switch(cmd) {
            case "add_graph":
                if(inputs.length != 3)
                    return "Usage : add_graph <graph_id> <no_of_nodes>";
                int no_of_nodes;
                try {
                    no_of_nodes = Integer.parseInt(inputs[2]);
                } catch (Exception e) {
                    return "Please enter a number for no of nodes";
                }
                try {
                    return Client.stub.add_graph(graph_id, no_of_nodes);
                } catch (Exception e) {
                    return "Error while calling add graph : " + e.toString(); 
                }
            case "add_edge":
                int u, v, w;
                if(inputs.length != 5)
                    return "Usage : add_edge <graph_id> <u> <v> <w>";
                try {
                    u = Integer.parseInt(inputs[2]);
                    v = Integer.parseInt(inputs[3]);
                    w = Integer.parseInt(inputs[4]);
                } catch (Exception e) {
                    return "Please enter a number for u, v, and w";
                }
                try {
                    return Client.stub.add_edge(graph_id, u, v, w);
                } catch (Exception e) {
                    return "Error while calling add graph : " + e.toString(); 
                }
            case "get_mst":
                if(inputs.length != 2) 
                    return "Usage: get_mst <graph_id>";
                
                try {
                    return Client.stub.get_mst(graph_id);
                } catch (Exception e) {
                    return "Error while calling add graph : " + e.toString(); 
                }
            default:
                return "Wrong command. Please try again.";
        }
    }

   public static void main(String[] args) {  
        try {
            Client.registry = LocateRegistry.getRegistry(null); 
            Client.stub = (Grapher) registry.lookup("Grapher");
        } catch (Exception e) {
            System.err.println("Client exception: " + e.toString()); 
            e.printStackTrace(); 
        }
        String input;
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        
        try {
            while ((input = br.readLine()) != null) {
                System.out.println(Client.HandleInput(input));
            }
        } catch (Exception e) {
            System.err.println("Client exception: " + e.toString()); 
            e.printStackTrace();
        }
    } 
}