/*****************************************************************************
                                                                             
             This file is a part of top-ii-vol meshing tools.                         
                                                                             
     -------------------------------------------------------------------     
                                                                             
     Author(s): Mohd Afeef Badri                                             
     Email    : mohd-afeef.badri@hotmail.com                                                              
     Date     : 2019‑03‑29
                                                                           
     -------------------------------------------------------------------

     top-ii-vol  provides  sequential  and  parallel tools for  creating  
     volumic tetrahedral meshes from a topology defined by apoint cloud.
     top-ii-vol  is  distributed in the hope that it will be useful, but 
     WITHOUT  ANY  WARRANTY; or  without  even  the  implied warranty of 
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*******************************************************************************/


#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <mpi.h>
using namespace std;


int main(int argc, char **argv){

//-----------------------------------------------------------------------------------//
//---- MPI variables -----
//-----------------------------------------------------------------------------------//

    MPI_Init(&argc, &argv);
    int mpisize, mpirank;
    MPI_Comm_size(MPI_COMM_WORLD, &mpisize);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);

//-----------------------------------------------------------------------------------//
//---- Logo -----
//-----------------------------------------------------------------------------------//

    if(mpirank==0)
     #include "./../lib/LogoTopiiVolCpp.hpp"

//-----------------------------------------------------------------------------------//
//---- I/O Files -----
//-----------------------------------------------------------------------------------//

    string * inputfile  = new string();
    string * outputfile = new string();

    * inputfile  = "point-cloud-strip";
    * outputfile = "Top2Vol-mesh";

//-----------------------------------------------------------------------------------//
//---- Input Parameters -----
//-----------------------------------------------------------------------------------//

    double zmax = -1920.0  ;

    int	pntz=9;
    int pnty=6;
    int pntx=500;  //250;//125;//63;//32;


//-----------------------------------------------------------------------------------//
//---- Comandline Parameters -----
//-----------------------------------------------------------------------------------//

    for(int i=0; i<argc-1; i++){

        string argvdummy=argv[i];

		if ( ! strcmp(argv[i], "--zpoints")) 
	    	pntz = atoi(argv[i+1]);

		if ( ! strcmp(argv[i], "--in"))
            *inputfile  = argv[i+1];

		if ( ! strcmp(argv[i], "--out"))
            *outputfile  = argv[i+1];

		if ( ! strcmp(argv[i], "--depth")) 
	    	zmax= atol(argv[i+1]);	
     }

//-----------------------------------------------------------------------------------//
//---- Time variable -----
//-----------------------------------------------------------------------------------//

    double t1 = MPI_Wtime();

//-----------------------------------------------------------------------------------//
//---- Main algo -----
//-----------------------------------------------------------------------------------//

     #include "./../lib/TopiiVolMeshAlgo.hpp"


//-----------------------------------------------------------------------------------//
//---- Time  -----
//-----------------------------------------------------------------------------------//

   MPI_Barrier(MPI_COMM_WORLD);
   if(mpirank==0){
    cout << "                                                               \n"
         << " *============================================================*\n"
         << std::fixed << std::setprecision(4) 
         <<  "  The program finshed in : " << MPI_Wtime()-t1 << " s\n"
         << " *============================================================*\n"
         << "                                                               \n";
    }


    MPI_Finalize();

return 0;
}
