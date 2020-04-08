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

    double t1 = MPI_Wtime();

//-----------------------------------------------------------------------------------//
//---- Global Variables -----
//-----------------------------------------------------------------------------------//

    int ierr 	 ;
    int rank 	 ;
    int size 	 ;

    int NPnt     ;
    int NTri     ;
    int NTet     ;
    int label 	 ;

    int	IJK	 ;
    int	Ip1JK	 ;
    int	IJp1K	 ;
    int	IJKp1	 ;
    int	Ip1JKp1	 ;
    int	IJp1Kp1	 ;
    int	Ip1Jp1K	 ;
    int	Ip1Jp1Kp1;
    int dummycount;

    int startrow ;
    int endrow   ;
    int locnrows ;
    int locNPnt  ;
    int nrows    ;

    float **data ;

    float xx	 ;
    float yy	 ;
    float zz	 ;
    float delz	 ;
    float zznew  ;

    const int charspernum = 41 ;

    char *const fmt1      = "%-13d "     ;
    char *const endfmt1   = "%-13d\n"    ;
    char *const fmt       = "%-13.6f   " ;
    char *const endfmt    = "%-13d\n"    ;
    char *const fmtint    = "%-11d "     ;
    char *const endfmtint = "%-7d\n"     ;

    FILE *infile;

//-----------------------------------------------------------------------------------//
//---- MPI variables -----
//-----------------------------------------------------------------------------------//

    MPI_Offset 	 offset		;
    MPI_File   	 file		;
    MPI_File   	 file1		;
    MPI_Status 	 status		;
    MPI_Datatype num_as_string	;
    MPI_Datatype localarray	;

    ierr = MPI_Init(&argc, &argv);
    ierr|= MPI_Comm_size(MPI_COMM_WORLD, &size);
    ierr|= MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Type_contiguous(charspernum, MPI_CHAR, &num_as_string); 
    MPI_Type_commit(&num_as_string);

//-----------------------------------------------------------------------------------//
//---- Global Parameters -----
//-----------------------------------------------------------------------------------//

    double zmax = -1920.0;

    int pntx=120;
    int pnty=113;
    int	pntz=51;

//-----------------------------------------------------------------------------------//
//---- Calculating Parameters -----
//-----------------------------------------------------------------------------------//

    NPnt = pntx * pnty						      ;
    NTri = 4*((pntz-1)*(pntx-1)+(pnty-1)*(pntz-1)+(pntx-1)*(pnty-1))  ;
    NTet = (pntx-1) * (pnty-1) * (pntz-1) * 6			      ;
    locNPnt = 	(pntx * pnty)/size ;

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

    MPI_Type_create_subarray(2, globalsizes, localsizes, starts, order, num_as_string, &localarray);
    MPI_Type_commit(&localarray);

//-----------------------------------------------------------------------------------//
//---- open the file, and set the view -----
//-----------------------------------------------------------------------------------//

    MPI_File_open(MPI_COMM_WORLD, "CoarseMesh-Skip500.xyz", 
                  MPI_MODE_RDONLY,
                  MPI_INFO_NULL, &file);

    MPI_File_open(MPI_COMM_WORLD, "test-wr.xyz", 
                  MPI_MODE_CREATE|MPI_MODE_WRONLY,
                  MPI_INFO_NULL, &file1);

//-----------------------------------------------------------------------------------//
//---- Header writing -----
//-----------------------------------------------------------------------------------//


    if(rank==0)printf("\n Writing mesh points ");


    offset = 0;
    MPI_File_set_view(file, offset,  MPI_CHAR, localarray, 
                           "native", MPI_INFO_NULL);

    MPI_File_set_view(file1, offset,  MPI_CHAR, localarray, 
                           "native", MPI_INFO_NULL);

//-----------------------------------------------------------------------------------//
//---- Pointdata writing -----
//-----------------------------------------------------------------------------------//
    char *data_as_txt = malloc(locnrows*1*charspernum*sizeof(char));

    MPI_File_read_all(file, data_as_txt, locnrows*1, num_as_string, &status);

    MPI_File_write_all(file1, data_as_txt, locnrows*1, num_as_string, &status);
    free(data_as_txt);
    MPI_Type_free(&localarray);

    MPI_Barrier(MPI_COMM_WORLD);

    if(rank==0)
	printf(" ---- Done\n");

    MPI_File_close(&file);
    MPI_Type_free(&num_as_string);

    MPI_Finalize();
    return 0;
}
