LIST_COMPILE= top-ii-vol.cpp

ffCXX=ff-c++

NP=3
	
all: top-ii-vol.so
	@echo " *============================================================*"
	@echo ""
	@echo " Finished installing topIIvol compilation"
	@echo ""
	@echo " Compilation list $(LIST_COMPILE)"
	@echo ""
	@echo " *============================================================*"
	@echo ""

top-ii-vol.so: top-ii-vol.cpp  ./../lib/TopiiVolPartAlgo1D2D.hpp ./../lib/TopiiVolMeshAlgo.hpp ./../lib/LogoTopiiVolCpp.hpp
	$(ffCXX) -auto -mpi top-ii-vol.cpp
	@echo ""


test-freefem: top-ii-vol.so ./../../test/test.edp
	ff-mpirun -np $(NP) ../../test/test.edp -v 0
	paraview vtk_$(NP).pvd    

clean:
	@echo ""
	@echo "*============================================================*"
	@echo " Cleaning files in :"; pwd
	@echo "*============================================================*"
	@echo ""
	rm  -f *~  *.o *.so *.vtu *.pvd *.xyz *.info *.mesh *.meshb *.khi
	@echo ""
