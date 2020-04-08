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

top-ii-vol meshing tool provides sequential/parallel tools for creating volumic tetrahedral meshes from a topology (point cloud `*.xyz`). The volumic meshes can be extracted in Gmsh's `*.msh` format or medit's `*.mesh` and `*.meshb` format.  The framework is written in C and C++, and uses MPI I/O and MPI for parallelization. One could produce distributed meshes suitable for domain-decomposition based solvers or simply non distributed meshes (single mesh) suitable for a sequential/parallel solver.

top-ii-vol consists a total of five tools:

### `top-ii-vol-PreProc` ###

This tool is a point-cloud preprocessor. Often time point cloud data is huge and requires some alterations. This tool takes in a point-cloud as an input (`.xyz`). It can be used to coarsen a structured point cloud, by skipping specified n number of points. Another role of `top-ii-vol-PreProc` is to slice up a point cloud (stripping) into n number of strips, hence generating a stripped point-cloud. This stripping can be considered analogous to point-cloud partitioning.


### `top-ii-vol-Mesher` ###

This is a sequential computing tool. This tool takes in a point-cloud as an input (`.xyz`) and generates volumic meshes that can be extracted in Gmsh's `*.msh` format or medit's `*.mesh` and `*.meshb` format.


### `top-ii-vol-ParMesher` ###

This is a parallel computing tool. This tool takes in a stripped point-cloud as an input (`.xyz`) and generates volumic meshes that can be extracted in  medit's `*.mesh` and `*.meshb` format. To use this tool one first needs to generate a stripped point-cloud by using top-ii-vol-PreProc`.


### `top-ii-vol-Part` ###

This is point-cloud partitioner needed for creating a distributed volumic mesh. This tool takes in a point-cloud as an input (`.xyz`). One obtains partitioned point cloud using this tool.

### `top-ii-vol-DistMesher` ### 

This is a tool to create embarrassingly parallel distributed meshes, using partitioned point cloud. To generate the partitioned point cloud use `top-ii-vol-Part`.




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

If the compilation went successful you should have three tools at your disposal `top-ii-vol-ParMesher` , `top-ii-vol-Mesher` ,  `top-ii-vol-PreProc`, `top-ii-vol-Part`, and `top-ii-vol-DistMesher`. These tools can be worked with command line inputs. And these tools should be present in `$HOME/top-ii-vol/bin` folder. 


##### How to use top-ii-vol-PreProc ?

- For sequential meshing if you wish to coarsen your mesh by skipping 10 points in x and y direction 

  ```
  ./top-ii-vol-PreProc --xpoints 500 --ypoints 451 --xskip 10 --yskip 10 --in ./../etc/DEM_10m.xyz --out out-coarse.xyz --scatter no
  ```

- For parallel meshing using 2 processes 

  ```
  ./top-ii-vol-PreProc  --xpoints 32 --ypoints 29 --xskip 1 --yskip 1 --in ./../etc/DEM_160m.xyz --out out-coarse-striped.xyz --scatter yes --strips 2
  ```

*Command-line option definitions*

| Option      | Type       | Comment                                                      |
| ----------- | ---------- | :----------------------------------------------------------- |
| `--xpoints` | `[int]`    | These are # of x points present in your point cloud.         |
| `--ypoints` | `[int]`    | These are # of y points present in your point cloud.         |
| `--xskip`   | `[int]`    | These are # of periodic x points you would like to skip.     |
| `--yskip`   | `[int]`    | These are # of periodic y points you would like to skip.     |
| `--in`      | `[string]` | Sting to provide the input point cloud file `.xyz`           |
| `--out`     | `[string]` | Sting to provide the  output coarsend/stripped point cloud file `.xyz |
| `--scatter` | `[bool]`   | Point cloud partitioner. Use `yes` for parallel and `no ` for sequential. |
| `--strips`  | `[int]`    | Number of MPI ranks to be used in the parallel run.          |

*Note that after successfully running `./top-ii-vol-PreProc`there will be a  info file `info-<out-coarse.xyz>.txt` that give the number of x an y points in the coarsened mesh cloud.*





##### How to use top-ii-vol-Mesher ?

This is the sequential mesher 

- For  sequential mesher producing  `*.mesh` mesh.

  ```
  ./top-ii-vol-Mesher --xpoints 50 --ypoints 46 --zpoints 3 --in out-coarse.xyz --out out-mesh.mesh --depth -1000 --mesh mesh
  ```
  
- For  sequential mesher producing  `*.msh` mesh.

  ```
  ./top-ii-vol-Mesher --xpoints 50 --ypoints 46 --zpoints 3 --in out-coarse.xyz --out out-mesh.msh --depth -1000 --mesh msh
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

*Note that if one is using the coarsened mesh # of x points and # of y points should be the one  that are in the info file `info-<out-coarse.xyz>.txt` of the coarsened mesh cloud.*



##### How to use top-ii-vol-ParMesher ?

This is the parallel mesher (still under heavy development)

- For parallel mesher producing  `*.mesh` mesh with 2 MPI ranks.

```
  mpirun -n 2 ./top-ii-vol-ParMesher --xpoints 32 --ypoints 29 --zpoints 5 --depth -2000  --in out-coarse-striped.xyz --out Parallel-out-mesh.mesh
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



##### How to use top-ii-vol-Part ?

This is point cloud partitioning tool

- For parallel producing a partitioned point cloud with 2 partitions.

```
./top-ii-vol-Part --strips 2 --out point-cloud-strip --in ./../etc/DEM_160m --xpoints 32 --ypoints 29
```

*Command-line option definitions*

| Option      | Type       | Comment                                              |
| ----------- | ---------- | :--------------------------------------------------- |
| `--strips`  | `[int]`    | Number of partitions of the point cloud needed.      |
| `--ypoints` | `[int]`    | These are # of y points present in your point cloud. |
| `--zpoints` | `[int]`    | These are # of z points intended in the z direction. |
| `--in`      | `[string]` | Sting to provide the input point cloud file `.xyz`   |
| `--out`     | `[string]` | Sting to provide the  output mesh file  `.mesh`      |




##### How to use top-ii-vol-DistMesher ?

This is  tool to created distributed mesh from  partitioned point cloud

- For parallel distributed mesher producing  `*.mesh` mesh with 2 MPI ranks.

```
  mpirun -np 2 ./top-ii-vol-DistMesher --out top-ii-vol-mesh --zpoints 50
```

| Option      | Type       | Comment                                              |
| ----------- | ---------- | :--------------------------------------------------- |
| `--zpoints` | `[int]`    | These are # of z points intended in the z direction. |
| `--in`      | `[string]` | Sting to provide the input point cloud file `.xyz`   |
| `--out`     | `[string]` | Sting to provide the  output mesh file  `.mesh`      |
| `-np`       | `[int]`   | Provide the # of MPI ranks.                           |





*Note that if one is using the coarsened mesh # of x points and # of y points should be the one  that are in the info file `info-<out-coarse.xyz>.txt` of the coarsened mesh cloud. Also note that the volume will be tagged according to the MPI-Rank*





To report bugs, issues, feature-requests contact:* 

- **mohd-afeef.badri@cea.fr**
- **mohd-afeef.badri@outlook.com**
- **mohd-afeef.badri@etu.univ-nantes.fr** 
