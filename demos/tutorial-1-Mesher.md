


###  `topIIvol_Mesher`

This is a sequential computing tool. This tool takes in a point-cloud as an input (`.xyz`) and generates volumetric meshes that can be extracted in Gmsh's `*.msh` format or medit's `*.mesh` and `*.meshb` format.

This is the sequential mesher 

- For  sequential mesher producing  `*.mesh` mesh.

  ```bash
  topIIvol_Mesher --xpoints 32 --ypoints 29 --zpoints 15 --depth -1000 \
  --in ./../etc/DEM_160m.xyz --out out-mesh.mesh --mesh mesh
  ```
  
- For  sequential mesher producing  `*.msh` mesh.

  ```bash
  topIIvol_Mesher ---xpoints 32 --ypoints 29 --zpoints 15 --depth -1000 \
  --in ./../etc/DEM_160m.xyz --out out-mesh.msh  --mesh msh
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

To report bugs, issues, feature-requests contact:* 

- **mohd-afeef.badri@cea.fr**
- **mohd-afeef.badri@hotmail.com**
- **mohd-afeef.badri@etu.univ-nantes.fr** 
