FILE1 = topIIvol_PreProc.cpp 
FILE2 = topIIvol_Mesher.cpp 

MEDCOUPLING_ROOT_DIR=${salome}/INSTALL/MEDCOUPLING
MEDFILE_ROOT_DIR=${salome}/INSTALL/medfile
HDF5_ROOT_DIR=${salome}/INSTALL/hdf5

all-local:topIIvol_PreProc topIIvol_Mesher
	@echo " *============================================================*"
	@echo ""
	@echo " Compilation list in"; pwd
	@echo "  # $(FILE1)"
	@echo "  # $(FILE2)"
	@echo ""
	@echo " Finished compiling topIIvol tools:"
	@echo ""
	@echo "  # topIIvol_PreProc"
	@echo "           A point-cloud preprocessor  (sequential)"
	@echo "  # topIIvol_Mesher"
	@echo "           A point-cloud volume mesher (sequential)"
	@echo ""
	@echo " *============================================================*"

bin_PROGRAMS                = topIIvol_PreProc  topIIvol_Mesher
topIIvol_PreProc_SOURCES    = topIIvol_PreProc.cpp
topIIvol_Mesher_SOURCES     = topIIvol_Mesher.cpp

topIIvol_PreProc.o: topIIvol_PreProc.cpp ./../lib/LogoTopiiVolCpp.hpp
	$(CXX) -c topIIvol_PreProc.cpp -std=c++11	

#top-ii-vol-PreProc: Top2Vol-preproc.o
#	$(CXX) -o top-ii-vol-PreProc Top2Vol-preproc.o
#	@echo ""	

topIIvol_Mesher.o: topIIvol_Mesher.cpp
if HAVESALOME
	export LD_LIBRARY_PATH=$(MEDFILE_ROOT_DIR)/lib:$(HDF5_ROOT_DIR)/lib:$(MEDCOUPLING_ROOT_DIR)/lib
	$(CXX) -c topIIvol_Mesher.cpp -std=c++11 -I$(MEDCOUPLING_ROOT_DIR)/include -I$(MEDFILE_ROOT_DIR)/include -I$(HDF5_ROOT_DIR)/include -L$(MEDCOUPLING_ROOT_DIR)/lib -lmedloader -lmedcoupling -linterpkernel -lmedcouplingremapper $(PDMT_DEBUG) -DMEDCOUPLING
else !HAVESALOME
	$(CXX) -c topIIvol_Mesher.cpp -std=c++11	
	@echo ""
endif

#top-ii-vol-Mesher: Top2Vol-Mesher.o
#	$(CXX) -o top-ii-vol-Mesher Top2Vol-Mesher.o 
#	@echo ""

check-local:
	./topIIvol_PreProc --xpoints 500 --ypoints 451 --zpoints 7 --xskip 10 --yskip 10 --in ../../data/DEM_10m.xyz --out out-coarse.xyz
	./topIIvol_Mesher --xpoints 50 --ypoints 46 --zpoints 3 --in out-coarse.xyz --out out-mesh.mesh --depth -1000 --mesh mesh
	./topIIvol_Mesher --xpoints 50 --ypoints 46 --zpoints 3 --in out-coarse.xyz --out out-mesh.msh --depth -1000 --mesh msh
	@echo ""

NP = 2

test-freefem: check-local
	ff-mpirun -np $(NP) ../../test/NoreconstructSeq.edp -v 0
	paraview

clean-local:
	@echo ""
	@echo "*============================================================*"
	@echo " Cleaning files in :"; pwd
	@echo "*============================================================*"
	@echo ""
	rm  -f topIIvol_Mesher topIIvol_PreProc
	rm  -f *~ *.o *.vtu *.pvd *.xyz *.info *.mesh *.txt  *.meshb *.msh
	@echo ""
