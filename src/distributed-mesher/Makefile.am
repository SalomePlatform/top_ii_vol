FILE1= top-ii-vol-MPImesher.cpp 
FILE2= top-ii-vol-partitioner.cpp 

all-local: top-ii-vol-Part top-ii-vol-DistMesher
	@echo ""
	@echo " *============================================================*"
	@echo ""
	@echo " Compilation list in:" ; pwd
	@echo "  # $(FILE1)"
	@echo "  # $(FILE2)"
	@echo ""
	@echo " Finished compiling top-ii-vol tools"
	@echo ""
	@echo "  # top-ii-vol-Part      -> Point-cloud partioner"
	@echo "  # top-ii-vol-DistMesher-> Point-cloud distributed volume mesher (parallel)"
	@echo ""
	@echo " *============================================================*"

top-ii-vol-partitioner.o: top-ii-vol-partitioner.cpp
	$(CXX) -c top-ii-vol-partitioner.cpp -std=c++11	

top-ii-vol-Part: top-ii-vol-partitioner.o
	$(CXX) -o top-ii-vol-Part top-ii-vol-partitioner.o	

top-ii-vol-MPImesher.o: top-ii-vol-MPImesher.cpp
	$(MPICXX) -c top-ii-vol-MPImesher.cpp -std=c++11

top-ii-vol-DistMesher: top-ii-vol-MPImesher.o
	$(MPICXX) -o top-ii-vol-DistMesher top-ii-vol-MPImesher.o  

check-local:
	./top-ii-vol-Part --strips 2 --out point-cloud-strip --in ./../../data/DEM_160m --xpoints 32 --ypoints 29
	mpirun -np 2 ./top-ii-vol-DistMesher --out top-ii-vol-mesh --zpoints 50

test-freefem: check-local ./../../test/reconstruct.edp
	ff-mpirun -np 2 ../../test/reconstruct.edp -v 0
	paraview vtk_2.pvd  

clean-local:
	@echo "*============================================================*"
	@echo " Cleaning files in :"; pwd
	@echo "*============================================================*"
	rm  -f *~  top-ii-vol-DistMesher top-ii-vol-Part *.o *.vtu *.pvd *.xyz *.info *.mesh *.txt  *.meshb *.msh *.khi
	rm  -rf VTUs 	

