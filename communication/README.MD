Run:

```
mpicxx -o bcast bcast.cpp create_comm.cpp
mpirun -np 8 ./bcast 1
hoặc
mpirun -np 9 ./bcast 2
mpirun -np 8 ./bcast 3
```