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
    int ierr, rank, size;
    MPI_Offset offset;
    MPI_File   file;
    MPI_Status status;
    MPI_Datatype num_as_string;
    MPI_Datatype localarray;

    int ncols=3;
    float **data;

    char *const fmt0="%-13d ";
    char *const fmt="%-13.6f  ";
    char *const endfmt="%-13.6f\n";
    char *const endfmt1="%-13d\n";
    int startrow, endrow, locnrows;

    FILE *infile;
 

//--------------------------------------//
//---- Global Variables -----
//--------------------------------------//

int counter1	;

double xx	;
double yy	;
double zz	;
double delz	;
double zznew	;


//--------------------------------------//
//---- Global Parameters -----
//--------------------------------------//
double 	zmin=240.0000		,
       	zmax=-1920.0000		,
	zdiviation=50.		;

int 	pntx=5			,
	pnty=4			,
	pntz=2			;

//--------------------------------------//
//---- Calculating Parameters -----
//--------------------------------------//
int TotNodes = pntx * pnty * pntz		;
int TotTrigl = ((pntz-1) * (pntx-1) * 4)  + ((pnty-1) * (pntz-1) * 4) + ((pntx-1) * (pnty-1) * 4);
int TotTetra = (pntx-1) * (pnty-1) * (pntz-1) * 6;
int TotElemt = TotTetra + TotTrigl;


counter1=1;

    const int charspernum=14;
    const int charspernum1=14;


    const int charspernum00=5;


    ierr = MPI_Init(&argc, &argv);
    ierr|= MPI_Comm_size(MPI_COMM_WORLD, &size);
    ierr|= MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    const int nrows=10*size*pntz;

    locnrows = nrows/size;
    startrow = rank * locnrows;
    endrow = startrow + locnrows - 1;
    if (rank == size-1) {
        endrow = nrows - 1;
        locnrows = endrow - startrow + 1;
    }

    /* allocate local data */
    data = alloc2d(locnrows, ncols);

    char filepath[256];
    snprintf (filepath, sizeof(filepath), "./../data/CoarseMesh_%d.xyz", rank);
    infile = fopen(filepath,"r"); 

    /* fill local data */
    for (int i=0; i<10; i++){
        //for (int j=0; j<ncols; j++){
	    for (int k=0; k<pntz; k++){
	    if(k==0){
	      fscanf(infile,"%f",&data[i*pntz][0]);
	      fscanf(infile,"%f",&data[i*pntz][1]);
	      fscanf(infile,"%f",&data[i*pntz][2]);
              delz= (zmax-data[i*pntz][2])/(pntz-1);
              zznew=data[i*pntz][2];
            }
	    else{ 
              data[i*pntz+k][0]=data[i*pntz][0];
              data[i*pntz+k][1]=data[i*pntz][1];
              zznew  = zznew + delz;
              data[i*pntz+k][2]=zznew;
            }
	//}
           }
   }

 
    /* each number is represented by charspernum chars */
    MPI_Type_contiguous(charspernum, MPI_CHAR, &num_as_string); 
    MPI_Type_commit(&num_as_string); 

    /* convert our data into txt */
    char *data_as_txt = malloc(locnrows*(ncols)*charspernum*sizeof(char) + locnrows*charspernum1*sizeof(char));
    int totcar = (charspernum1+3*charspernum) *sizeof(char);


    int count = 0;
    for (int i=0; i<locnrows; i++) {
        for (int j=0; j<ncols; j++) {
	    sprintf(&data_as_txt[i*totcar+j*charspernum], fmt, data[i][j]);	
        }
        sprintf(&data_as_txt[i*totcar+(ncols)*charspernum], endfmt1, rank);
    }
    free(data[0]);
    free(data);




    ncols++;

    /* create a type describing our piece of the array */
    int globalsizes[2] = {nrows, ncols};
    int localsizes [2] = {locnrows, ncols};
    int starts[2]      = {startrow, 0};
    int order          = MPI_ORDER_C;

    MPI_Type_create_subarray(2, globalsizes, localsizes, starts, order, num_as_string, &localarray);
    MPI_Type_commit(&localarray);

    /* open the file, and set the view */
    MPI_File_open(MPI_COMM_WORLD, "all-data.mesh", 
                  MPI_MODE_CREATE|MPI_MODE_WRONLY,
                  MPI_INFO_NULL, &file);

    MPI_File_set_view(file, 0,  MPI_CHAR, localarray, 
                           "native", MPI_INFO_NULL);
    int nbvert=100;
    if(rank==0){
      char testchar[58];
      snprintf (testchar, sizeof(testchar), "MeshVersionFormatted 1\n\nDimension 3\n\nVertices\n%-10d\n", rank);   
      MPI_File_write(file, testchar, sizeof(testchar), MPI_CHAR, &status);
    }


    MPI_File_set_view(file, 57,  MPI_CHAR, localarray, 
                           "native", MPI_INFO_NULL);

    MPI_File_write_all(file, data_as_txt, locnrows*ncols, num_as_string, &status);

    if(rank==0){
      char testchar1[24];
      snprintf (testchar1, sizeof(testchar1), "\nTetrahedra\n%-10d\n", rank);
      MPI_File_write(file, testchar1, sizeof(testchar1), MPI_CHAR, &status);
    }


    MPI_File_set_view(file, 57+totcar*nrows+23,  MPI_CHAR, localarray, 
                           "native", MPI_INFO_NULL);




    MPI_File_write_all(file, data_as_txt, locnrows*ncols, num_as_string, &status);

    MPI_File_set_view(file, 57+totcar*nrows+23+totcar*nrows,  MPI_CHAR, localarray, 
                           "native", MPI_INFO_NULL);

    if(rank==0){
      char testchar2[23];
      snprintf (testchar2, sizeof(testchar2), "\nTriangles\n%-10d\n", rank);
      MPI_File_write(file, testchar2, sizeof(testchar2), MPI_CHAR, &status);
    }

    MPI_File_set_view(file, 57+totcar*nrows+23+totcar*nrows+22,  MPI_CHAR, localarray, 
                           "native", MPI_INFO_NULL);

    MPI_File_write_all(file, data_as_txt, locnrows*ncols, num_as_string, &status);


    MPI_File_set_view(file, 57+totcar*nrows+23+totcar*nrows+22+totcar*nrows,  MPI_CHAR, localarray, 
                           "native", MPI_INFO_NULL);
    if(rank==0){
      char testchar3[5];
      snprintf (testchar3, sizeof(testchar3), "\nEnd");
      MPI_File_write_all(file, testchar3, sizeof(testchar3)-1, MPI_CHAR, &status);
    }


    MPI_File_close(&file);

    MPI_Type_free(&localarray);
    MPI_Type_free(&num_as_string);

    MPI_Finalize();
    return 0;
}
