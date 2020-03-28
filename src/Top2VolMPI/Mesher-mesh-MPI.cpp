#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <mpi.h>
using namespace std;


int main(int argc, char **argv){

    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double t1 = MPI_Wtime();

//-----------------------------------------------------------------------------------//
//---- Global Variables -----
//-----------------------------------------------------------------------------------//

    double xx	  ;
    double yy	  ;
    double zz	  ;
    double delz	  ;
    double zznew  ;

    int IJK	  ;
    int Ip1JK	  ;
    int IJp1K	  ;
    int IJKp1	  ;
    int Ip1JKp1	  ;
    int IJp1Kp1	  ;
    int Ip1Jp1K	  ;
    int Ip1Jp1Kp1 ;

//-----------------------------------------------------------------------------------//
//---- I/O Files -----
//-----------------------------------------------------------------------------------//

    ifstream in;
    in.open("point-cloud-strip_"+std::to_string(rank)+".xyz");

    ifstream in1;
    in1.open("point-cloud-strip_"+std::to_string(rank)+".info");

    ofstream wrgmsh;
        wrgmsh.open("Top2Vol-mesh_"+std::to_string(rank)+".mesh");

//-----------------------------------------------------------------------------------//
//---- Input Parameters -----
//-----------------------------------------------------------------------------------//

    double zmax = -1920.0  ;
    int	pntz=9;

    int pntx=500;//250;//125;//63;//32;

    int pnty=6;

    in1 >> pnty ;
    in1.close() ;
    

    if(rank==(size-1))pnty=pnty-1; // LAST rank does not have comman partion 

//-----------------------------------------------------------------------------------//
//---- Calculating Parameters -----
//-----------------------------------------------------------------------------------//

    int NPnt = pntx * pnty * pntz					;
    int NTri = 4*((pntz-1)*(pntx-1)+(pnty-1)*(pntz-1)+(pntx-1)*(pnty-1));
    int NTet = (pntx-1) * (pnty-1) * (pntz-1) * 6			;


//=============================================================================
// ------------- Commandline logo output ------------------
//=============================================================================

    if(rank==0)
    cout << " *============================================================*\n" 
         << "         ___                                            ___    \n" 
         << "        /  /               ___________                 /  /    \n" 
         << "     __/  /_ ___    ___   /__  __  __/__    __ ____   /  /     \n" 
         << "    /_   __// _  \\ / _  \\   / / / /   \\ \\  / // _  \\ /  / \n" 
         << "     /  /_ / /_/ // /_/ /__/ /_/ /__   \\ \\/ // /_/ //  /__   \n" 
         << "     \\___/ \\____// ____//__________/    \\__/ \\____/ \\____/\n" 
         << "                / /                                            \n" 
         << "               /_/                                             \n"
         << " *============================================================*\n"; 

    MPI_Barrier(MPI_COMM_WORLD);

    cout << "                                                               \n"
         << "           MPI Processs # "<<rank <<"                          \n"
         << "                                                               \n"
         << "  # points in the .xyz ------------- "<< pntx*pnty << "        \n" 
         << "  # points in X -------------------- "<< pntx << "             \n"
         << "  # points in Y -------------------- "<< pnty << "             \n"
         << "                                                               \n"
         << " *============================================================*\n";

//-----------------------------------------------------------------------------------//
//---- Header for mesh -----
//-----------------------------------------------------------------------------------//

    wrgmsh << "MeshVersionFormatted 1" 		<< endl;
    wrgmsh << "" 				<< endl;
    wrgmsh << "Dimension 3" 			<< endl;
    wrgmsh << "" 				<< endl;

//-----------------------------------------------------------------------------------//
//---- Generating points -----
//-----------------------------------------------------------------------------------//

    cout   << "Generating points ........"		               ;
    wrgmsh << "Vertices" 				<< endl;
    wrgmsh <<  NPnt	 				<< endl;

    for(int i=0; i<pntx*pnty; i++){
        in     >> std::fixed >> xx >>        yy        >> zz                 ;
        wrgmsh << std::fixed << xx << " " << yy << " " << zz << " 0"  << endl;

        zznew=zz;  delz= (zmax-zz)/(pntz-1);
        for(int j=0; j<pntz-1; j++){
            zznew  = zznew + delz;
            wrgmsh << std::fixed << xx << " " << yy << " " << zznew << " 0" << endl;
        }
    }

    wrgmsh << "" 					<< endl;
    cout   << " finished for MPI rank : " << rank << endl;

//-----------------------------------------------------------------------------------//
//---- Generating Tetrahedra -----
//-----------------------------------------------------------------------------------//

    cout   << "Generating Tetrahedra ...."			       ;
    wrgmsh << "Tetrahedra" 					<< endl;
    wrgmsh <<  NTet	        				<< endl;

    for(int j=0; j<pnty-1;  j++){
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

        wrgmsh << std::fixed << IJK     << " " << IJKp1   << " " << IJp1K     << " " << Ip1Jp1K << " 0" << endl;
        wrgmsh << std::fixed << IJKp1   << " " << IJK     << " " << Ip1JK     << " " << Ip1Jp1K << " 0" << endl;
        wrgmsh << std::fixed << Ip1JKp1 << " " << IJKp1   << " " << Ip1JK     << " " << Ip1Jp1K << " 0" << endl;
        wrgmsh << std::fixed << IJKp1   << " " << Ip1JKp1 << " " << Ip1Jp1Kp1 << " " << Ip1Jp1K << " 0" << endl;
        wrgmsh << std::fixed << IJp1Kp1 << " " << IJKp1   << " " << Ip1Jp1Kp1 << " " << Ip1Jp1K << " 0" << endl;
        wrgmsh << std::fixed << IJKp1   << " " << IJp1Kp1 << " " << IJp1K     << " " << Ip1Jp1K << " 0" << endl;
    }
    }
    }

    wrgmsh << "" 					    << endl;
    cout   << " finished for MPI rank : " << rank << endl;

