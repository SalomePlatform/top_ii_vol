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

    int ierr 	 ;
    int rank 	 ;
    int size 	 ;

    int startrow ;
    int endrow   ;
    int locnrows ;

    float **data ;

    float xx	 ;
    float yy	 ;
    float zz	 ;
    float delz	 ;
    float zznew  ;

    const int charspernum=14;

    char *const fmt    = "%-13.6f   " ;
    char *const endfmt = "%-13d\n"    ;


    FILE *infile;

    MPI_Offset 	 offset		;
    MPI_File   	 file		;
    MPI_Status 	 status		;
    MPI_Datatype num_as_string	;
    MPI_Datatype localarray	;

//-----------------------------------------------------------------------------------//
//---- Global Parameters -----
//-----------------------------------------------------------------------------------//
    double zmax = -1920.0;

    int pntx=6;
    int pnty=5;
    int	pntz=2;

//-----------------------------------------------------------------------------------//
//---- Calculating Parameters -----
//-----------------------------------------------------------------------------------//


    int NPnt = pntx * pnty * pntz					 ;
    int NTri = 4*((pntz-1)*(pntx-1)+(pnty-1)*(pntz-1)+(pntx-1)*(pnty-1)) ;
    int NTet = (pntx-1) * (pnty-1) * (pntz-1) * 6			 ;

    ierr = MPI_Init(&argc, &argv);
    ierr|= MPI_Comm_size(MPI_COMM_WORLD, &size);
    ierr|= MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int Ndiv = 	(pntx * pnty)/size ;
    const int nrows = Ndiv*size*pntz ;

    locnrows = nrows/size;
    startrow = rank * locnrows;
    endrow = startrow + locnrows - 1	;
    if (rank == size-1) {
        endrow = nrows - 1;
        locnrows = endrow - startrow + 1;
    }

    data = alloc2d(locnrows, 3);

//-----------------------------------------------------------------------------------//
//---- Gathering point data from partitioned files -----
//-----------------------------------------------------------------------------------//

    char filepath[256];
    //snprintf (filepath, sizeof(filepath), "./../data/CoarseMesh_%d.xyz", rank);
    snprintf (filepath, sizeof(filepath), "CoarseMesh-Skip500_%d.xyz", rank);
    infile = fopen(filepath,"r"); 

    for (int i=0; i<Ndiv; i++){
        for (int k=0; k<pntz; k++){
            if(k==0){
                fscanf(infile,"%f",&data[i*pntz][0]) ;
                fscanf(infile,"%f",&data[i*pntz][1]) ;
                fscanf(infile,"%f",&data[i*pntz][2]) ;
                delz= (zmax-data[i*pntz][2])/(pntz-1);
                zznew=data[i*pntz][2]                ;
            }
	    else{ 
                data[i*pntz+k][0]=data[i*pntz][0] ;
                data[i*pntz+k][1]=data[i*pntz][1] ;
                zznew  = zznew + delz             ;
                data[i*pntz+k][2]=zznew           ;
            }
        }
    }

    fclose(infile);
 
//-----------------------------------------------------------------------------------//
//---- New MPI data type -----
//-----------------------------------------------------------------------------------//

    MPI_Type_contiguous(charspernum, MPI_CHAR, &num_as_string); 
    MPI_Type_commit(&num_as_string); 

//-----------------------------------------------------------------------------------//
//---- convert our data into txt -----
//-----------------------------------------------------------------------------------//

    char *data_as_txt = malloc(locnrows*4*charspernum*sizeof(char));
    int totcar = 4*charspernum*sizeof(char);

    int NElePnt=0;

    for (int i=0; i<locnrows; i++) {
        for (int j=0; j<3; j++) {
	    sprintf(&data_as_txt[i*totcar+j*charspernum], fmt, data[i][j]);	
        }
        sprintf(&data_as_txt[i*totcar+3*charspernum], endfmt, NElePnt);
    }

    free(data[0]);
    free(data);

//-----------------------------------------------------------------------------------//
//---- create a type describing our piece of the array -----
//-----------------------------------------------------------------------------------//

    int globalsizes[2] = {nrows   , 4};
    int localsizes [2] = {locnrows, 4};
    int starts[2]      = {startrow, 0};
    int order          = MPI_ORDER_C  ;

    MPI_Type_create_subarray(2, globalsizes, localsizes, starts, order, num_as_string, &localarray);
    MPI_Type_commit(&localarray);

