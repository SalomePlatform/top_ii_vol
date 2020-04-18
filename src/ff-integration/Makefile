LIST_COMPILE= top-ii-vol.cpp

ffCXX=ff-c++

NP=4
	
all: top-ii-vol.so
	@echo "  			                              "
	@echo " Finished installing top-ii-vol compilation"
	@echo "  			                              "
	@echo " Compilation list $(LIST_COMPILE)          "

top-ii-vol.so: top-ii-vol.cpp
	$(ffCXX) -auto 	top-ii-vol.cpp


test-freefem: ./../../test/test.edp
	ff-mpirun -np $(NP) ../../test/test.edp -v 0
	paraview vtk_$(NP).pvd    

clean:
	@echo "  			            "
	@echo " Cleaning in progress ..."
	@echo "  			            "
	@echo "  			            "
	rm  -f *~  top-ii-vol-Mesh top-ii-vol-Part *.o *.so *.vtu *.pvd *.xyz *.info *.mesh *.meshb *.khi
	rm  -rf VTUs 
