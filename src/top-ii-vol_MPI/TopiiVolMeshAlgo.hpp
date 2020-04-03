
//-----------------------------------------------------------------------------------//
//---- Global Variables -----
//-----------------------------------------------------------------------------------//

    double xx	  ;
    double yy	  ;
    double zz	  ;
    double delz	  ;
    double zznew  ;

    int IJK	      ;
    int Ip1JK	  ;
    int IJp1K	  ;
    int IJKp1	  ;
    int Ip1JKp1	  ;
    int IJp1Kp1	  ;
    int Ip1Jp1K	  ;
    int Ip1Jp1Kp1 ;


//-----------------------------------------------------------------------------------//
//---- Calculating Parameters -----
//-----------------------------------------------------------------------------------//

    ifstream in;
    in.open(*inputfile+"_"+std::to_string(mpirank)+".xyz",ios::binary);

    ifstream in1;
    in1.open(*inputfile+"_"+std::to_string(mpirank)+".info",ios::binary);

    ofstream wrgmsh;
    wrgmsh.open(*outputfile+"_"+std::to_string(mpirank)+".meshb",ios::binary);

    in1 >> pnty >> pntx;
    in1.close() ;
    

    if(mpirank==(mpisize-1))pnty=pnty-1; // Last mpirank does not have comman partion 

//-----------------------------------------------------------------------------------//
//---- Calculating Parameters -----
//-----------------------------------------------------------------------------------//

    int NPnt = pntx * pnty * pntz					;
    int NTri = 4*((pntz-1)*(pntx-1)+(pnty-1)*(pntz-1)+(pntx-1)*(pnty-1));
    int NTet = (pntx-1) * (pnty-1) * (pntz-1) * 6			;


/*
//=============================================================================
// ------------- Commandline  output ------------------
//=============================================================================



    MPI_Barrier(MPI_COMM_WORLD);

    cout << "                                                               \n"
         << "           MPI Processs # "<<mpirank <<"                       \n"
         << "                                                               \n"
         << "  # points in the .xyz ------------- "<< pntx*pnty << "        \n" 
         << "  # points in X -------------------- "<< pntx << "             \n"
         << "  # points in Y -------------------- "<< pnty << "             \n"
         << "                                                               \n"
         << " *============================================================*\n";
*/

//-----------------------------------------------------------------------------------//
//---- Header for mesh -----
//-----------------------------------------------------------------------------------//

    wrgmsh << "MeshVersionFormatted 1\n\n"
           << "Dimension 3\n\n";

//-----------------------------------------------------------------------------------//
//---- Generating points -----
//-----------------------------------------------------------------------------------//

    cout   << "Generating points ........"		               ;
    wrgmsh << "Vertices\n"
           <<  NPnt	<<"\n";

    for(int i=0; i<pntx*pnty; i++){
        in     >> std::fixed >> xx >>        yy        >> zz                 ;
        wrgmsh << std::fixed << xx << "\t" << yy << "\t" << zz << " 0\n";

        zznew=zz;  delz= (zmax-zz)/(pntz-1);
        for(int j=0; j<pntz-1; j++){
            zznew  = zznew + delz;
            wrgmsh << std::fixed << xx << "\t" << yy << "\t" << zznew << " 0\n";
        }
    }

    wrgmsh << "\n";
    cout   << " finished for MPI rank : " << mpirank << "\n";

//-----------------------------------------------------------------------------------//
//---- Generating Tetrahedra -----
//-----------------------------------------------------------------------------------//

    cout   << "Generating Tetrahedra ...."			       ;
    wrgmsh << "Tetrahedra\n"
           <<  NTet <<"\n";

    for(int j=0; j<pnty-1;  j++){
    for(int i=0; i<pntx-1;  i++){
    for(int k=1; k<=pntz-1; k++){

        IJK	        =	i*pntz  + j*pntx*pntz + k	    ;
        Ip1JK	    =	IJK 	+ (pntx*pntz)		    ;
        IJp1K	    =	IJK 	+ (pntz)		        ;
        Ip1Jp1K     =	IJK 	+ (pntx*pntz) + pntz	;
        IJKp1       =	IJK 	+ 1			;
        Ip1JKp1     =	Ip1JK 	+ 1			;
        IJp1Kp1     =	IJp1K   + 1			;
        Ip1Jp1Kp1   =	Ip1Jp1K + 1			;

        wrgmsh << std::fixed << IJK     << "\t" << IJKp1   << "\t" << IJp1K     << "\t" << Ip1Jp1K << " 0\n"
                             << IJKp1   << "\t" << IJK     << "\t" << Ip1JK     << "\t" << Ip1Jp1K << " 0\n"
                             << Ip1JKp1 << "\t" << IJKp1   << "\t" << Ip1JK     << "\t" << Ip1Jp1K << " 0\n"
                             << IJKp1   << "\t" << Ip1JKp1 << "\t" << Ip1Jp1Kp1 << "\t" << Ip1Jp1K << " 0\n"
                             << IJp1Kp1 << "\t" << IJKp1   << "\t" << Ip1Jp1Kp1 << "\t" << Ip1Jp1K << " 0\n"
                             << IJKp1   << "\t" << IJp1Kp1 << "\t" << IJp1K     << "\t" << Ip1Jp1K << " 0\n";
    }
    }
    }

    wrgmsh << "\n";
    cout   << " finished for MPI rank : " << mpirank << "\n";

