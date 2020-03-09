# Assignment 4

### How to run

* Compile : 
    ```
    erlc q<no>.erl
    ```
* Run : 
    ```
    erl -noshell -s q<no> -s init stop
    ```
* Sample input for the first question : (Ring token passing around).
    ```
    <no of processes> <token value>
    5 1
    ```
* Sample output for the first question : 
    ```
    Process 1 received token 1 from process 0. 
    Process 2 received token 1 from process 1. 
    Process 3 received token 1 from process 2. 
    Process 4 received token 1 from process 3. 
    Process 0 received token 1 from process 4.
    ```
### Solution : 
* Q1 : 
    * N processes were spawned, at the start of the program.
    * Each process waits at the start to receive the process id of the process that it has to send a token to.
    * Once each process receives the token, except the root process, all processes wait to receive the token. After receiving the token, they pass it on to the id of the process they received earlier.
    * The root process on the other hand, first sends the token, after which it waits for the token to circulate around all processes, and eventually receives the token from the last process.
* Q2 :
    * Mergesort implemented as of now.