//-----------------------------------------------------------------------------------//
//---- Generating Triangles -----
//-----------------------------------------------------------------------------------//

    cout   << "Generating Triangles ....."			       ;
    wrgmsh << "Triangles" 					<< endl;
    wrgmsh << NTri	        				<< endl;

//----X-MIN-PLANE---//

    for(int i=0; i<pnty-1;  i++){
    for(int j=0; j<pntz-1;  j++){

        IJK	  =	i*(pntz*pntx) + j+1	;
        IJKp1	  =	IJK + 1			;
        Ip1JK	  =	IJK + (pntz*pntx)	;
        Ip1JKp1 =	Ip1JK + 1		;

        wrgmsh << std::fixed << IJKp1   << " " <<  IJK  << " " << Ip1JK << " 1" << endl;
        wrgmsh << std::fixed << Ip1JKp1 << " " << IJKp1 << " " << Ip1JK << " 1" << endl;
    }
    }


//----Y-MIN-PLANE----//
int labymin=99099;

if(rank==0)labymin=2;

    for(int i=0; i<pntx-1;  i++){
    for(int j=0; j<pntz-1;  j++){

        IJK	  =	i*pntz + j+1	;
        IJKp1	  =	IJK + 1		;
        IJp1K	  =	IJK + pntz	;
        IJp1Kp1 =	IJp1K + 1	;

        wrgmsh << std::fixed << IJK   << " " << IJKp1   << " " << IJp1K << " " << labymin << endl;
        wrgmsh << std::fixed << IJKp1 << " " << IJp1Kp1 << " " << IJp1K << " " << labymin << endl;

    }
    }


//----Z-MIN-PLANE----//
    for(int i=0; i<pnty-1;  i++){
    for(int j=0; j<pntx-1;  j++){

        IJK	  =	i*(pntz*pntx) + j*(pntz)+1	;
        Ip1JK	  =	IJK + (pntz*pntx)		;
        IJp1K	  =	IJK + pntz			;
        Ip1Jp1K =	Ip1JK + pntz			;

        wrgmsh << std::fixed << IJK   << " " << IJp1K << " " << Ip1Jp1K << " 3" << endl;
        wrgmsh << std::fixed << Ip1JK << " " << IJK   << " " << Ip1Jp1K << " 3" << endl;
    }
    }

//----X-MAX-PLANE----//
    for(int i=0; i<pnty-1;  i++){
    for(int j=0; j<pntz-1;  j++){

        IJK	  =	i*(pntz*pntx) + j+1 + (pntx-1)*(pntz)	;
        IJKp1	  =	IJK + 1					;
        Ip1JK	  =	IJK + (pntz*pntx)			;
        Ip1JKp1 =	Ip1JK + 1				;

        wrgmsh << std::fixed << IJK   << " " << IJKp1   << " " << Ip1JK << " 4" << endl;
        wrgmsh << std::fixed << IJKp1 << " " << Ip1JKp1 << " " << Ip1JK << " 4" << endl;


    }
    }


//----Y-MAX-PLANE----//
int labymax=99099;

if(rank==(size-1))labymax=5;

    for(int i=0; i<pntx-1;  i++){
    for(int j=0; j<pntz-1;  j++){

        IJK	  =	i*pntz + j+1 + (pntx*pntz*(pnty-1))	;
        IJKp1	  =	IJK + 1					;
        IJp1K	  =	IJK + pntz				;
        IJp1Kp1 =	IJp1K + 1				;

        wrgmsh << std::fixed << IJKp1   << " " << IJK   << " " << IJp1K << " " << labymax << endl;
        wrgmsh << std::fixed << IJp1Kp1 << " " << IJKp1 << " " << IJp1K << " " << labymax << endl;

    }
    }
//}

//----Z-MAX-PLANE----//
    for(int i=0; i<pnty-1;  i++){
    for(int j=0; j<pntx-1;  j++){

        IJK	  =	i*(pntz*pntx) + j*(pntz)+1 + (pntz-1)	;
        Ip1JK	  =	IJK + (pntz*pntx)			;
        IJp1K	  =	IJK + pntz				;
        Ip1Jp1K =	Ip1JK + pntz				;

        wrgmsh << std::fixed << IJp1K << " " << IJK   << " " << Ip1Jp1K << " 6" << endl;
        wrgmsh << std::fixed << IJK   << " " << Ip1JK << " " << Ip1Jp1K << " 6" << endl;
    }
    }

    cout   << " finished for MPI rank : " << rank << endl;

//-----------------------------------------------------------------------------------//
//---- Finishing footer -----
//-----------------------------------------------------------------------------------//

    wrgmsh << "" 					        << endl;
    wrgmsh << "End" 						<< endl;



   MPI_Barrier(MPI_COMM_WORLD);
   if(rank==0){
    cout << "                                                               \n"
         << " *============================================================*\n"
         << "                                                               \n";
    cout << std::fixed << std::setprecision(4) <<  "  Total time taken by MPI mesher : " << MPI_Wtime()-t1 << " s"<< endl;

    cout << "                                                               \n"
         << " *============================================================*\n"
         << "                                                               \n";
    }
    MPI_Finalize();
return 0;
}
