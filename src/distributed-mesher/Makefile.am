FILE1= top-ii-vol-MPImesher.cpp 

NP=4

all-local: top-ii-vol-DistMesher
	@echo " *============================================================*"
	@echo ""
	@echo " Compilation list in:" ; pwd
	@echo "  # $(FILE1)"
	@echo ""
	@echo " Finished compiling top-ii-vol tools"
	@echo ""
	@echo "  # top-ii-vol-DistMesher-> Point-cloud distributed volume mesher (parallel)"
	@echo ""
	@echo " *============================================================*"

top-ii-vol-MPImesher.o: top-ii-vol-MPImesher.cpp ./../lib/TopiiVolPartAlgo1D2D3D.hpp ./../lib/TopiiVolMeshAlgo.hpp ./../lib/LogoTopiiVolCpp.hpp
	$(MPICXX) -c top-ii-vol-MPImesher.cpp -std=c++11

top-ii-vol-DistMesher: top-ii-vol-MPImesher.o
	$(MPICXX) -o top-ii-vol-DistMesher top-ii-vol-MPImesher.o 
	@echo "" 

check-local: top-ii-vol-MPImesher.o
	mpirun -np $(NP) ./top-ii-vol-DistMesher --zpoints 50 --xpoints 32 --ypoints 29 --depth -1000 --out top-ii-vol-mesh  --in ./../../data/DEM_160m
	mpirun -np $(NP) ./top-ii-vol-DistMesher --zpoints 50 --xpoints 32 --ypoints 29 --depth -1000 --out top-ii-vol-mesh  --in ./../../data/DEM_160m --partition_x 1 --partition_y 4 
	mpirun -np $(NP) ./top-ii-vol-DistMesher --zpoints 50 --xpoints 32 --ypoints 29 --depth -1000 --out top-ii-vol-mesh  --in ./../../data/DEM_160m --partition_x 4 --partition_y 1 
	mpirun -np $(NP) ./top-ii-vol-DistMesher --zpoints 50 --xpoints 32 --ypoints 29 --depth -1000 --out top-ii-vol-mesh  --in ./../../data/DEM_160m --partition_x 2 --partition_y 2 
	mpirun -np $(NP) ./top-ii-vol-DistMesher --zpoints 50 --xpoints 32 --ypoints 29 --depth -1000 --out top-ii-vol-mesh  --in ./../../data/DEM_160m --partition_x 2 --partition_z 2 
	mpirun -np $(NP) ./top-ii-vol-DistMesher --zpoints 50 --xpoints 32 --ypoints 29 --depth -1000 --out top-ii-vol-mesh  --in ./../../data/DEM_160m --partition_y 2 --partition_z 2 	
	mpirun -np $(NP) ./top-ii-vol-DistMesher --zpoints 50 --xpoints 32 --ypoints 29 --depth -1000 --out top-ii-vol-mesh  --in ./../../data/DEM_160m --partition_x 1 --partition_z 4
	mpirun -np $(NP) ./top-ii-vol-DistMesher --zpoints 50 --xpoints 32 --ypoints 29 --depth -1000 --out top-ii-vol-mesh  --in ./../../data/DEM_160m --partition_x 1 --partition_z 2 --partition_y 2
	mpirun -np $(NP) ./top-ii-vol-DistMesher --zpoints 50 --xpoints 32 --ypoints 29 --depth -1000 --out top-ii-vol-mesh  --in ./../../data/DEM_160m --partition_y 1 --partition_z 2 --partition_x 1				
	@echo ""

test-freefem: ./../../test/reconstruct.edp
	ff-mpirun -np $(NP) ../../test/reconstruct.edp -v 0
	paraview vtk_$(NP).pvd
	@echo ""  

clean-local:
	@echo ""
	@echo "*============================================================*"
	@echo " Cleaning files in :"; pwd
	@echo "*============================================================*"
	@echo ""
	rm  -f *~  top-ii-vol-DistMesher *.o *.vtu *.pvd *.xyz
	rm  -f *.xyz *.info *.mesh *.txt  *.meshb *.msh *.khi
	@echo ""
