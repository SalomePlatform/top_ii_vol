LIST_COMPILE = Paralle-mesh-scatter.c \
			ParTop2Vol-Mesher.c \
			skip-mesh-coarsining.cpp \
			Top2Vol-Mesher.cpp \
			Top2Vol-preproc.cpp

FILE1= Paralle-mesh-scatter.c 
FILE2= ParTop2Vol-Mesher.c 
FILE3= skip-mesh-coarsining.cpp 
FILE4= Top2Vol-Mesher.cpp 
FILE5= Top2Vol-preproc.cpp 


MPICXX=mpic++
MPICC=mpicc
	
all: logo top-ii-vol-PreProc top-ii-vol-Mesher top-ii-vol-ParMesher
	@echo ""
	@echo " *============================================================*"
	@echo ""
	@echo " Compilation list:"
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

logo:
	@echo " *============================================================*"
	@echo "         ___                                            ___    " 
	@echo "        /  /               ___________                 /  /    "
	@echo "     __/  /_ ___    ___   /__  __  __/__    __ ____   /  /     " 
	@echo "    /_   __// _  \\ / _  \\   / / / /   \\ \\  / // _  \\ /  / " 
	@echo "     /  /_ / /_/ // /_/ /__/ /_/ /__   \\ \\/ // /_/ //  /__   " 
	@echo "     \\___/ \\____// ____//__________/    \\__/ \\____/ \\____/" 
	@echo "                / /                                            " 
	@echo "               /_/                                             "
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

test-all: logo test-Mesher


test-Mesher:
	./top-ii-vol-PreProc --xpoints 500 --ypoints 451 --xskip 10 --yskip 10 --in ../data/DEM_10m.xyz --out out-coarse.xyz --scatter no
	./top-ii-vol-Mesher --xpoints 50 --ypoints 46 --zpoints 3 --in out-coarse.xyz --out out-mesh.mesh --depth -1000 --mesh mesh


clean:
	@echo "  			            "
	@echo " Cleaning in progress ..."
	@echo "  			            "
	@echo "  			            "
	rm  -f *~  top-ii-vol-Mesh top-ii-vol-Part *.o *.vtu *.pvd *.xyz *.info *.mesh *.meshb
	rm  -rf VTUs 
