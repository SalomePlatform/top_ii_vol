FILE1= ParTop2Vol-Mesher.c 
FILE2= skip-mesh-coarsining.cpp 
FILE3= Top2Vol-Mesher.cpp 
FILE4= Top2Vol-preproc.cpp 


all-local:top-ii-vol-PreProc top-ii-vol-Mesher top-ii-vol-ParMesher
	@echo ""
	@echo " *============================================================*"
	@echo ""
	@echo " Compilation list in"; pwd
	@echo "  # $(FILE1)"
	@echo "  # $(FILE2)"
	@echo "  # $(FILE3)"
	@echo "  # $(FILE4)"
	@echo ""
	@echo " Finished compiling top-ii-vol tools"
	@echo ""
	@echo "  # top-ii-vol-PreProc  -> Preprocess point-cloud"
	@echo "  # top-ii-vol-Mesher   -> Point-cloud volume mesher"
	@echo "  # top-ii-vol-ParMesher-> Point-cloud volume mesher (parallel)"
	@echo ""
	@echo " *============================================================*"


Top2Vol-preproc.o: Top2Vol-preproc.cpp
	$(CXX) -c Top2Vol-preproc.cpp -std=c++11	

top-ii-vol-PreProc: Top2Vol-preproc.o
	$(CXX) -o top-ii-vol-PreProc Top2Vol-preproc.o	

Top2Vol-Mesher.o: Top2Vol-Mesher.cpp
	$(CXX) -c Top2Vol-Mesher.cpp -std=c++11	

top-ii-vol-Mesher: Top2Vol-Mesher.o
	$(CXX) -o top-ii-vol-Mesher Top2Vol-Mesher.o 

ParTop2Vol-Mesher.o: ParTop2Vol-Mesher.c ./../lib/TopiiVolPartitionPointCloudAlgo.h
	$(MPICC) -c ParTop2Vol-Mesher.c -std=c99

top-ii-vol-ParMesher: ParTop2Vol-Mesher.o
	$(MPICC) -o top-ii-vol-ParMesher ParTop2Vol-Mesher.o

NP=3
SX=24
SY=24
PZ=15
SX1=105
SY1=94

check-local:
	./top-ii-vol-PreProc --xpoints 500 --ypoints 451 --zpoints 7 --xskip 10 --yskip 10 --in ../../data/DEM_10m.xyz --out out-coarse.xyz
	./top-ii-vol-Mesher --xpoints 50 --ypoints 46 --zpoints 3 --in out-coarse.xyz --out out-mesh.mesh --depth -1000 --mesh mesh
	./top-ii-vol-Mesher --xpoints 50 --ypoints 46 --zpoints 3 --in out-coarse.xyz --out out-mesh.msh --depth -1000 --mesh msh
	mpirun -n $(NP) ./top-ii-vol-ParMesher --xpoints 32 --ypoints 29 --zpoints $(PZ) --depth -3000  --in ./../../data/DEM_160m.xyz --out Parallel-out-mesh.mesh
##	mpirun -n $(NP) ./top-ii-vol-ParMesher --xpoints $(SX1) --ypoints $(SY1) --zpoints $(PZ) --depth -2000  --in out-coarse-striped.xyz --out Parallel-out-mesh.mesh

test-freefem: check-local
	ff-mpirun -np 2 ../../test/Noreconstruct.edp -v 0
	paraview

clean-local:
	@echo "*============================================================*"
	@echo " Cleaning files in :"; pwd
	@echo "*============================================================*"
	rm  -f *~  top-ii-vol-Mesher top-ii-vol-PreProc  top-ii-vol-ParMesher *.o *.vtu *.pvd *.xyz *.info *.mesh *.txt  *.meshb *.msh
	rm  -rf VTUs
