SUBDIRS = src 
EXTRA_DIST = Readme.md

AUTOMAKE_LIST=autom4te.cache \
config.h \
config.status \
configure \
install-sh \
missing \
Makefile \
Makefile.in \
aclocal.m4 \
config.h.in \
config.log \
stamp-h1 \
depcomp  \
.swp

all-local:
	@echo "*============================================================*"
	@echo " 'make' finished now do 'make check'"
	@echo "*============================================================*"

install-exec-local:
	@mkdir -p ${prefix}/top-ii-vol
	@mkdir -p ${prefix}/top-ii-vol/bin
	@mkdir -p ${prefix}/top-ii-vol/doc
	@mkdir -p ${prefix}/top-ii-vol/etc
	@cp  ./src/mesher/top-ii-vol-PreProc   ${prefix}/top-ii-vol/bin/.
	@cp  ./src/mesher/top-ii-vol-Mesher    ${prefix}/top-ii-vol/bin/.
	@cp  ./src/mesher/top-ii-vol-ParMesher ${prefix}/top-ii-vol/bin/.
	@cp  ./src/distributed-mesher/top-ii-vol-Part   ${prefix}/top-ii-vol/bin/.
	@cp  ./src/distributed-mesher/top-ii-vol-DistMesher   ${prefix}/top-ii-vol/bin/.
	@cp  ./data/DEM_160m.xyz ${prefix}/top-ii-vol/etc/.
	@cp  ./man/Readme.html   ${prefix}/top-ii-vol/doc/.
	@echo "*============================================================*"
	@echo ""
	@echo ""
	@echo " # top-ii-vol tools are now installed at ${prefix}/top-ii-vol/bin"
	@echo ""
	@echo " # top-ii-vol documentations will be present ${prefix}/top-ii-vol/doc"
	@echo ""
	@echo " # top-ii-vol can be tested on a dummy .xyz file in ${prefix}/top-ii-vol/etc"
	@echo ""
	@echo "*============================================================*"		
	
check:
	@echo "*============================================================*"
	@echo " 'make check' finished now do 'make install'"
	@echo "*============================================================*"				

clean-local:
	@echo "*============================================================*"
	@echo " Cleaning files in :"; pwd
	@echo "*============================================================*"
	rm  -f *~
	
clean-automake:
	@echo "*============================================================*"
	@echo " Cleaning all automake files in :"; pwd
	@echo "*============================================================*"
	rm  -rf ${AUTOMAKE_LIST} *~
	rm  -rf ./src/Makefile ./src/Makefile.in	./src/*~
	rm  -rf ./src/top-ii-vol_MPI/Makefile ./src/top-ii-vol_MPI/Makefile.in	./src/top-ii-vol_MPI/*~