//-----------------------------------------------------------------------------------//
//---- open the file, and set the view -----
//-----------------------------------------------------------------------------------//

    MPI_File_open(MPI_COMM_WORLD, "Parallel.mesh", 
                  MPI_MODE_CREATE|MPI_MODE_WRONLY,
                  MPI_INFO_NULL, &file);

//-----------------------------------------------------------------------------------//
//---- Header writing -----
//-----------------------------------------------------------------------------------//

    offset = 0;
    MPI_File_set_view(file, offset,  MPI_CHAR, localarray, 
                           "native", MPI_INFO_NULL);
    if(rank==0){
        char testchar[58];
        snprintf(testchar, sizeof(testchar), 
                "MeshVersionFormatted 1\n\nDimension 3\n\nVertices\n%-10d\n", 
                 NPnt);   
        MPI_File_write(file, testchar, sizeof(testchar), MPI_CHAR, &status);
    }

//-----------------------------------------------------------------------------------//
//---- Pointdata writing -----
//-----------------------------------------------------------------------------------//

    offset += 57;
    MPI_File_set_view(file, offset,  MPI_CHAR, localarray, 
                           "native", MPI_INFO_NULL);

    MPI_File_write_all(file, data_as_txt, locnrows*4, num_as_string, &status);


//-----------------------------------------------------------------------------------//
//---- Tetdata writing -----
//-----------------------------------------------------------------------------------//

    if(rank==0){
      char testchar1[24];
      snprintf (testchar1, sizeof(testchar1), "\nTetrahedra\n%-10d\n", NTet);
      MPI_File_write(file, testchar1, sizeof(testchar1), MPI_CHAR, &status);
    }

    offset += totcar*nrows + 23;
    MPI_File_set_view(file, offset,  MPI_CHAR, localarray, 
                           "native", MPI_INFO_NULL);
int	IJK		,
	Ip1JK		,
	IJp1K		,
	IJKp1		,
	Ip1JKp1		,
	IJp1Kp1		,
	Ip1Jp1K		,
	Ip1Jp1Kp1	;

    locnrows=NTet/size;
    char *data_as_txt1 = malloc(locnrows*4*charspernum*sizeof(char));
    int totcar1 = 4*charspernum*sizeof(char);

    char *const fmtint = "%-11d " ;
    char *const endfmtint = "%-7d\n" ;

/*
    for (int i=0; i<locnrows; i++) {
        for (int j=0; j<4; j++) {
	    int nn=1000000000;
	    sprintf(&data_as_txt1[i*totcar1+j*12], fmtint, nn);	
        }
        sprintf(&data_as_txt1[i*totcar1+4*12], endfmtint, NElePnt);
    }
*/


