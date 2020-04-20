# Changelog
All notable changes to top-ii-vol mesher will be documented in this file.

## Unreleased
### Added
- New header for all files
- Better algorithm for parallel mesher
  - Dimension with Highest number of points gets partitioned
  - Unbalanced partitioning improved  
- Parallel partitioner for point cloud (sequential before)

### Removed
- Merged the partitioner and distributed mesher, no more `top-ii-vol-Part`
- No more `--scatter`, now a part of parallel mesher

### Bug

- proper labeling for `.msh` was missing : now fixed



## [1.1] 2020‑03‑31
### Added
- New MPI based distributed mesher
- New partitioner for distributed mesher
- Fully parallel MPI I/O, now partitioning can be even or unbalaced 
- Moved to automake for cross platform installation 

### Changed
- Name changes from Top2Vol to top-ii-vol
- Larger buffer size written at once to iprove performance 5X

## [1.0] 2019‑12‑19
### Added
- Meshes can now be exported in `.msh` and `.mesh` format
- New Parallel mesher based on MPI I/O
- New tool for preprocessing point cloud

## [0.0] 2019‑10‑09
### Added
- Moved to git 
- Sequential mesher for `.msh` format : `full-mesh.cpp`
