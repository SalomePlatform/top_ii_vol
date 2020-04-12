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

all-local: logo
	@echo "*============================================================*"
	@echo " 'make' finished now do 'make check'"
	@echo "*============================================================*"

logo:
	@echo "*============================================================*"
	@echo "         ___                                            ___    " 
	@echo "        /  /               ___________                 /  /    "
	@echo "     __/  /_ ___    ___   /__  __  __/__    __ ____   /  /     " 
	@echo "    /_   __// _  \\ / _  \\   / / / /   \\ \\  / // _  \\ /  / " 
	@echo "     /  /_ / /_/ // /_/ /__/ /_/ /__   \\ \\/ // /_/ //  /__   " 
	@echo "     \\___/ \\____// ____//__________/    \\__/ \\____/ \\____/" 
	@echo "                / /                                            " 
	@echo "               /_/                                             "
	@echo "*============================================================*"

install-exec-local: logo
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
	@echo " # top-ii-vol can be tested on a dummy .xyz files in ${prefix}/top-ii-vol/etc"
	@echo ""
	@echo "*============================================================*"		
	
check: logo
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