int istart=rank*(pnty-1)/size, iend=rank*(pnty-1)/size + (pnty-1)/size;
int dummycount=0,label=0;
//for(int j=0; j<pnty-1;  j++){
for(int j=istart; j<iend;  j++){
for(int i=0; i<pntx-1;  i++){

for(int k=1; k<=pntz-1; k++){

  IJK	    =	i*pntz  + j*pntx*pntz + k	;
  Ip1JK	    =	IJK 	+ (pntx*pntz)		;
  IJp1K	    =	IJK 	+ (pntz)		;
  Ip1Jp1K   =	IJK 	+ (pntx*pntz) + pntz	;
  IJKp1     =	IJK 	+ 1			;
  Ip1JKp1   =	Ip1JK 	+ 1			;
  IJp1Kp1   =	IJp1K   + 1			;
  Ip1Jp1Kp1 =	Ip1Jp1K + 1			;

  sprintf(&data_as_txt1[dummycount*totcar1+0*12], fmtint, IJK);
  sprintf(&data_as_txt1[dummycount*totcar1+1*12], fmtint, IJKp1);
  sprintf(&data_as_txt1[dummycount*totcar1+2*12], fmtint, IJp1K);
  sprintf(&data_as_txt1[dummycount*totcar1+3*12], fmtint, Ip1Jp1K);
  sprintf(&data_as_txt1[dummycount*totcar1+4*12], endfmtint, label);

dummycount++;

  sprintf(&data_as_txt1[dummycount*totcar1+0*12], fmtint, IJKp1);
  sprintf(&data_as_txt1[dummycount*totcar1+1*12], fmtint, IJK);
  sprintf(&data_as_txt1[dummycount*totcar1+2*12], fmtint, Ip1JK);
  sprintf(&data_as_txt1[dummycount*totcar1+3*12], fmtint, Ip1Jp1K);
  sprintf(&data_as_txt1[dummycount*totcar1+4*12], endfmtint, label);

dummycount++;

  sprintf(&data_as_txt1[dummycount*totcar1+0*12], fmtint, Ip1JKp1);
  sprintf(&data_as_txt1[dummycount*totcar1+1*12], fmtint, IJKp1);
  sprintf(&data_as_txt1[dummycount*totcar1+2*12], fmtint, Ip1JK);
  sprintf(&data_as_txt1[dummycount*totcar1+3*12], fmtint, Ip1Jp1K);
  sprintf(&data_as_txt1[dummycount*totcar1+4*12], endfmtint, label);

dummycount++;

  sprintf(&data_as_txt1[dummycount*totcar1+0*12], fmtint, IJKp1);
  sprintf(&data_as_txt1[dummycount*totcar1+1*12], fmtint, Ip1JKp1);
  sprintf(&data_as_txt1[dummycount*totcar1+2*12], fmtint, Ip1Jp1Kp1);
  sprintf(&data_as_txt1[dummycount*totcar1+3*12], fmtint, Ip1Jp1K);
  sprintf(&data_as_txt1[dummycount*totcar1+4*12], endfmtint, label);

dummycount++;

  sprintf(&data_as_txt1[dummycount*totcar1+0*12], fmtint, IJp1Kp1);
  sprintf(&data_as_txt1[dummycount*totcar1+1*12], fmtint, IJKp1);
  sprintf(&data_as_txt1[dummycount*totcar1+2*12], fmtint, Ip1Jp1Kp1);
  sprintf(&data_as_txt1[dummycount*totcar1+3*12], fmtint, Ip1Jp1K);
  sprintf(&data_as_txt1[dummycount*totcar1+4*12], endfmtint, label);

dummycount++;

  sprintf(&data_as_txt1[dummycount*totcar1+0*12], fmtint, IJKp1);
  sprintf(&data_as_txt1[dummycount*totcar1+1*12], fmtint, IJp1Kp1);
  sprintf(&data_as_txt1[dummycount*totcar1+2*12], fmtint, IJp1K);
  sprintf(&data_as_txt1[dummycount*totcar1+3*12], fmtint, Ip1Jp1K);
  sprintf(&data_as_txt1[dummycount*totcar1+4*12], endfmtint, label);

dummycount++;

}
}
}

    MPI_File_write_all(file, data_as_txt1, locnrows*4, num_as_string, &status);
/*
//-----------------------------------------------------------------------------------//
//---- Triangle writing -----
//-----------------------------------------------------------------------------------//

    offset += totcar*nrows;
    MPI_File_set_view(file, offset,  MPI_CHAR, localarray, 
                           "native", MPI_INFO_NULL);

    if(rank==0){
      char testchar2[23];
      snprintf (testchar2, sizeof(testchar2), "\nTriangles\n%-10d\n", NTri);
      MPI_File_write(file, testchar2, sizeof(testchar2), MPI_CHAR, &status);
    }

    offset += 22;
    MPI_File_set_view(file, offset,  MPI_CHAR, localarray, 
                           "native", MPI_INFO_NULL);

    MPI_File_write_all(file, data_as_txt, locnrows*4, num_as_string, &status);
*/


//-----------------------------------------------------------------------------------//
//---- Footer writing -----
//-----------------------------------------------------------------------------------//

    offset += totcar*NTet;
    MPI_File_set_view(file, offset,  MPI_CHAR, localarray, 
                           "native", MPI_INFO_NULL);
    if(rank==0){
      char testchar3[5];
      snprintf (testchar3, sizeof(testchar3), "\nEnd");
      MPI_File_write_all(file, testchar3, sizeof(testchar3)-1, MPI_CHAR, &status);
    }

//-----------------------------------------------------------------------------------//
//---- Free memory -----
//-----------------------------------------------------------------------------------//

    MPI_File_close(&file);
    MPI_Type_free(&localarray);
    MPI_Type_free(&num_as_string);

    MPI_Finalize();
    return 0;
}
