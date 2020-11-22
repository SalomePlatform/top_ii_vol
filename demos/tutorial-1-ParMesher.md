


###  `topIIvol_ParMesher`

This is a parallel computing tool. This tool takes in a point-cloud as an input (`.xyz`) and generates volumetric meshes that can be extracted in  medit's `*.mesh` format.

This is the parallel mesher (still under heavy development)

- For parallel mesher producing  `*.mesh` mesh with 2 MPI ranks.

```bash
mpirun -n 2 topIIvol_ParMesher --xpoints 32 --ypoints 29 --zpoints 15 \
--depth -2000 --in ./../etc/DEM_160m.xyz  --out Parallel-out-mesh.mesh
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

To report bugs, issues, feature-requests contact:* 

- **mohd-afeef.badri@cea.fr**
- **mohd-afeef.badri@hotmail.com**
- **mohd-afeef.badri@etu.univ-nantes.fr** 
