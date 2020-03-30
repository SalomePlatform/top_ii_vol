LIST_COMPILE= top-ii-vol-partitioner.cpp top-ii-vol-MPImesher.cpp

MPICXX=mpic++
	
all: top-ii-vol-Part top-ii-vol-Mesh
	@echo "  			                              "
	@echo " Finished installing top-ii-vol compilation"
	@echo "  			                              "
	@echo " Compilation list $(LIST_COMPILE)          "

top-ii-vol-partitioner.o: top-ii-vol-partitioner.cpp
	$(CXX) -c top-ii-vol-partitioner.cpp -std=c++11	

top-ii-vol-Part: top-ii-vol-partitioner.o
	$(CXX) -o top-ii-vol-Part top-ii-vol-partitioner.o	


top-ii-vol-MPImesher.o: top-ii-vol-MPImesher.cpp
	$(MPICXX) -c top-ii-vol-MPImesher.cpp -std=c++11

top-ii-vol-Mesh: top-ii-vol-MPImesher.o
	$(MPICXX) -o top-ii-vol-Mesh top-ii-vol-MPImesher.o	

test: reconstruct.edp top-ii-vol-partitioner.cpp top-ii-vol-MPImesher.cpp
	@echo " KOKOKOK "
	./top-ii-vol-Part --strips 2 --out point-cloud-strip --in ./../../data/DEM_160m --xpoints 32 --ypoints 29
	mpirun -np 2 ./top-ii-vol-Mesh
	ff-mpirun -np 2 reconstruct.edp
	paraview vtk.pvd    

clean:
	@echo "  			            "
	@echo " Cleaning in progress ..."
	@echo "  			            "
	@echo "  			            "
	rm  -f *~  top-ii-vol-Mesh top-ii-vol-Part *.o *.vtu *.pvd *.xyz *.info *.mesh
	rm  -rf VTUs 
