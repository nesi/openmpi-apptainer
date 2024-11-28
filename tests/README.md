## `mpi_test.c`

* Run across multiple nodes (2 nodes, 2 processes per node by default)
* Print detailed information about each process including:

  - Hostname of each node
  - MPI processor name
  - Process ID
  - Rank distribution

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
