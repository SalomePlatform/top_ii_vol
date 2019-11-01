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
}z

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

    const int charspernum = 14 ;

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

    NPnt = pntx * pnty * pntz					      ;
    NTri = 4*((pntz-1)*(pntx-1)+(pnty-1)*(pntz-1)+(pntx-1)*(pnty-1))  ;
    NTet = (pntx-1) * (pnty-1) * (pntz-1) * 6			      ;
    locNPnt = 	(pntx * pnty)/size ;

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

    data = alloc2d(locnrows, 3);

//-----------------------------------------------------------------------------------//
//---- Gathering point data from partitioned files -----
//-----------------------------------------------------------------------------------//

    if(rank==0)printf("\n Reading the partitioned point cloud mesh");

    char filepath[256];
    snprintf (filepath, sizeof(filepath), "CoarseMesh-Skip500_%d.xyz", rank);
    infile = fopen(filepath,"r"); 

    for (int i=0; i<locNPnt; i++){
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

    if(rank==0)
	printf(" ---- Done\n");

    MPI_Barrier(MPI_COMM_WORLD); 

//-----------------------------------------------------------------------------------//
//---- convert our data into txt -----
//-----------------------------------------------------------------------------------//

    if(rank==0)printf("\n Point cloud mesh data to parallel data conversion");

    char *data_as_txt = malloc(locnrows*4*charspernum*sizeof(char));
    int totcar = 4*charspernum*sizeof(char);

    label=0;

    for (int i=0; i<locnrows; i++) {
        for (int j=0; j<3; j++) {
	    sprintf(&data_as_txt[i*totcar+j*charspernum], fmt, data[i][j]);	
        }
        sprintf(&data_as_txt[i*totcar+3*charspernum], endfmt, label);
    }

    free(data[0]);
    free(data);

    if(rank==0)
	printf(" ---- Done\n");

    MPI_Barrier(MPI_COMM_WORLD);

//-----------------------------------------------------------------------------------//
//---- create a type describing our piece of the array (Points) -----
//-----------------------------------------------------------------------------------//

    nrows = NPnt		      ;
    int globalsizes[2] = {nrows   , 4};
    int localsizes [2] = {locnrows, 4};
    int starts[2]      = {startrow, 0};
    int order          = MPI_ORDER_C  ;

    MPI_Type_create_subarray(2, globalsizes, localsizes, starts, order, num_as_string, &localarray);
    MPI_Type_commit(&localarray);

//-----------------------------------------------------------------------------------//
//---- open the file, and set the view -----
//-----------------------------------------------------------------------------------//

    MPI_File_open(MPI_COMM_WORLD, "test.mesh", 
                  MPI_MODE_CREATE|MPI_MODE_WRONLY,
                  MPI_INFO_NULL, &file);

//-----------------------------------------------------------------------------------//
//---- Header writing -----
//-----------------------------------------------------------------------------------//


    if(rank==0)printf("\n Writing mesh points ");


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

    offset += totcar*NPnt;

    free(data_as_txt);
    MPI_Type_free(&localarray);

    MPI_Barrier(MPI_COMM_WORLD);

    if(rank==0)
	printf(" ---- Done\n");

//-----------------------------------------------------------------------------------//
//---- Tetdata writing -----
//-----------------------------------------------------------------------------------//

    if(rank==0) printf("\n Writing mesh volumes ");

    if(rank==0){
      char testchar1[24];
      snprintf (testchar1, sizeof(testchar1), "\nTetrahedra\n%-10d\n", NTet);
      MPI_File_write(file, testchar1, sizeof(testchar1), MPI_CHAR, &status);
    }

    offset += 23;

    locnrows=NTet/size;

    char *data_as_txt1 = malloc(locnrows*4*charspernum*sizeof(char));

    dummycount=0;
    label=rank;

//    int istart=rank*(pntx-1)/size, iend=rank*(pntx-1)/size + (pntx-1)/size;
//    for(int j=0; j<pnty-1;  j++){
//    for(int i=istart; i<iend;  i++){

    int istart=rank*(pnty-1)/size, iend=rank*(pnty-1)/size + (pnty-1)/size;
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

        sprintf(&data_as_txt1[dummycount*totcar+0*12], fmtint, IJK);
        sprintf(&data_as_txt1[dummycount*totcar+1*12], fmtint, IJKp1);
        sprintf(&data_as_txt1[dummycount*totcar+2*12], fmtint, IJp1K);
        sprintf(&data_as_txt1[dummycount*totcar+3*12], fmtint, Ip1Jp1K);
        sprintf(&data_as_txt1[dummycount*totcar+4*12], endfmtint, label);

        dummycount++;

        sprintf(&data_as_txt1[dummycount*totcar+0*12], fmtint, IJKp1);
        sprintf(&data_as_txt1[dummycount*totcar+1*12], fmtint, IJK);
        sprintf(&data_as_txt1[dummycount*totcar+2*12], fmtint, Ip1JK);
        sprintf(&data_as_txt1[dummycount*totcar+3*12], fmtint, Ip1Jp1K);
        sprintf(&data_as_txt1[dummycount*totcar+4*12], endfmtint, label);

        dummycount++;

        sprintf(&data_as_txt1[dummycount*totcar+0*12], fmtint, Ip1JKp1);
        sprintf(&data_as_txt1[dummycount*totcar+1*12], fmtint, IJKp1);
        sprintf(&data_as_txt1[dummycount*totcar+2*12], fmtint, Ip1JK);
        sprintf(&data_as_txt1[dummycount*totcar+3*12], fmtint, Ip1Jp1K);
        sprintf(&data_as_txt1[dummycount*totcar+4*12], endfmtint, label);

        dummycount++;

        sprintf(&data_as_txt1[dummycount*totcar+0*12], fmtint, IJKp1);
        sprintf(&data_as_txt1[dummycount*totcar+1*12], fmtint, Ip1JKp1);
        sprintf(&data_as_txt1[dummycount*totcar+2*12], fmtint, Ip1Jp1Kp1);
        sprintf(&data_as_txt1[dummycount*totcar+3*12], fmtint, Ip1Jp1K);
        sprintf(&data_as_txt1[dummycount*totcar+4*12], endfmtint, label);

        dummycount++;

        sprintf(&data_as_txt1[dummycount*totcar+0*12], fmtint, IJp1Kp1);
        sprintf(&data_as_txt1[dummycount*totcar+1*12], fmtint, IJKp1);
        sprintf(&data_as_txt1[dummycount*totcar+2*12], fmtint, Ip1Jp1Kp1);
        sprintf(&data_as_txt1[dummycount*totcar+3*12], fmtint, Ip1Jp1K);
        sprintf(&data_as_txt1[dummycount*totcar+4*12], endfmtint, label);

        dummycount++;

        sprintf(&data_as_txt1[dummycount*totcar+0*12], fmtint, IJKp1);
        sprintf(&data_as_txt1[dummycount*totcar+1*12], fmtint, IJp1Kp1);
        sprintf(&data_as_txt1[dummycount*totcar+2*12], fmtint, IJp1K);
        sprintf(&data_as_txt1[dummycount*totcar+3*12], fmtint, Ip1Jp1K);
        sprintf(&data_as_txt1[dummycount*totcar+4*12], endfmtint, label);

        dummycount++;

    }
    }
    }

    nrows = NTet;
    startrow = rank * locnrows;
    endrow = startrow + locnrows - 1	;
    if (rank == size-1) {
        endrow = nrows - 1;
        locnrows = endrow - startrow + 1;
    }

    int globalsizes1[2] = {nrows   , 4};
    int localsizes1 [2] = {locnrows, 4};
    int starts1[2]      = {startrow, 0};
    int order1          = MPI_ORDER_C  ;

    MPI_Datatype localarray1	;
    MPI_Type_create_subarray(2, globalsizes1, localsizes1, starts1, order1, num_as_string, &localarray1);
    MPI_Type_commit(&localarray1);

    MPI_File_set_view(file, offset,  MPI_CHAR, localarray1, 
                           "native", MPI_INFO_NULL);

    MPI_File_write_all(file, data_as_txt1, locnrows*4, num_as_string, &status);

    offset += totcar*NTet;

    if(rank==0)
	printf(" ---- Done\n");

    free(data_as_txt1);
    MPI_Type_free(&localarray1);

    MPI_Barrier(MPI_COMM_WORLD);


