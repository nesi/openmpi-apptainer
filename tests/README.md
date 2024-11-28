This test script does the following:

* runs for approximately 2.5 minutes
* Performs repeated AllReduce operations, which test collective communication.
* Also performs point-to-point communication between the first and last ranks.
* Prints the node information for each process, helping you verify that the processes are running on different nodes.
* Reports the total runtime, number of iterations completed, and average time per iteration.

