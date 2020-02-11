# Assignment 2

### How to run : 
* Make sure java is installed.
* Run following commands on one terminal: 
    ```
    javac *.java
    rmiregistry &
    java Server
    ```
* The server will now wait for clients to make calls.
* On another terminal window, execute 
    ```
    java Client
    ```
* On the server side, this should be printed : 
    ```
    This is an example RMI program
    ```
* This is a boilerplate of the code.