//-----------------------------------------------------------------------------------//
//---- Triangle writing -----
//-----------------------------------------------------------------------------------//

    if(rank==0)
	printf("\n Writing mesh surfaces ");

    //NTri=(pnty-1)*(pntz-1)*4+(pnty-1)*(pntx-1)*4+(pnty-1)*(pntz-1)*4+;	

    MPI_File_set_view(file, offset,  MPI_CHAR, localarray, 
                           "native", MPI_INFO_NULL);

    if(rank==0){
      char testchar2[23];
      snprintf (testchar2, sizeof(testchar2), "\nTriangles\n%-10d\n", NTri);
      MPI_File_write(file, testchar2, sizeof(testchar2), MPI_CHAR, &status);
    }

    offset += 22;


    locnrows=NTri/size;
    nrows = NTri;
    startrow = rank * locnrows;
    endrow = startrow + locnrows - 1	;
    if (rank == size-1) {
        endrow = nrows - 1;
        locnrows = endrow - startrow + 1;
    }


    int globalsizes2[2] = {nrows   , 4};
    int localsizes2 [2] = {locnrows, 4};
    int starts2[2]      = {startrow, 0};
    int order2          = MPI_ORDER_C  ;

    MPI_Datatype localarray2	;
    MPI_Type_create_subarray(2, globalsizes2, localsizes2, starts2, order2, num_as_string, &localarray2);
    MPI_Type_commit(&localarray2);

    char *data_as_txt2 = malloc(locnrows*4*charspernum*sizeof(char));

