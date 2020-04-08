FILE1= Paralle-mesh-scatter.c 
FILE2= ParTop2Vol-Mesher.c 
FILE3= skip-mesh-coarsining.cpp 
FILE4= Top2Vol-Mesher.cpp 
FILE5= Top2Vol-preproc.cpp 


all-local:top-ii-vol-PreProc top-ii-vol-Mesher top-ii-vol-ParMesher
	@echo ""
	@echo " *============================================================*"
	@echo ""
	@echo " Compilation list in"; pwd
	@echo "  # $(FILE1)"
	@echo "  # $(FILE2)"
	@echo "  # $(FILE3)"
	@echo "  # $(FILE4)"
	@echo "  # $(FILE5)"
	@echo ""
	@echo " Finished compiling top-ii-vol tools"
	@echo ""
	@echo "  # top-ii-vol-PreProc  -> Preprocess point-cloud"
	@echo "  # top-ii-vol-Mesher   -> Point-cloud volume mesher"
	@echo "  # top-ii-vol-ParMesher-> Point-cloud volume mesher (parallel)"
	@echo ""
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

ParTop2Vol-Mesher.o: ParTop2Vol-Mesher.c
	$(MPICC) -c ParTop2Vol-Mesher.c -std=c99

top-ii-vol-ParMesher: ParTop2Vol-Mesher.o
	$(MPICC) -o top-ii-vol-ParMesher ParTop2Vol-Mesher.o


check-local:
	./top-ii-vol-PreProc --xpoints 500 --ypoints 451 --xskip 10 --yskip 10 --in ../../data/DEM_10m.xyz --out out-coarse.xyz --scatter no
	./top-ii-vol-Mesher --xpoints 50 --ypoints 46 --zpoints 3 --in out-coarse.xyz --out out-mesh.mesh --depth -1000 --mesh mesh
	./top-ii-vol-Mesher --xpoints 50 --ypoints 46 --zpoints 3 --in out-coarse.xyz --out out-mesh.msh --depth -1000 --mesh msh
	./top-ii-vol-PreProc  --xpoints 32 --ypoints 29 --xskip 1 --yskip 1 --in ../../data/DEM_160m.xyz --out out-coarse-striped.xyz --scatter yes --strips 2
	mpirun -n 2 ./top-ii-vol-ParMesher --xpoints 32 --ypoints 29 --zpoints 5 --depth -2000  --in out-coarse-striped.xyz --out Parallel-out-mesh.mesh
##	./top-ii-vol-PreProc  --xpoints 2500 --ypoints 2251 --xskip 24 --yskip 24 --in ../../data/DEM_2m.xyz --out out-coarse-striped.xyz --scatter yes --strips 3
##	mpirun -n 3 ./top-ii-vol-ParMesher --xpoints 105 --ypoints 94 --depth -2000  --in out-coarse-striped.xyz --out Parallel-out-mesh.mesh


clean-local:
	@echo "*============================================================*"
	@echo " Cleaning files in :"; pwd
	@echo "*============================================================*"
	rm  -f *~  top-ii-vol-Mesher top-ii-vol-PreProc  top-ii-vol-ParMesher *.o *.vtu *.pvd *.xyz *.info *.mesh *.txt  *.meshb *.msh
	rm  -rf VTUs
