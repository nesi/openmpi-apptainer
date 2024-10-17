#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define ARRAY_SIZE 1000000
#define NUM_ITERATIONS 1000
#define RUN_TIME 150 // Run for 2.5 minutes

void print_node_info(int rank) {
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    printf("Process %d running on node %s\n", rank, processor_name);
}

int main(int argc, char** argv) {
    int rank, size, i;
    double *send_buffer, *recv_buffer;
    double start_time, end_time, elapsed_time;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Starting OpenMPI Infiniband test with %d processes\n", size);
        printf("This test will run for approximately %d seconds\n", RUN_TIME);
    }

    print_node_info(rank);
    MPI_Barrier(MPI_COMM_WORLD);

    send_buffer = (double*)malloc(ARRAY_SIZE * sizeof(double));
    recv_buffer = (double*)malloc(ARRAY_SIZE * sizeof(double));

    if (!send_buffer || !recv_buffer) {
        fprintf(stderr, "Process %d: Memory allocation failed\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    for (i = 0; i < ARRAY_SIZE; i++) {
        send_buffer[i] = (double)rank;
    }

    start_time = MPI_Wtime();
    int iteration = 0;

    while ((MPI_Wtime() - start_time) < RUN_TIME) {
        if (rank == 0) printf("Starting iteration %d\n", iteration);
        
        MPI_Barrier(MPI_COMM_WORLD);
        
        // Perform an AllReduce operation
        if (rank == 0) printf("Process %d: Starting AllReduce\n", rank);
        MPI_Allreduce(send_buffer, recv_buffer, ARRAY_SIZE, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        if (rank == 0) printf("Process %d: Completed AllReduce\n", rank);

        MPI_Barrier(MPI_COMM_WORLD);

        // Perform point-to-point communication
        if (rank == 0) {
            printf("Process 0: Sending to process %d\n", size - 1);
            MPI_Send(send_buffer, ARRAY_SIZE, MPI_DOUBLE, size - 1, 0, MPI_COMM_WORLD);
            printf("Process 0: Receiving from process %d\n", size - 1);
            MPI_Recv(recv_buffer, ARRAY_SIZE, MPI_DOUBLE, size - 1, 0, MPI_COMM_WORLD, &status);
            printf("Process 0: Completed point-to-point communication\n");
        } else if (rank == size - 1) {
            printf("Process %d: Receiving from process 0\n", rank);
            MPI_Recv(recv_buffer, ARRAY_SIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
            printf("Process %d: Sending to process 0\n", rank);
            MPI_Send(send_buffer, ARRAY_SIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
            printf("Process %d: Completed point-to-point communication\n", rank);
        }

        MPI_Barrier(MPI_COMM_WORLD);

        if (rank == 0) {
            printf("Completed iteration %d\n", iteration);
        }

        iteration++;
    }

    end_time = MPI_Wtime();
    elapsed_time = end_time - start_time;

    if (rank == 0) {
        printf("Test completed. Ran for %.2f seconds\n", elapsed_time);
        printf("Performed %d iterations\n", iteration);
        printf("Average time per iteration: %.6f seconds\n", elapsed_time / iteration);
    }

    free(send_buffer);
    free(recv_buffer);

    MPI_Finalize();
    return 0;
}