//----X-MIN-PLANE---//
    dummycount=0;
    istart=rank*(pnty-1)/size, iend=rank*(pnty-1)/size + (pnty-1)/size;  // Targeting Y points

    label=11;

    for(int i=istart; i<iend;  i++){
    for(int j=0; j<pntz-1;  j++){

        IJK	  =	i*(pntz*pntx) + j+1	;
        IJKp1	  =	IJK + 1			;
        Ip1JK	  =	IJK + (pntz*pntx)	;
        Ip1JKp1 =	Ip1JK + 1		;

        sprintf(&data_as_txt2[dummycount*totcar+0*charspernum], fmt1, IJKp1);
        sprintf(&data_as_txt2[dummycount*totcar+1*charspernum], fmt1, IJK);
        sprintf(&data_as_txt2[dummycount*totcar+2*charspernum], fmt1, Ip1JK);
        sprintf(&data_as_txt2[dummycount*totcar+3*charspernum], endfmt1, label);

        dummycount++;

        sprintf(&data_as_txt2[dummycount*totcar+0*charspernum], fmt1, Ip1JKp1);
        sprintf(&data_as_txt2[dummycount*totcar+1*charspernum], fmt1, IJKp1);
        sprintf(&data_as_txt2[dummycount*totcar+2*charspernum], fmt1, Ip1JK);
        sprintf(&data_as_txt2[dummycount*totcar+3*charspernum], endfmt1, label);

        dummycount++;
    }
    }


