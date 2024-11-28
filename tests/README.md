This test script does the following:

* runs for approximately 2.5 minutes
* Performs repeated AllReduce operations, which test collective communication.
* Also performs point-to-point communication between the first and last ranks.
* Prints the node information for each process, helping you verify that the processes are running on different nodes.
* Reports the total runtime, number of iterations completed, and average time per iteration.



## `matrixmultiplympi_test.c`

* Performs parallel matrix multiplication of two 1000x1000 matrices
* Uses MPI collective operations (Bcast, Scatterv, Gatherv)
* Includes detailed timing and performance metrics
* Shows memory usage per process
* Includes a checksum verification

Standard output should show:

* Overall execution time
* Per-process statistics
* Node distribution
* Memory usage
* Result verification via checksum
