```
            *============================================================*
                     ___                                            ___    
                    /  /               ___________                 /  /    
                 __/  /_ ___    ___   /__  __  __/__    __ ____   /  /
                /_   __// _  \ / _  \   / / / /   \ \  / // _  \ /  / 
                 /  /_ / /_/ // /_/ /__/ /_/ /__   \ \/ // /_/ //  /__
                 \___/ \____// ____//__________/    \__/ \____/ \____/
                            / /                                            
                           /_/                                             
             *============================================================*
```

# Introduction

top-ii-vol meshing tool provides sequential/parallel tools for creating volumetric tetrahedral meshes from a given topology (point-cloud `*.xyz`). The volumetric meshes can be extracted in Gmsh's `*.msh` format or medit's `*.mesh` format.  The framework is written in C++, and uses MPI I/O and MPI for parallelization. One could produce distributed meshes suitable for domain-decomposition based solvers or simply non distributed meshes (single mesh) suitable for a sequential/parallel solver.

top-ii-vol consists a total of four tools:

### 1. `top-ii-vol-PreProc`

This tool is a point-cloud preprocessor. Often point-cloud data is huge and requires some alterations. This tool takes in a point-cloud as an input (`.xyz`). It can be used to coarsen a structured point cloud, by skipping a specified n number of points.


### 2. `top-ii-vol-Mesher`

This is a sequential computing tool. This tool takes in a point-cloud as an input (`.xyz`) and generates volumetric meshes that can be extracted in Gmsh's `*.msh` format or medit's `*.mesh` and `*.meshb` format.


### 3. `top-ii-vol-ParMesher`

This is a parallel computing tool. This tool takes in a point-cloud as an input (`.xyz`) and generates volumetric meshes that can be extracted in  medit's `*.mesh` format.


### 4. `top-ii-vol-DistMesher`

This is a tool to create embarrassingly parallel distributed meshes. The mesher takes in a point-cloud as an input (`.xyz`) and outputs distributed mesh. 



## Installation process ##

Before you begin installing top-ii-vol please check if your system meets the dependencies.

**Dependencies**

