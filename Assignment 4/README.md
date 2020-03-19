# Assignment 4

### How to run

* Compile : 
    ```
    erlc q<no>.erl
    ```
* Run : 
    ```
    erl -noshell -s q<no> main <input_filename> <output_filename> -s init stop
    ```
* Sample input for the first question : (Ring token passing around).
    ```
    <no of processes> <token value>
    5 1
    ```
    * Sample input has been provided in the file `asdfa`.
* Sample output for the first question : 
    ```
    Process 1 received token 1 from process 0. 
    Process 2 received token 1 from process 1. 
    Process 3 received token 1 from process 2. 
    Process 4 received token 1 from process 3. 
    Process 0 received token 1 from process 4.
    ```
* Sample input for the first question:
    ```
    10 12 69 10 9 8 7 6 5 4 3 2 1 20 88 87 100 102 103
    ```
    The input for the second question has been provided in the file `2input`.
* Sample output for the second question:
    ```
    1 2 3 4 5 6 7 8 9 10 10 12 20 69 87 88 100 102 103 
    ```
### Solution : 
* Q1 : 
    * N processes were spawned, at the start of the program.
    * Each process waits at the start to receive the process id of the process that it has to send a token to.
    * Once each process receives the token, except the root process, all processes wait to receive the token. After receiving the token, they pass it on to the id of the process they received earlier.
    * The root process on the other hand, first sends the token, after which it waits for the token to circulate around all processes, and eventually receives the token from the last process.
* Q2 :
    * No of processes that are spawned are fixed = `8` = P.
    * The array is divided into `K = Length/P` parts, every process is given `K` parts except for the last.
    * The last process is given `K + Length % P` parts, i.e. its share + all the remaining numbers.
    * Each process does a mergesort on the sublist that it receives.
    * The master process then collects all sorted sublists, and recursively applies the `merge` function of mergesort, and gathers all sorted sublists into one sorted list.
