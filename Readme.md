# Introduction



## Top2Vol sequential

- Compiling skip-mesh Top2Vol_xyz_coarsining

  ```
  g++ -c skip-mesh-coarsining.cpp ; g++ -o Top2Vol_xyz_coarsining skip-mesh-coarsining.o
  ```

- running 

  ```
  ./Top2Vol_xyz_coarsining
  ```

  ​

## Top2Vol parallel

- scatter with 

  ```
     Enter total  points in  x  [int]   2500
     Enter points to skip in x  [int]   24
     Enter total  points in  y  [int]   2251
     Enter points to skip in y  [int]   24
     Enter the input .xyz file  [char]  ./../data/DEM_2m_new.xyz
     Enter the output file name [char]  outfile-par
  ```

- Then solve with 

  ```
  mpirun -n 3 ./a.out
  ```

  ​