//-----------------------------------------------------------------------------------//
//---- Generating Triangles -----
//-----------------------------------------------------------------------------------//

    cout   << "Generating Triangles ....."			       ;
    wrgmsh << "Triangles\n"
           << NTri << "\n";

//----X-MIN-PLANE---//

    for(int i=0; i<pnty-1;  i++){
    for(int j=0; j<pntz-1;  j++){

        IJK	  =	i*(pntz*pntx) + j+1	;
        IJKp1	  =	IJK + 1			;
        Ip1JK	  =	IJK + (pntz*pntx)	;
        Ip1JKp1 =	Ip1JK + 1		;

        wrgmsh << std::fixed << IJKp1   << "\t" <<  IJK  << "\t" << Ip1JK << " 1\n"
                             << Ip1JKp1 << "\t" << IJKp1 << "\t" << Ip1JK << " 1\n";
    }
    }


//----Y-MIN-PLANE----//
int labymin=99099;

if(mpirank==0)labymin=2;

    for(int i=0; i<pntx-1;  i++){
    for(int j=0; j<pntz-1;  j++){

        IJK	  =	i*pntz + j+1	;
        IJKp1	  =	IJK + 1		;
        IJp1K	  =	IJK + pntz	;
        IJp1Kp1 =	IJp1K + 1	;

        wrgmsh << std::fixed << IJK   << "\t" << IJKp1   << "\t" << IJp1K << "\t" << labymin << "\n"
                             << IJKp1 << "\t" << IJp1Kp1 << "\t" << IJp1K << "\t" << labymin << "\n";

    }
    }


//----Z-MIN-PLANE----//
    for(int i=0; i<pnty-1;  i++){
    for(int j=0; j<pntx-1;  j++){

        IJK	  =	i*(pntz*pntx) + j*(pntz)+1	;
        Ip1JK	  =	IJK + (pntz*pntx)		;
        IJp1K	  =	IJK + pntz			;
        Ip1Jp1K =	Ip1JK + pntz			;

        wrgmsh << std::fixed << IJK   << "\t" << IJp1K << "\t" << Ip1Jp1K << " 3\n"
                             << Ip1JK << "\t" << IJK   << "\t" << Ip1Jp1K << " 3\n";
    }
    }

//----X-MAX-PLANE----//
    for(int i=0; i<pnty-1;  i++){
    for(int j=0; j<pntz-1;  j++){

        IJK	  =	i*(pntz*pntx) + j+1 + (pntx-1)*(pntz)	;
        IJKp1	  =	IJK + 1					;
        Ip1JK	  =	IJK + (pntz*pntx)			;
        Ip1JKp1 =	Ip1JK + 1				;

        wrgmsh << std::fixed << IJK   << "\t" << IJKp1   << "\t" << Ip1JK << " 4\n"
                             << IJKp1 << "\t" << Ip1JKp1 << "\t" << Ip1JK << " 4\n";


    }
    }


//----Y-MAX-PLANE----//
int labymax=99099;

if(mpirank==(mpisize-1))labymax=5;

    for(int i=0; i<pntx-1;  i++){
    for(int j=0; j<pntz-1;  j++){

        IJK	  =	i*pntz + j+1 + (pntx*pntz*(pnty-1))	;
        IJKp1	  =	IJK + 1					;
        IJp1K	  =	IJK + pntz				;
        IJp1Kp1 =	IJp1K + 1				;

        wrgmsh << std::fixed << IJKp1   << "\t" << IJK   << "\t" << IJp1K << "\t" << labymax << "\n"
                             << IJp1Kp1 << "\t" << IJKp1 << "\t" << IJp1K << "\t" << labymax << "\n";

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

        wrgmsh << std::fixed << IJp1K << "\t" << IJK   << "\t" << Ip1Jp1K << " 6\n"
                             << IJK   << "\t" << Ip1JK << "\t" << Ip1Jp1K << " 6\n";
    }
    }

    cout   << " finished for MPI rank : " << mpirank << "\n";

//-----------------------------------------------------------------------------------//
//---- Finishing footer -----
//-----------------------------------------------------------------------------------//

    wrgmsh << "\n"
           << "End\n";
