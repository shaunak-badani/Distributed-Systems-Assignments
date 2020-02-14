# Assignment 2

### How to run : 
* Make sure java is installed.
* Run following commands on one terminal: 
    ```
    javac *.java
    rmiregistry <port-number> &
    java Server <port-number>
    ```
* The server will now wait for clients to make calls.
* On another terminal window / client, execute 
    ```
    javac Client.java
    java Client <server-ip> <port-number>
    ```
* A command line interface is provided on the Client side, after the `java Client` command is run.

### Commands available :

Command  | Explanation
------------- | -------------
add_graph \<graph-id\> \<no_of_nodes\> | Adds graph with number of vertices = `no_of_nodes` to list of graphs on the server side, with graph id = `graph_id`.
add_edge \<graph-id\> \<u\> \<v\> \<w\>  | Adds edge from `u` to `v` with weight `w` to graph with graph id = `graph-id`.
get_mst \<graph_id\> | Computes the sum of weights of minimum spanning tree of the graph id passed. If no MST exists, `-1` is printed.

* An example is given in the `input` file.