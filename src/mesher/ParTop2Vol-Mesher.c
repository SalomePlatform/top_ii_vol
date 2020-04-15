/*****************************************************************************
                                                                             
             This file is a part of top-ii-vol meshing tools.                         
                                                                             
     -------------------------------------------------------------------     
                                                                             
     Author(s): Mohd Afeef Badri                                             
     Email    : mohd-afeef.badri@hotmail.com                                                              
     Date     : 2019‑10‑09                                                  
                                                                           
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


int main(int argc, char *argv[]) {

//====================================================================================//
//---- variable to calculate time -----
//====================================================================================//

    double t1 = MPI_Wtime();

//====================================================================================//
//---- Global Variables -----
//====================================================================================//

    int NPnt     ;			    // # of points in the mesh
    int NTri     ;			    // # of triangles on the mesh surfaces
    int NTet     ;			    // # of tetrahedra in the mesh
    int label 	 ;			    // label # of the mesh surfaces

    int	IJK	     ;			    // variable used during tera or triangle generation
    int	Ip1JK	 ;			    // variable used during tera or triangle generation
    int	IJp1K	 ;			    // variable used during tera or triangle generation
    int	IJKp1	 ;			    // variable used during tera or triangle generation
    int	Ip1JKp1	 ;			    // variable used during tera or triangle generation
    int	IJp1Kp1	 ;			    // variable used during tera or triangle generation
    int	Ip1Jp1K	 ;			    // variable used during tera or triangle generation
    int	Ip1Jp1Kp1;			    // variable used during tera or triangle generation
    int dummycount;			    // variable used during tera or triangle generation

    int startrow ;			    // used by MPI ranks to mark their starting row 
    int endrow   ;			    // used by MPI ranks to mark their ending  row
    int locnrows ;			    // used by MPI ranks to know the # of rows held 
    int locNPnt  ;			    // used by MPI ranks to know the # of points held
    int nrows    ;			    // used by MPI ranks to know the total # of rows

    float **data ;			    // array to hold the input data from point cloud xyz

    float xx	 ;			    // variable hold x point value
    float yy	 ;			    // variable hold y point value
    float zz	 ;			    // variable hold z point value
    float delz	 ;			    // variable get delta z according to B/C (Laplacian)
    float zznew  ;			    // variable hold z point value

    const int charspernum = 14 ;

    char *const fmt1      = "%-13d "     ;
    char *const endfmt1   = "%-13d\n"    ;
    char *const fmt       = "%-13.6f   " ;
    char *const endfmt    = "%-13d\n"    ;
    char *const fmtint    = "%-11d "     ;
    char *const endfmtint = "%-7d\n"     ;

    FILE *infile;


//====================================================================================//
//---- MPI variables -----
//====================================================================================//

    int ierr 	 ;              // MPI Variable to capture error
    int mpirank	 ;              // MPI Variable to give MPI rank        
    int mpisize  ;              // MPI Variable to give MPI size

    MPI_Offset 	 offset		    ;
    MPI_File   	 file		    ;
    MPI_Status 	 status		    ;
    MPI_Datatype num_as_string	;
    MPI_Datatype localarray	    ;

    ierr = MPI_Init(&argc, &argv);
    ierr|= MPI_Comm_size(MPI_COMM_WORLD, &mpisize);
    ierr|= MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);

    MPI_Type_contiguous(charspernum, MPI_CHAR, &num_as_string); 
    MPI_Type_commit(&num_as_string);

//====================================================================================//
//---- Commandline logo output -----
//====================================================================================//

    if(mpirank==0){
     #include "./../lib/LogoTopiiVolC.h"
    }

//====================================================================================//
//---- Global Parameters -----
//====================================================================================//

    double zmax    = -1920.0;

    int pntx = 10  ;//=105;
    int pnty = 9   ; //=94;
    int	pntz = 100 ;


    char inpurfile[80] = "out-coarse.xyz"        ;
    char outpufile[80] = "Tetra-ParTop2Vol.mesh" ;

//====================================================================================//
//---- Comandline Parameters -----
//====================================================================================//

    for(int i=0; i<argc-1; i++){

        char argvdummy=*argv[i];

		if ( ! strcmp(argv[i], "--xpoints")) 
	    	pntx= atoi(argv[i+1]);

		if ( ! strcmp(argv[i], "--ypoints")) 
	    	pnty= atoi(argv[i+1]);

		if ( ! strcmp(argv[i], "--zpoints")) 
	    	pntz= atoi(argv[i+1]);

		if ( ! strcmp(argv[i], "--in"))
			strcpy(inpurfile, argv[i+1]); 

		if ( ! strcmp(argv[i], "--out"))
			strcpy(outpufile, argv[i+1]); 

		if ( ! strcmp(argv[i], "--depth")) 
	    	zmax= atol(argv[i+1]);
	
     }

//====================================================================================//
//---- Calculating mesh attributes -----
//====================================================================================//

    NPnt = pntx * pnty * pntz					      ;
    NTri = 4*((pntz-1)*(pntx-1)+(pnty-1)*(pntz-1)+(pntx-1)*(pnty-1))  ;
    NTet = (pntx-1) * (pnty-1) * (pntz-1) * 6			      ;


//====================================================================================//
//---- local point calculation -----
//====================================================================================//

//----------------------------------//
//      balanced       
//----------------------------------//

   locNPnt = (pntx * pnty)/mpisize ;

//----------------------------------//
//     un balanced     
//----------------------------------//

    if( ((pntx * pnty)%mpisize) > 0 )
      if( mpirank < ((pntx * pnty)%mpisize) )
         locNPnt++;

//====================================================================================//
//---- local rows calculation -----
//====================================================================================//

    nrows    = NPnt                     ;
    locnrows = locNPnt * pntz           ;

//====================================================================================//
//---- start and endrow local array -----
//====================================================================================//

//----------------------------------//
//      balanced       
//----------------------------------//

    if( ((pntx * pnty)%mpisize) == 0 ){

      startrow = mpirank * locnrows            ;
      endrow   = startrow + locnrows - 1	;

      if (mpirank == mpisize-1) {
        endrow = nrows - 1;
        locnrows = endrow - startrow + 1;
      }

    }

//----------------------------------//
//     un balanced     
//----------------------------------/

    if( ((pntx * pnty)%mpisize) > 0 ){

      if( mpirank < ((pntx * pnty)%mpisize) ){
        startrow = mpirank *  locNPnt * pntz      ;
        endrow   = startrow + locNPnt * pntz -1;
      }

      if( mpirank >= ((pntx * pnty)%mpisize) ){
        startrow = mpirank *  locNPnt * pntz     + ((pntx * pnty)%mpisize) * pntz    ;
        endrow   = startrow + locNPnt * pntz -1                             	;
      }

   }

//   if( ((pntx * pnty)%mpisize) == 0 )
//     printf("== I %d startrow %d endrow %d  localrow %d \n", mpirank,startrow,endrow,locnrows);

//   if( ((pntx * pnty)%mpisize) > 0 )
//     printf("§§§ I %d startrow %d endrow %d  localrow %d \n", mpirank,startrow,endrow,locnrows);


//====================================================================================//
//---- Data allocation -----
//====================================================================================//

    data = alloc2d(locnrows, 3);

//====================================================================================//
//---- Gathering point data from partitioned files -----
//====================================================================================//

    if(mpirank==0)
    printf("\n Reading the partitioned point cloud mesh");

    char filepath[256];
    snprintf (filepath, sizeof(filepath), "%s_%d.xyz", inpurfile, mpirank);
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

    if(mpirank==0)
	printf(" ---- Done");

//    MPI_Barrier(MPI_COMM_WORLD); 

//====================================================================================//
//---- convert our data into txt -----
//====================================================================================//

    if(mpirank==0)
    printf("\n Point cloud data to parallel data conversion");

    char *data_as_txt = malloc(locnrows*4*charspernum*sizeof(char));
    int totcar = 4*charspernum*sizeof(char);

    label=0;

    for (int i=0; i<locnrows; i++) {
        for (int j=0; j<3; j++) {
	       sprintf(&data_as_txt[i*totcar+j*charspernum], fmt, data[i][j]);	
        }
        sprintf(&data_as_txt[i*totcar+3*charspernum], endfmt, label);
    }

    if( ((pntx * pnty)%mpisize) == 0 )
      free(data);                      //CAUSING ERROR IN SOME MPI RANKS CHECK ?????

    if(mpirank==0)
	printf(" ---- Done\n\n");

    MPI_Barrier(MPI_COMM_WORLD);

//====================================================================================//
//---- create a type describing our piece of the array (Points) -----
//====================================================================================//

    nrows = NPnt		              ;
    int globalsizes[2] = {nrows   , 4};
    int localsizes [2] = {locnrows, 4};
    int starts[2]      = {startrow, 0};
    int order          = MPI_ORDER_C  ;

    MPI_Type_create_subarray(2, globalsizes, localsizes, starts, order, num_as_string, &localarray);
    MPI_Type_commit(&localarray);

//====================================================================================//
//---- open the file, and set the view -----
//====================================================================================//

	char str[80]="";
    strcat (str, outpufile);

    MPI_File_open(MPI_COMM_WORLD, str, 
                  MPI_MODE_CREATE|MPI_MODE_WRONLY,
                  MPI_INFO_NULL, &file);

//====================================================================================//
//---- Header writing -----
//====================================================================================//

    if(mpirank==0)
    printf("\n Writing mesh points ");

    offset = 0;
    MPI_File_set_view(file, offset,  MPI_CHAR, localarray, 
                           "native", MPI_INFO_NULL);
    if(mpirank==0){
        char testchar[58];
        snprintf(testchar, sizeof(testchar), 
                "MeshVersionFormatted 1\n\nDimension 3\n\nVertices\n%-10d\n", 
                 NPnt);   
        MPI_File_write(file, testchar, sizeof(testchar), MPI_CHAR, &status);
    }


//====================================================================================//
//---- Point data writing -----
//====================================================================================//

    offset += 57;
    MPI_File_set_view(file, offset,  MPI_CHAR, localarray, 
                           "native", MPI_INFO_NULL);

    MPI_File_write_all(file, data_as_txt, locnrows*4, num_as_string, &status);

    offset += totcar*NPnt;

    free(data_as_txt);
    MPI_Type_free(&localarray);

    MPI_Barrier(MPI_COMM_WORLD);

    if(mpirank==0)
	printf(" ---- Done\n ->  %d points written ",NPnt);

//====================================================================================//
//---- Tetdata header writing -----
//====================================================================================//

    if(mpirank==0) 
    printf("\n Writing mesh volumes ");

    if(mpirank==0){
      char testchar1[24];
      snprintf (testchar1, sizeof(testchar1), "\nTetrahedra\n%-10d\n", NTet);
      MPI_File_write(file, testchar1, sizeof(testchar1), MPI_CHAR, &status);
    }

    offset += 23;

//====================================================================================//
//---- local row calculation ----
//====================================================================================//

//----------------------------------//
//      balanced       
//----------------------------------//

   if( ((pnty-1)%mpisize) == 0 )
     locnrows=((pntx-1)  * (pntz-1) * 6)*((pnty-1)/mpisize); //NTet/mpisize

//----------------------------------//
//     un balanced       
//----------------------------------//

    if( ((pnty-1)%mpisize) > 0 ){

      if( mpirank < ((pnty-1)%mpisize) )
         locnrows=((pntx-1)  * (pntz-1) * 6)*((pnty-1)/mpisize+1);  //NTet/mpisize

      if( mpirank >= ((pnty-1)%mpisize) )
         locnrows=((pntx-1)  * (pntz-1) * 6)*((pnty-1)/mpisize);  //NTet/size

    }

//====================================================================================//
//---- Data allocation -----
//====================================================================================//

    char *data_as_txt1 = malloc(locnrows*4*charspernum*sizeof(char));

    dummycount=0;
//  label=mpirank;
    label=0;

//====================================================================================//
//---- start and endrow local array -----
//====================================================================================//

    int istart,iend;

   //----------------------------------//
   //      balanced       
   //----------------------------------//

   if( ((pnty-1)%mpisize) == 0 ){
    istart = mpirank*(pnty-1)/mpisize; 
    iend   = mpirank*(pnty-1)/mpisize + (pnty-1)/mpisize;
   }

   //----------------------------------//
   //      un balanced       
   //----------------------------------//

    if( ((pnty-1)%mpisize) > 0 ){
      if( mpirank < ((pnty-1)%mpisize) ){
         istart = mpirank*((pnty-1)/mpisize + 1)  ;
         iend   = istart + ((pnty-1)/mpisize + 1) ;
      }
      if( mpirank >= ((pnty-1)%mpisize) ){
         istart=mpirank*((pnty-1)/mpisize) +   ((pnty-1)%mpisize) ;
         iend=istart + ((pnty-1)/mpisize);
      }
    }
       
//     MPI_Barrier(MPI_COMM_WORLD); 
//    int istart=mpirank*(pnty)/mpisize, iend=mpirank*(pnty)/mpisize + (pnty)/mpisize;

//====================================================================================//
//---- Gathering tetrahedra data -----
//====================================================================================//

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


//====================================================================================//
//---- start and endrow local array -----
//====================================================================================//

    nrows = NTet;

   //----------------------------------//
   //      balanced       
   //----------------------------------//

   if( ((pnty-1)%mpisize) == 0 ){

     startrow = mpirank * locnrows          ;
     endrow = startrow + locnrows - 1	;

     if (mpirank == mpisize-1) {
        endrow = nrows - 1;
        locnrows = endrow - startrow + 1;
     }

   }

   //----------------------------------//
   //     un balanced       
   //----------------------------------//

    if( ((pnty-1)%mpisize) > 0 ){

      if( mpirank < ((pnty-1)%mpisize) ){
        startrow = mpirank * ( ((pntx-1)*(pntz-1)*6) * ((pnty-1)/mpisize+1) )         ;
        endrow  = startrow + ( ((pntx-1)  * (pntz-1) * 6)*((pnty-1)/mpisize+1) ) -1;
      }

      if( mpirank >= ((pnty-1)%mpisize) ){
        startrow = mpirank * ( ((pntx-1)  * (pntz-1) * 6)*((pnty-1)/mpisize)) +   ((pnty-1)%mpisize) * ((pntx-1)  * (pntz-1) * 6)   ;
        endrow   = startrow +  ((pntx-1)  * (pntz-1) * 6)*((pnty-1)/mpisize) -1;
      }
   }

//====================================================================================//
//---- Data allocation -----
//====================================================================================//

    int globalsizes1[2] = {nrows   , 4};
    int localsizes1 [2] = {locnrows, 4};
    int starts1[2]      = {startrow, 0};
    int order1          = MPI_ORDER_C  ;

//====================================================================================//
//---- Tetdata writing -----
//====================================================================================//

    MPI_Datatype localarray1	;
    MPI_Type_create_subarray(2, globalsizes1, localsizes1, starts1, order1, num_as_string, &localarray1);
    MPI_Type_commit(&localarray1);

    MPI_File_set_view(file, offset,  MPI_CHAR, localarray1, 
                           "native", MPI_INFO_NULL);

    MPI_File_write_all(file, data_as_txt1, locnrows*4, num_as_string, &status);

    offset += totcar*NTet;

    if(mpirank==0)
	printf(" ---- Done\n ->  %d tetrahedra written ",NTet);

    free(data_as_txt1);
    MPI_Type_free(&localarray1);

    MPI_Barrier(MPI_COMM_WORLD);

//====================================================================================//
//---- Triangle writing -----
//====================================================================================//

    if(mpirank==0)
	printf("\n Writing mesh surfaces ");

    MPI_File_set_view(file, offset,  MPI_CHAR, localarray, 
                           "native", MPI_INFO_NULL);

    if(mpirank==0){
      char testchar2[23];
      snprintf (testchar2, sizeof(testchar2), "\nTriangles\n%-10d\n", NTri);
      MPI_File_write(file, testchar2, sizeof(testchar2), MPI_CHAR, &status);
    }

    offset += 22;

    NTri = 4*((pntz-1)*(pntx-1)+(pnty-1)*(pntz-1)+(pntx-1)*(pnty-1))  ;

    locnrows=NTri/mpisize;  
   
//====================================================================================//
//---- local row calculation NTri/mpisize ----
//====================================================================================//

//----------------------------------//
//      balanced  Y part      
//----------------------------------//

   if( ((pnty-1)%mpisize) == 0 ){
     locnrows= 4* ((pnty-1)/mpisize)* ((pntz-1) + (pntx-1));
   }

//----------------------------------//
//     un balanced  Y part     
//----------------------------------//

    if( ((pnty-1)%mpisize) > 0 ){

      if( mpirank < ((pnty-1)%mpisize) )
        locnrows= 4* ((pnty-1)/mpisize+1)* ((pntz-1) + (pntx-1));

      if( mpirank >= ((pnty-1)%mpisize) )
        locnrows= 4* ((pnty-1)/mpisize)* ((pntz-1) + (pntx-1));
    }

//----------------------------------//
//      balanced  Z part      
//----------------------------------//

   if( ((pntz-1)%mpisize) == 0 ){
        locnrows += 4* ((pntz-1)/mpisize)*(pntx-1);
   }

//----------------------------------//
//     un balanced  Z part     
//----------------------------------//

    if( ((pntz-1)%mpisize) > 0 ){

      if( mpirank < ((pntz-1)%mpisize) )
        locnrows += 4* ((pntz-1)/mpisize+1)*(pntx-1);

      if( mpirank >= ((pntz-1)%mpisize) )
        locnrows += 4* ((pntz-1)/mpisize)*(pntx-1);

    }


    char *data_as_txt2 = malloc(locnrows*4*charspernum*sizeof(char)); 

//====================================================================================//
//---- start and endrow local array -----
//====================================================================================//

   istart = mpirank*(pnty-1)/mpisize;
   iend   = istart +(pnty-1)/mpisize;

//----------------------------------//
//      balanced       
//----------------------------------//

   if( ((pnty-1)%mpisize) == 0 ){

    istart=mpirank*(pnty-1)/mpisize; 
    iend=mpirank*(pnty-1)/mpisize + (pnty-1)/mpisize;

   }

//----------------------------------//
//      un balanced       
//----------------------------------//

    if( ((pnty-1)%mpisize) > 0 ){

      if( mpirank < ((pnty-1)%mpisize) ){
         istart=mpirank*((pnty-1)/mpisize + 1) ;
         iend=istart + ((pnty-1)/mpisize + 1) ;
      }

      if( mpirank >= ((pnty-1)%mpisize) ){
         istart=mpirank*((pnty-1)/mpisize) +   ((pnty-1)%mpisize) ;
         iend=istart + ((pnty-1)/mpisize);
      }
    }

//====================================================================================//
//---- writing triangles -----
//====================================================================================//

//----X-MIN-PLANE---//
    dummycount=0;

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

//====================================================================================//
//---- start and endrow local array -----
//====================================================================================//

   istart = mpirank*(pntz-1)/mpisize;
   iend   = istart +(pntz-1)/mpisize;

//----------------------------------//
//      balanced       
//----------------------------------//

   if( ((pntz-1)%mpisize) == 0 ){

    istart=mpirank*(pntz-1)/mpisize; 
    iend=mpirank*(pntz-1)/mpisize + (pntz-1)/mpisize;
   }

//----------------------------------//
//      un balanced       
//----------------------------------//

    if( ((pntz-1)%mpisize) > 0 ){

      if( mpirank < ((pntz-1)%mpisize) ){
         istart=mpirank*((pntz-1)/mpisize + 1) ;
         iend=istart + ((pntz-1)/mpisize + 1) ;
      }

      if( mpirank >= ((pntz-1)%mpisize) ){
         istart=mpirank*((pntz-1)/mpisize) +   ((pntz-1)%mpisize) ;
         iend=istart + ((pntz-1)/mpisize);
      }
    }




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

//====================================================================================//
//---- start and endrow local array -----
//====================================================================================//

    nrows = NTri;

   //----------------------------------//
   //     un balanced       
   //----------------------------------//

    NTri = 4*((pntz-1)*(pntx-1)+(pnty-1)*(pntz-1)+(pntx-1)*(pnty-1))  ;

    if(  ((pnty-1)%mpisize) == 0 ){
      startrow = mpirank * 4 * ((pnty-1)/mpisize) * ((pntx-1)+(pntz-1));
      endrow  = startrow + (4 * ((pnty-1)/mpisize) * ((pntx-1)+(pntz-1))) 	;
    }

    if( ((pnty-1)%mpisize) > 0 ){

    if( mpirank < ((pnty-1)%mpisize) ){
      startrow = mpirank * 4 * ((pnty-1)/mpisize+1) * ((pntx-1)+(pntz-1));
      endrow  = startrow + (4 * ((pnty-1)/mpisize+1) * ((pntx-1)+(pntz-1))) 	;
    }

    if( mpirank >= ((pnty-1)%mpisize) ){
      startrow = mpirank * 4 * ((pnty-1)/mpisize) * ((pntx-1)+(pntz-1))  + ((pnty-1)%mpisize)* (4 * ((pntx-1)+(pntz-1)));
      endrow  = startrow + (4 * ((pnty-1)/mpisize) * ((pntx-1)+(pntz-1))) 	;
    }

   }


    if( ((pntz-1)%mpisize) == 0 ){

      startrow += mpirank * 4 * ((pntz-1)/mpisize) * ((pntx-1));
      endrow   += startrow+(4 * ((pntz-1)/mpisize) * ((pntx-1)));
    }


    if( ((pntz-1)%mpisize) > 0 ){

    if( mpirank < ((pntz-1)%mpisize) ){
      startrow += mpirank * 4 * ((pntz-1)/mpisize+1) * ((pntx-1)) ; 
      endrow  += startrow + (4 * ((pntz-1)/mpisize+1) * ((pntx-1))) 	;
    }

    if( mpirank >= ((pntz-1)%mpisize) ){
      startrow += mpirank * 4 * ((pntz-1)/mpisize) * ((pntx-1))  + ((pntz-1)%mpisize) * ( 4 *  ((pntx-1)) );
      endrow  += startrow + (4 * ((pntz-1)/mpisize) * ((pntx-1))) 	;
    }
   }

   endrow--;



   //----------------------------------//
   //      balanced       
   //----------------------------------//

   if( ((pnty-1)%mpisize) == 0 &&  ((pntz-1)%mpisize) == 0 ){
    startrow = mpirank * locnrows;
    endrow = startrow + locnrows - 1	;
    if (mpirank == mpisize-1) {
        endrow = nrows - 1;
        locnrows = endrow - startrow + 1;
    }
   }


//====================================================================================//
//---- Triangle writing -----
//====================================================================================//


    int globalsizes2[2] = {nrows   , 4};
    int localsizes2 [2] = {locnrows, 4};
    int starts2[2]      = {startrow, 0};
    int order2          = MPI_ORDER_C  ;

    MPI_Datatype localarray2	;
    MPI_Type_create_subarray(2, globalsizes2, localsizes2, starts2, order2, num_as_string, &localarray2);
    MPI_Type_commit(&localarray2);


    MPI_File_set_view(file, offset,  MPI_CHAR, localarray2, 
                           "native", MPI_INFO_NULL);

    MPI_File_write_all(file, data_as_txt2, locnrows*4, num_as_string, &status);

    offset += totcar*NTet;

    free(data_as_txt2);
    MPI_Type_free(&localarray2);

    if(mpirank==0)
	printf(" ---- Done\n ->  %d triangles written ",NTri);

    MPI_Barrier(MPI_COMM_WORLD);


//-----------------------------------------------------------------------------------//
//---- Footer writing -----
//-----------------------------------------------------------------------------------//

    MPI_File_set_view(file, offset,  MPI_CHAR, localarray, 
                           "native", MPI_INFO_NULL);

    if(mpirank==0){
      char testchar3[5];
      snprintf (testchar3, sizeof(testchar3), "\nEnd");
      MPI_File_write(file, testchar3, sizeof(testchar3)-1, MPI_CHAR, &status);
    }

/**/
//-----------------------------------------------------------------------------------//
//---- Free memory -----
//-----------------------------------------------------------------------------------//

    if(mpirank==0){
      printf( "\n\n *============================================================*\n"); 
      printf("  The program finshed in : %1.2f\n",  MPI_Wtime()-t1);
      printf( " *============================================================*\n"); 
    }
    fflush(stdout);

    MPI_File_close(&file);
    MPI_Type_free(&num_as_string);

    MPI_Finalize();
    return 0;
}
