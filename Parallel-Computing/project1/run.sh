make clean
make all
mpirun --map-by node -n 2 ./mpi_hello_world 100000