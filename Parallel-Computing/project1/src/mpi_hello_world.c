#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

float * create_rand_nums(int num_elemnts){
	float *rand_nums;
	rand_nums = (float *)malloc(sizeof(float)*num_elemnts);
	assert(rand_nums != NULL);
	for(int i = 0; i < num_elemnts; i++){
		rand_nums[i] = (rand()/(float)RAND_MAX);
	}
	return rand_nums;
}

int main(int argc, char** argv){
	if(argc != 2){
		fprintf(stderr, "Usage: input averge num_elements_per_proc\n");
		exit(1);
	}
	
	int num_elements_per_proc = atoi(argv[1]);
	double tc = MPI_Wtime();

	MPI_Init(NULL, NULL);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	srand(time(NULL)*world_rank);
	float *rand_nums[2];
	rand_nums[0] = create_rand_nums(num_elements_per_proc);
	rand_nums[1] = create_rand_nums(num_elements_per_proc);

	float local_sum = 0;
	for(int i = 0; i < num_elements_per_proc; i ++){
		if(rand_nums[0][i]*rand_nums[0][i] + rand_nums[1][i]*rand_nums[1][i] <= 1)local_sum += 1;
	}

	float global_sum;
  	MPI_Reduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

	if(world_rank == 0){
		printf("number of points belong to circle = %.0f, number of total points = %.0f, avg = %f\n", global_sum, (float)world_size * num_elements_per_proc, global_sum/
						(world_size * num_elements_per_proc)*4.0);
	}

	free(rand_nums[0]);
	free(rand_nums[1]);

	tc = MPI_Wtime() - tc;

	int name_len;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	MPI_Get_processor_name(processor_name, &name_len);
	printf("Hello world from processor %s, rank %d out of %d processor, time = %f ms. \n",processor_name, world_rank, world_size, tc*1000);
	
	MPI_Finalize();
}