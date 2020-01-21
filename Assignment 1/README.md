# Parallelization of popular algorithms using Mpirun

### How to run : 

* Install MPI from [here]("https://ireneli.eu/2016/02/15/installation")
* Run the following :
```
mpic++ Q<no>.cpp -o <objectfile>
mpirun -np <no_of_cpus> <objectfile>
```
---

### Question 1 : Quicksort

* The task : To parallelize quicksort.
* Method : 
    * Divide array into (n / p) parts.
    * Allot each part to a processor.
    * Divide remaining n % p elements among processors.
    * Command each processor to apply quicksort to received arrays.
    * Collect sorted arrays into the root processor.
    * Use k-way merge to join all the sorted arrays.

---