//----Z-MIN-PLANE----//
    label = 22;
    for(int i=istart; i<iend;  i++){
    for(int j=0; j<pntx-1;  j++){

        IJK	  =	i*(pntz*pntx) + j*(pntz)+1	;
        Ip1JK	  =	IJK + (pntz*pntx)		;
        IJp1K	  =	IJK + pntz			;
        Ip1Jp1K =	Ip1JK + pntz			;

        sprintf(&data_as_txt2[dummycount*totcar+0*charspernum], fmt1, IJK);
        sprintf(&data_as_txt2[dummycount*totcar+1*charspernum], fmt1, IJp1K);
        sprintf(&data_as_txt2[dummycount*totcar+2*charspernum], fmt1, Ip1Jp1K);
        sprintf(&data_as_txt2[dummycount*totcar+3*charspernum], endfmt1, label);

        dummycount++;

        sprintf(&data_as_txt2[dummycount*totcar+0*charspernum], fmt1, Ip1JK);
        sprintf(&data_as_txt2[dummycount*totcar+1*charspernum], fmt1, IJK);
        sprintf(&data_as_txt2[dummycount*totcar+2*charspernum], fmt1, Ip1Jp1K);
        sprintf(&data_as_txt2[dummycount*totcar+3*charspernum], endfmt1, label);

        dummycount++;
    }
    }

//----X-MAX-PLANE----//
    label=33;
    for(int i=istart; i<iend;  i++){
    for(int j=0; j<pntz-1;  j++){

        IJK	  =	i*(pntz*pntx) + j+1 + (pntx-1)*(pntz)	;
        IJKp1	  =	IJK + 1					;
        Ip1JK	  =	IJK + (pntz*pntx)			;
        Ip1JKp1 =	Ip1JK + 1				;

        sprintf(&data_as_txt2[dummycount*totcar+0*charspernum], fmt1, IJK);
        sprintf(&data_as_txt2[dummycount*totcar+1*charspernum], fmt1, IJKp1);
        sprintf(&data_as_txt2[dummycount*totcar+2*charspernum], fmt1, Ip1JK);
        sprintf(&data_as_txt2[dummycount*totcar+3*charspernum], endfmt1, label);

        dummycount++;

        sprintf(&data_as_txt2[dummycount*totcar+0*charspernum], fmt1, IJKp1);
        sprintf(&data_as_txt2[dummycount*totcar+1*charspernum], fmt1, Ip1JKp1);
        sprintf(&data_as_txt2[dummycount*totcar+2*charspernum], fmt1, Ip1JK);
        sprintf(&data_as_txt2[dummycount*totcar+3*charspernum], endfmt1, label);

        dummycount++;

    }
    }

//----Z-MAX-PLANE----//
    label=44;
    for(int i=istart; i<iend;  i++){
    for(int j=0; j<pntx-1;  j++){

        IJK	  =	i*(pntz*pntx) + j*(pntz)+1 + (pntz-1)	;
        Ip1JK	  =	IJK + (pntz*pntx)			;
        IJp1K	  =	IJK + pntz				;
        Ip1Jp1K =	Ip1JK + pntz				;

        sprintf(&data_as_txt2[dummycount*totcar+0*charspernum], fmt1, IJp1K);
        sprintf(&data_as_txt2[dummycount*totcar+1*charspernum], fmt1, IJK);
        sprintf(&data_as_txt2[dummycount*totcar+2*charspernum], fmt1, Ip1Jp1K);
        sprintf(&data_as_txt2[dummycount*totcar+3*charspernum], endfmt1, label);

        dummycount++;

        sprintf(&data_as_txt2[dummycount*totcar+0*charspernum], fmt1, IJK);
        sprintf(&data_as_txt2[dummycount*totcar+1*charspernum], fmt1, Ip1JK);
        sprintf(&data_as_txt2[dummycount*totcar+2*charspernum], fmt1, Ip1Jp1K);
        sprintf(&data_as_txt2[dummycount*totcar+3*charspernum], endfmt1, label);

        dummycount++;
    }
    }


    istart=rank*(pntz-1)/size, iend=rank*(pntz-1)/size + (pntz-1)/size;  // Targeting Y points

