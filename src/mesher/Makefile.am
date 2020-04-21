FILE1= ParTop2Vol-Mesher.cpp 
FILE2= Top2Vol-Mesher.cpp 
FILE3= Top2Vol-preproc.cpp 

all-local:top-ii-vol-PreProc top-ii-vol-Mesher top-ii-vol-ParMesher
	@echo " *============================================================*"
	@echo ""
	@echo " Compilation list in"; pwd
	@echo "  # $(FILE1)"
	@echo "  # $(FILE2)"
	@echo "  # $(FILE3)"
	@echo ""
	@echo " Finished compiling top-ii-vol tools:"
	@echo ""
	@echo "  # top-ii-vol-PreProc"
	@echo "           A point-cloud preprocessor  (sequential)"
	@echo "  # top-ii-vol-Mesher"
	@echo "           A point-cloud volume mesher (sequential)"
	@echo "  # top-ii-vol-ParMesher"
	@echo "           A point-cloud volume mesher (parallel)"
	@echo ""
	@echo " *============================================================*"


Top2Vol-preproc.o: Top2Vol-preproc.cpp ./../lib/LogoTopiiVolCpp.hpp
	$(CXX) -c Top2Vol-preproc.cpp -std=c++11	

top-ii-vol-PreProc: Top2Vol-preproc.o
	$(CXX) -o top-ii-vol-PreProc Top2Vol-preproc.o
	@echo ""	

Top2Vol-Mesher.o: Top2Vol-Mesher.cpp
	$(CXX) -c Top2Vol-Mesher.cpp -std=c++11	

top-ii-vol-Mesher: Top2Vol-Mesher.o
	$(CXX) -o top-ii-vol-Mesher Top2Vol-Mesher.o 
	@echo ""

ParTop2Vol-Mesher.o: ParTop2Vol-Mesher.cpp ./../lib/TopiiVolPartitionPointCloudAlgo.h ./../lib/LogoTopiiVolC.h
	$(MPICXX) -c ParTop2Vol-Mesher.cpp -std=c++11

top-ii-vol-ParMesher: ParTop2Vol-Mesher.o
	$(MPICXX) -o top-ii-vol-ParMesher ParTop2Vol-Mesher.o
	@echo ""

NP=1
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
	@echo ""

test-freefem: check-local
	ff-mpirun -np $(NP) ../../test/Noreconstruct.edp -v 0
	paraview

clean-local:
	@echo ""
	@echo "*============================================================*"
	@echo " Cleaning files in :"; pwd
	@echo "*============================================================*"
	@echo ""
	rm  -f top-ii-vol-Mesher top-ii-vol-PreProc  top-ii-vol-ParMesher 
	rm  -f *~ *.o *.vtu *.pvd *.xyz *.info *.mesh *.txt  *.meshb *.msh
	@echo ""