- [automake](https://www.gnu.org/software/automake/)
- [C++](http://www.cplusplus.com/)
- [MPI](https://www.mcs.anl.gov/research/projects/mpi/)

**Now that I have all the dependencies what next**

Goto `top-ii-vol-Source` folder and

- step 1
```
autoreconf -i
```

- step 2
```
./configure 
```
*Note*:   `./configure` will install top-ii-vol in `$HOME/top-ii-vol` to change this directory use `--prefix=Your/Own/Path` with `./configure`.

- step 3
```
make
```

- step 4
```
make check
```

- step 5
```
make install
```

## Running top-ii-vol

If the compilation went successful you should have three tools at your disposal `top-ii-vol-ParMesher`, `top-ii-vol-Mesher`,  `top-ii-vol-PreProc`, and `top-ii-vol-DistMesher`. These tools can be worked with command line inputs. And these tools should be present in `$HOME/top-ii-vol/bin` folder. 


##### How to use top-ii-vol-PreProc ?

- If you wish to coarsen your mesh by skipping 10 points in x and y direction 

  ```
  ./top-ii-vol-PreProc --xpoints 500 --ypoints 451 --xskip 10 --yskip 10 --in ./../etc/DEM_10m.xyz --out out-coarse.xyz
  ```

*Command-line option definitions*

| Option      | Type       | Comment                                                      |
| ----------- | ---------- | :----------------------------------------------------------- |
| `--xpoints` | `[int]`    | These are # of x points present in your point cloud.         |
| `--ypoints` | `[int]`    | These are # of y points present in your point cloud.         |
| `--xskip`   | `[int]`    | These are # of periodic x points you would like to skip.     |
| `--yskip`   | `[int]`    | These are # of periodic y points you would like to skip.     |
| `--in`      | `[string]` | Sting to provide the input point cloud file `.xyz`           |
| `--out`     | `[string]` | Sting to provide the  output coarsened/stripped point cloud file `.xyz` |

*Note that after successfully running `./top-ii-vol-PreProc`there will be a  info file `info-<out-coarse.xyz>.txt` that give the number of x an y points in the coarsened mesh cloud.*





##### How to use top-ii-vol-Mesher ?

This is the sequential mesher 

- For  sequential mesher producing  `*.mesh` mesh.

  ```
  ./top-ii-vol-Mesher --xpoints 32 --ypoints 29 --zpoints 15 --in ./../etc/DEM_160m.xyz --out out-mesh.mesh --depth -1000 --mesh mesh
  ```
  
- For  sequential mesher producing  `*.msh` mesh.

  ```
  ./top-ii-vol-Mesher ---xpoints 32 --ypoints 29 --zpoints 15 --in ./../etc/DEM_160m.xyz --out out-mesh.msh --depth -1000 --mesh msh
  ```

*Command-line option definitions*

| Option      | Type       | Comment                                              |
| ----------- | ---------- | :--------------------------------------------------- |
| `--xpoints` | `[int]`    | These are # of x points present in your point cloud. |
| `--ypoints` | `[int]`    | These are # of y points present in your point cloud. |
| `--zpoints` | `[int]`    | These are # of z points intended in the z direction. |
| `--in`      | `[string]` | Sting to provide the input point cloud file `.xyz`   |
| `--out`     | `[string]` | Sting to provide the  output mesh file  `.mesh`      |
| `--depth`   | `[int]`    | This is the depth of the mesh needed.                |
| `--mesh`    | `[string]` | To specify the kind of mesh needed                   |





##### How to use top-ii-vol-ParMesher ?

This is the parallel mesher (still under heavy development)

- For parallel mesher producing  `*.mesh` mesh with 2 MPI ranks.

```
  mpirun -n 2 ./top-ii-vol-ParMesher --xpoints 32 --ypoints 29 --zpoints 15 --depth -2000  --in ./../etc/DEM_160m.xyz  --out Parallel-out-mesh.mesh
```

*Command-line option definitions*

| Option      | Type       | Comment                                              |
| ----------- | ---------- | :--------------------------------------------------- |
| `--xpoints` | `[int]`    | These are # of x points present in your point cloud. |
| `--ypoints` | `[int]`    | These are # of y points present in your point cloud. |
| `--zpoints` | `[int]`    | These are # of z points intended in the z direction. |
| `--in`      | `[string]` | Sting to provide the input point cloud file `.xyz`   |
| `--out`     | `[string]` | Sting to provide the  output mesh file  `.mesh`      |
| `-n`        | `[int]`    | Provide the # of MPI ranks.                          |





##### How to use top-ii-vol-DistMesher ?

This is  tool to created distributed mesh from  partitioned point cloud

- For parallel distributed mesher producing  `*.mesh` mesh with 2 MPI ranks.

```
 mpirun -np 2 ./top-ii-vol-DistMesher  --zpoints 50 --xpoints 32 --ypoints 29 --depth -1000 --out top-ii-vol-mesh  --in ./../etc/DEM_160m
```

*Command-line option definitions*

| Option      | Type       | Comment                                              |
| ----------- | ---------- | :--------------------------------------------------- |
| `--xpoints` | `[int]`    | These are # of x points present in your point cloud. |
| `--ypoints` | `[int]`    | These are # of y points present in your point cloud. |
| `--zpoints` | `[int]`    | These are # of z points intended in the z direction. |
| `--in`      | `[string]` | Sting to provide the input point cloud file `.xyz`   |
| `--out`     | `[string]` | Sting to provide the  output mesh file  `.mesh`      |
| `-np`       | `[int]`   | Provide the # of MPI ranks.                           |



To report bugs, issues, feature-requests contact:* 

- **mohd-afeef.badri@cea.fr**
- **mohd-afeef.badri@hotmail.com**
- **mohd-afeef.badri@etu.univ-nantes.fr** 
