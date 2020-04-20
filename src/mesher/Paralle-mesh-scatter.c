/*****************************************************************************
                                                                             
             This file is a part of top-ii-vol meshing tools.                         
                                                                             
     -------------------------------------------------------------------     
                                                                             
     Author(s): Mohd Afeef Badri                                             
     Email    : mohd-afeef.badri@hotmail.com                                                              
     Date     : 2019‑12‑21                                                  
                                                                           
     -------------------------------------------------------------------

     top-ii-vol  provides  sequential  and  parallel tools for  creating  
     volumic tetrahedral meshes from a topology defined by a point cloud.
     top-ii-vol  is  distributed in the hope that it will be useful, but 
     WITHOUT  ANY  WARRANTY; or  without  even  the  implied warranty of 
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*******************************************************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>

float **alloc2d(int n, int m) {
    float *data = malloc(n*m*sizeof(float));
    float **array = malloc(n*sizeof(float *));
    for (int i=0; i<n; i++)
        array[i] = &(data[i*m]);
    return array;
}

int main(int argc, char **argv) {

//-----------------------------------------------------------------------------------//
//---- Global Variables -----
//-----------------------------------------------------------------------------------//

    char filepath[256];

    int ierr 	 ;
    int rank 	 ;
    int size 	 ;
    int NPnt     ;


    int startrow ;
    int endrow   ;
    int locnrows ;
    int locNPnt  ;
    int nrows    ;

    float **data ;

    const int charspernum = 41 ;

//-----------------------------------------------------------------------------------//
//---- MPI variables -----
//-----------------------------------------------------------------------------------//

    MPI_Offset 	 offset		;
    MPI_File   	 file		;
    MPI_File   	 file1		;
    MPI_Status 	 status		;
    MPI_Datatype num_as_string	;
    MPI_Datatype localarray	;
    MPI_Request request;

    ierr = MPI_Init(&argc, &argv);
    ierr|= MPI_Comm_size(MPI_COMM_WORLD, &size);
    ierr|= MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Type_contiguous(charspernum, MPI_CHAR, &num_as_string); 
    MPI_Type_commit(&num_as_string);

    double t1 = MPI_Wtime();

//-----------------------------------------------------------------------------------//
//---- Global Parameters -----
//-----------------------------------------------------------------------------------//

    int pntx=32;
    int pnty=29;

//-----------------------------------------------------------------------------------//
//---- Calculating Parameters -----
//-----------------------------------------------------------------------------------//

    NPnt = pntx * pnty						      ;

//=============================================================================
// ------------- Commandline logo output ------------------
//=============================================================================

    if(rank==0){
     #include "./../lib/LogoTopiiVolC.h"
    }

//-----------------------------------------------------------------------------------//
//---- Data allocation -----
//-----------------------------------------------------------------------------------//

    nrows = NPnt                   ;
    locnrows = NPnt/size;
    startrow = rank * locnrows;
    endrow = startrow + locnrows - 1	;
    if (rank == size-1) {
        endrow = nrows - 1;
        locnrows = endrow - startrow + 1;
    }

//-----------------------------------------------------------------------------------//
//---- create a type describing our piece of the array (Points) -----
//-----------------------------------------------------------------------------------//

    nrows = NPnt		      ;
    int globalsizes[2] = {nrows   , 1};
    int localsizes [2] = {locnrows, 1};
    int starts[2]      = {startrow, 0};
    int order          = MPI_ORDER_C  ;

    MPI_Type_create_subarray(2, globalsizes, localsizes, 
                             starts, order, num_as_string, 
                             &localarray);

    MPI_Type_commit(&localarray);

//-----------------------------------------------------------------------------------//
//---- open the file, and set the view -----
//-----------------------------------------------------------------------------------//

    MPI_File_open( MPI_COMM_WORLD, "./../../data/DEM_160m.xyz", 
                   MPI_MODE_RDONLY,
                   MPI_INFO_NULL, &file);


    snprintf (filepath, sizeof(filepath), "test-xxxwr_%d.xyz",  rank);

    MPI_File_open( MPI_COMM_SELF, filepath, 
                   MPI_MODE_CREATE | MPI_MODE_RDWR,
                   MPI_INFO_NULL, &file1 );

//-----------------------------------------------------------------------------------//
//---- Set file view -----
//-----------------------------------------------------------------------------------//

    offset = 0;

    MPI_File_set_view( file, offset,  MPI_CHAR, localarray, 
                           "native", MPI_INFO_NULL );

//-----------------------------------------------------------------------------------//
//---- Pointdata writing -----
//-----------------------------------------------------------------------------------//

    char *data_as_txt = malloc(locnrows*1*charspernum*sizeof(char));

    //printf("rank %d localrow %d startrow %d\n",rank,locnrows,startrow);

    MPI_File_iread( file, data_as_txt, locnrows*1, num_as_string,  &request ); 
    MPI_Wait( &request, &status );

    MPI_File_iwrite( file1, data_as_txt, locnrows*1, num_as_string, &request );
    MPI_Wait( &request, &status );

//-----------------------------------------------------------------------------------//
//---- Freeup memory -----
//-----------------------------------------------------------------------------------//

    free(data_as_txt);
    MPI_Type_free(&localarray);
    MPI_File_close(&file);
    MPI_File_close(&file1);
    MPI_Type_free(&num_as_string);

    MPI_Finalize();
    return 0;
}