//----Y-MAX-PLANE----//

    label=55;

    for(int i=0; i<pntx-1;  i++){
    for(int j=istart; j<iend;  j++){

        IJK	  =	i*pntz + j+1 + (pntx*pntz*(pnty-1))	;
        IJKp1	  =	IJK + 1					;
        IJp1K	  =	IJK + pntz				;
        IJp1Kp1 =	IJp1K + 1				;

        sprintf(&data_as_txt2[dummycount*totcar+0*charspernum], fmt1, IJKp1);
        sprintf(&data_as_txt2[dummycount*totcar+1*charspernum], fmt1, IJK);
        sprintf(&data_as_txt2[dummycount*totcar+2*charspernum], fmt1, IJp1K);
        sprintf(&data_as_txt2[dummycount*totcar+3*charspernum], endfmt1, label);

        dummycount++;

        sprintf(&data_as_txt2[dummycount*totcar+0*charspernum], fmt1, IJp1Kp1);
        sprintf(&data_as_txt2[dummycount*totcar+1*charspernum], fmt1, IJKp1);
        sprintf(&data_as_txt2[dummycount*totcar+2*charspernum], fmt1, IJp1K);
        sprintf(&data_as_txt2[dummycount*totcar+3*charspernum], endfmt1, label);

        dummycount++;

    }
    }

//----Y-MIN-PLANE----//

    label=66;

    for(int i=0; i<pntx-1;  i++){
    for(int j=istart; j<iend;  j++){

        IJK	  =	i*pntz + j+1	;
        IJKp1	  =	IJK + 1		;
        IJp1K	  =	IJK + pntz	;
        IJp1Kp1 =	IJp1K + 1	;

        sprintf(&data_as_txt2[dummycount*totcar+0*charspernum], fmt1, IJK);
        sprintf(&data_as_txt2[dummycount*totcar+1*charspernum], fmt1, IJKp1);
        sprintf(&data_as_txt2[dummycount*totcar+2*charspernum], fmt1, IJp1K);
        sprintf(&data_as_txt2[dummycount*totcar+3*charspernum], endfmt1, label);

        dummycount++;

        sprintf(&data_as_txt2[dummycount*totcar+0*charspernum], fmt1, IJKp1);
        sprintf(&data_as_txt2[dummycount*totcar+1*charspernum], fmt1, IJp1Kp1);
        sprintf(&data_as_txt2[dummycount*totcar+2*charspernum], fmt1, IJp1K);
        sprintf(&data_as_txt2[dummycount*totcar+3*charspernum], endfmt1, label);

        dummycount++;

    }
    }

    MPI_File_set_view(file, offset,  MPI_CHAR, localarray2, 
                           "native", MPI_INFO_NULL);

    MPI_File_write_all(file, data_as_txt2, locnrows*4, num_as_string, &status);

    offset += totcar*NTet;

    free(data_as_txt2);
    MPI_Type_free(&localarray2);

    if(rank==0)
	printf(" ---- Done\n");

    MPI_Barrier(MPI_COMM_WORLD);


//-----------------------------------------------------------------------------------//
//---- Footer writing -----
//-----------------------------------------------------------------------------------//

    if(rank==0)
	printf("\n Finalizing the file ");

    MPI_File_set_view(file, offset,  MPI_CHAR, localarray, 
                           "native", MPI_INFO_NULL);

    if(rank==0){
      char testchar3[5];
      snprintf (testchar3, sizeof(testchar3), "\nEnd");
      MPI_File_write(file, testchar3, sizeof(testchar3)-1, MPI_CHAR, &status);
    }

    if(rank==0)
	printf(" ---- Done\n");

//-----------------------------------------------------------------------------------//
//---- Free memory -----
//-----------------------------------------------------------------------------------//

    if(rank==0){
      printf("The program finshed in : %1.2f\n",  MPI_Wtime()-t1);fflush(stdout);}

    MPI_File_close(&file);
    MPI_Type_free(&num_as_string);

    MPI_Finalize();
    return 0;
}
