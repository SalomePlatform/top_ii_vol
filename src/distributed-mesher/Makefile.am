FILE1= top-ii-vol-MPImesher.cpp 

all-local: top-ii-vol-DistMesher
	@echo ""
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

top-ii-vol-MPImesher.o: top-ii-vol-MPImesher.cpp ./../lib/TopiiVolPartAlgo.hpp ./../lib/TopiiVolMeshAlgo.hpp ./../lib/LogoTopiiVolCpp.hpp
	$(MPICXX) -c top-ii-vol-MPImesher.cpp -std=c++11

top-ii-vol-DistMesher: top-ii-vol-MPImesher.o
	$(MPICXX) -o top-ii-vol-DistMesher top-ii-vol-MPImesher.o  

check-local:
	mpirun -np 4 ./top-ii-vol-DistMesher --zpoints 50 --xpoints 32 --ypoints 29 --depth -1000 --out top-ii-vol-mesh  --in ./../../data/DEM_160m

test-freefem: check-local ./../../test/reconstruct.edp
	ff-mpirun -np 2 ../../test/reconstruct.edp -v 0
	paraview vtk_2.pvd  

clean-local:
	@echo "*============================================================*"
	@echo " Cleaning files in :"; pwd
	@echo "*============================================================*"
	rm  -f *~  top-ii-vol-DistMesher *.o *.vtu *.pvd *.xyz *.info *.mesh *.txt  *.meshb *.msh *.khi
	rm  -rf VTUs 	

