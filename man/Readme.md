# Introduction

Top2Vol meshing tool provides sequential/parallel tools for creating volumic tetrahedral meshes from topology (point cloud `*.xyz`). The volumic meshes can be extracted in Gmsh's `*.msh` format or medit's `*.mesh` format.  The framwork is written in C and C++, and uses MPI I/O for parallleization.  



## Compilation process ##

Plan is to build Automake compilation for multiplatform builds but for now the manual setup shoul be followed:

#### Top2Vol-preproc

It has two main fuctions 

- Provides way to coarsen the point cloud by skipping points.
- Provides point cloud scattering (Necessary for parallel for now).  

To compile and build the executable 

```
g++ -c Top2Vol-preproc.cpp ; g++ -o Top2Vol-preproc Top2Vol-preproc.o
```

#### Top2Vol-Mesher

This is a sequential mesher that can output `.msh` and `.mesh` format tetrahedral meshes.

To compile and build the executable 

```
g++ -c Top2Vol-Mesher.cpp ; g++ -o Top2Vol-Mesher Top2Vol-Mesher.o
```

#### ParTop2Vol-Mesher

This is a parallel mesher that can output `.msh` and `.mesh` format tetrahedral meshes.

To compile and build the executable 

```
mpicc -c ParTop2Vol-Mesher.c ; mpicc -o ParTop2Vol-Mesher ParTop2Vol-Mesher.o
```



## Running Top2Vol

If the compilation went successfull you should have three tools at your disposal `ParTop2Vol-Mesher` , `Top2Vol-Mesher` , and `Top2Vol-preproc`. These tools can be worked with command line inputs.

##### How to use Top2Vol-preproc ?

- For sequential meshing if you sih to coarsen your mesh 

  ```
  Top2Vol-preproc  --xpoints 2500 --ypoints 2251 --xskip 24 --yskip 24 --in DEM_2m_new.xyz --out out-coarse.xyz --scatter no
  ```

- For parallel meshing using 3 processes 

  ```
  Top2Vol-preproc  --xpoints 2500 --ypoints 2251 --xskip 24 --yskip 24 --in DEM_2m_new.xyz --out out-coarse.xyz --scatter no --strips 3
  ```

*Command-line oprtion definitions*

| Option      | Type       | Comment                                                      |
| ----------- | ---------- | :----------------------------------------------------------- |
| `--xpoints` | `[int]`    | These are # of x points prent in your point cloud.           |
| `--ypoints` | `[int]`    | These are # of y points prent in your point cloud.           |
| `--xskip`   | `[int]`    | These are # of preodic x points you would like to skip.      |
| `--yskip`   | `[int]`    | These are # of preodic y points you would like to skip.      |
| `--in`      | `[string]` | Sting to provide the input point coloud file `.xyz`          |
| `--out`     | `[string]` | Sting to provide the  output coarsend/stripped point coloud file `.xyz |
| `--scatter` | `[bool]`   | Point cloud partitioner. Use `yes` for paralle and `no ` for sequential. |
| `--strips`  | `[int]`    | Number of MPI ranks to be used in the parallel run.          |

##### How to use Top2Vol-Mesher ?

This is the sequential mesher 

- For  sequential mesher producing  `*.mesh` mesh.

  ```
  ./Top2Vol-Mesher-mesh --xpoints 11 --ypoints 10 --zpoints 11 --in out-coarse.xyz --out out-mesh.mesh
  ```

*Command-line oprtion definitions*

| Option      | Type       | Comment                                              |
| ----------- | ---------- | :--------------------------------------------------- |
| `--xpoints` | `[int]`    | These are # of x points prent in your point cloud.   |
| `--ypoints` | `[int]`    | These are # of y points prent in your point cloud.   |
| `--zpoints` | `[int]`    | These are # of z points intended in the z direction. |
| `--in`      | `[string]` | Sting to provide the input point coloud file `.xyz`  |
| `--out`     | `[string]` | Sting to provide the  output mesh file  `.mesh`      |



##### How to use ParTop2Vol-Mesher ?

This is the parallel mesher 

- For parallel mesher producing  `*.mesh` mesh with 3 MPI ranks.

  ```
  mpirun -n 3 ParTop2Vol-Mesher
  ```

*Command-line oprtion definitions*

| Option      | Type       | Comment                                              |
| ----------- | ---------- | :--------------------------------------------------- |
| `--xpoints` | `[int]`    | These are # of x points prent in your point cloud.   |
| `--ypoints` | `[int]`    | These are # of y points prent in your point cloud.   |
| `--zpoints` | `[int]`    | These are # of z points intended in the z direction. |
| `--in`      | `[string]` | Sting to provide the input point coloud file `.xyz`  |
| `--out`     | `[string]` | Sting to provide the  output mesh file  `.mesh`      |
| `-n`        | `[int]`    | Provide the # of MPI ranks.                          |

*To report bugs, issues, feature-requests contact:* 

- **mohd-afeef.badri@cea.fr**
- **mohd-afeef.badri@outlook.com**
- **mohd-afeef.badri@etu.univ-nantes.fr** 