#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;


int main(int argc, char *argv[]){

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
//---- Message on commandline -----
//-----------------------------------------------------------------------------------//

    cout << " *===================================================* \n" 
         << " *                  Top2Vol                          * \n"  
         << " *===================================================* \n"
         << " 							    \n" 
         << " *  This program  will take your .xyz cloud topology * \n" 
         << " *  mesh and and generate a  pseudo-structured tetra * \n"
         << " *  mesh in medit's '.mesh' format                   * \n"  
         << " 							    \n";

//-----------------------------------------------------------------------------------//
//---- Input Parameters -----
//-----------------------------------------------------------------------------------//

    double zmax = -1920.0  	;

    int pntx = 11          	;
    int pnty = 10			;
    int	pntz = 10			;

    string inpurfile = "out-coarse.xyz"	;
    string outpufile = "test.mesh"		;
	string meshtype  = "mesh"		    ;

//-----------------------------------------------------------------------------------//
//---- Comandline Parameters -----
//-----------------------------------------------------------------------------------//

    for(int i=0; i<argc-1; i++){

        string argvdummy=argv[i];
        string argvdummy1=argv[i+1];

		if( argvdummy == "--xpoints") 
	    	pntx= stoi(argvdummy1);

		if( argvdummy == "--ypoints") 
	    	pnty= stoi(argvdummy1);

		if( argvdummy == "--zpoints") 
	    	pntz= stoi(argvdummy1);

		if( argvdummy == "--in") 
	    	inpurfile = argvdummy1;

		if( argvdummy == "--out") 
	    	outpufile = argvdummy1;

		if( argvdummy == "--mesh") 
	    	meshtype = argvdummy1;

		if( argvdummy == "--depth") 
	    	zmax= stod(argvdummy1);
	
     }


//-----------------------------------------------------------------------------------//
//---- I/O Files -----
//-----------------------------------------------------------------------------------//

    ifstream in;
    in.open(inpurfile);

    ofstream wrgmsh;
        wrgmsh.open(outpufile)	;

//-----------------------------------------------------------------------------------//
//---- Message on commandline -----
//-----------------------------------------------------------------------------------//

    cout << " *===================================================* \n" 
         << " *                  User Input                       * \n"  
         << " *===================================================* \n" 
         << "  							    \n"; 


    cout << "   X points are        ------ " << pntx      << endl;
    cout << "   Y points are        ------ " << pnty      << endl;
    cout << "   Z points are        ------ " << pntz      << endl;
    cout << "   Z depth provided    ------ " << zmax      << endl;
    cout << "   Input file          ------ " << inpurfile << endl;
    cout << "   Output file         ------ " << outpufile << endl;
    cout << " 							    \n";


//-----------------------------------------------------------------------------------//
//---- Calculating Parameters -----
//-----------------------------------------------------------------------------------//

    int NPnt = pntx * pnty * pntz					;
    int NTri = 4*((pntz-1)*(pntx-1)+(pnty-1)*(pntz-1)+(pntx-1)*(pnty-1));
    int NTet = (pntx-1) * (pnty-1) * (pntz-1) * 6			;


//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
//---- Writing mesh in Medits .mesh format -----
//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//

   if(meshtype == "mesh"){

    cout << "Meshing the topology in Medit's *.mesh format"	<< endl;

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

    cout   << "Generating points...."		               ;
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
    cout   << "Done  "					<< endl;

//-----------------------------------------------------------------------------------//
//---- Generating Tetrahedra -----
//-----------------------------------------------------------------------------------//

    cout   << "Generating Tetrahedra...."			       ;
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

    wrgmsh << "" 					<< endl;
    cout   << "Done"					<< endl;

//-----------------------------------------------------------------------------------//
//---- Generating Triangles -----
//-----------------------------------------------------------------------------------//

    cout   << "Generating Triangles...."			       ;
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
    for(int i=0; i<pntx-1;  i++){
    for(int j=0; j<pntz-1;  j++){

        IJK	  =	i*pntz + j+1	;
        IJKp1	  =	IJK + 1		;
        IJp1K	  =	IJK + pntz	;
        IJp1Kp1 =	IJp1K + 1	;

        wrgmsh << std::fixed << IJK   << " " << IJKp1   << " " << IJp1K << " 2" << endl;
        wrgmsh << std::fixed << IJKp1 << " " << IJp1Kp1 << " " << IJp1K << " 2" << endl;

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
    for(int i=0; i<pntx-1;  i++){
    for(int j=0; j<pntz-1;  j++){

        IJK	  =	i*pntz + j+1 + (pntx*pntz*(pnty-1))	;
        IJKp1	  =	IJK + 1					;
        IJp1K	  =	IJK + pntz				;
        IJp1Kp1 =	IJp1K + 1				;

        wrgmsh << std::fixed << IJKp1   << " " << IJK   << " " << IJp1K << " 5" << endl;
        wrgmsh << std::fixed << IJp1Kp1 << " " << IJKp1 << " " << IJp1K << " 5" << endl;

    }
    }

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

    cout << "Done  "<< endl;

//-----------------------------------------------------------------------------------//
//---- Finishing footer -----
//-----------------------------------------------------------------------------------//

    wrgmsh << "" 					        << endl;
    wrgmsh << "End" 						<< endl;
  }

//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
//---- Writing mesh in Gmsh's .msh format -----
//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//

  else if (meshtype == "msh"){
    cout << "Meshing the topology in Gmsh's *.msh format"	<< endl;

//--------------------------------------//
//---- Header for msh2 -----
//--------------------------------------//

    wrgmsh<< "$MeshFormat" 		<< endl;
    wrgmsh<< "2.2 0 8" 		    << endl;
    wrgmsh<< "$EndMeshFormat" 	<< endl;
    wrgmsh<< "$Nodes" 		    << endl;
    wrgmsh<< NPnt    		    << endl;

    int counter1=1;

cout << "Generating points....";

for(int i=0; i<pntx*pnty; i++){
	in>>std::fixed>> xx  >> yy >> zz;
	wrgmsh<< std::fixed << counter1 << "  " << xx << "  " << yy << "  "<<zz << endl;
	counter1++;
	zznew=zz; delz= (zmax-zz)/(pntz-1);
	for(int j=0; j<pntz-1; j++){
		zznew  = zznew + delz;
			wrgmsh<< std::fixed << counter1 << "  " << xx << "  " << yy << "  "<<zznew << endl;
			counter1++;

	}
}

wrgmsh<< "$EndNodes" 		<< endl;
cout << "Done  "<< endl;


//-----------------------------------------------------------------------------------//
//---- Generating Tetrahedra -----
//-----------------------------------------------------------------------------------//


cout << "Generating Triangles....";

wrgmsh<< "$Elements" 		<< endl;
wrgmsh<< int(NTet+NTri)     << endl;


counter1=1;

//---------------------------------X-MIN-PLANE----------------------------------//
for(int i=0; i<pnty-1;  i++){
for(int j=0; j<pntz-1;  j++){

IJK	=	i*(pntz*pntx) + j+1	;
IJKp1	=	IJK + 1			;
Ip1JK	=	IJK + (pntz*pntx)	;
Ip1JKp1	=	Ip1JK + 1		;

wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "1" <<" " << "11" << " " << IJKp1  << " " <<  IJK << " " << Ip1JK <<endl;
counter1++;
wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "1" <<" " << "11" <<  " " << Ip1JKp1 << " " << IJKp1 << " " << Ip1JK <<endl;
counter1++;

}
}
//------------------------------------------------------------------------------------//

//---------------------------------Y-MIN-PLANE----------------------------------//
for(int i=0; i<pntx-1;  i++){
for(int j=0; j<pntz-1;  j++){

IJK	=	i*pntz + j+1	;
IJKp1	=	IJK + 1		;
IJp1K	=	IJK + pntz	;
IJp1Kp1	=	IJp1K + 1	;

wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "2" <<" " << "22" << " " << "0" << " "<< " " << IJK << " " << IJKp1 << " " << IJp1K <<endl;
counter1++;
wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "2" <<" " << "22" << " " << "0" << " "<< " " << IJKp1 << " " << IJp1Kp1 << " " << IJp1K <<endl;
counter1++;

}}
//------------------------------------------------------------------------------------//


//---------------------------------Z-MIN-PLANE----------------------------------//
for(int i=0; i<pnty-1;  i++){
for(int j=0; j<pntx-1;  j++){

IJK	=	i*(pntz*pntx) + j*(pntz)+1	;
Ip1JK	=	IJK + (pntz*pntx)		;
IJp1K	=	IJK + pntz			;
Ip1Jp1K	=	Ip1JK + pntz		;

wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "1" <<" " << "33" << " "  << IJK  << " " <<  IJp1K  << " " <<   Ip1Jp1K   <<endl;
counter1++;
wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "1" <<" " << "33" << " "  << Ip1JK << " " << IJK << " " << Ip1Jp1K <<endl;
counter1++;
}
}
//------------------------------------------------------------------------------------//


//---------------------------------X-MAX-PLANE----------------------------------//
for(int i=0; i<pnty-1;  i++){
for(int j=0; j<pntz-1;  j++){

IJK	=	i*(pntz*pntx) + j+1 + (pntx-1)*(pntz)	;
IJKp1	=	IJK + 1			;
Ip1JK	=	IJK + (pntz*pntx)	;
Ip1JKp1	=	Ip1JK + 1		;

wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "1" <<" " << "44" << " " <<  IJK  << " " << IJKp1  << " " << Ip1JK <<endl;
counter1++;
wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "1" <<" " << "44" <<  " " <<  IJKp1 << " " << Ip1JKp1 << " " << Ip1JK <<endl;
counter1++;

}
}
//------------------------------------------------------------------------------------//

//---------------------------------Y-MAX-PLANE----------------------------------//
for(int i=0; i<pntx-1;  i++){
for(int j=0; j<pntz-1;  j++){

IJK	=	i*pntz + j+1 + (pntx*pntz*(pnty-1))	;
IJKp1	=	IJK + 1		;
IJp1K	=	IJK + pntz	;
IJp1Kp1	=	IJp1K + 1	;

wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "2" <<" " << "55" << " " << "0" << " "<< " " <<  IJKp1 << " " << IJK << " " << IJp1K <<endl;
counter1++;
wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "2" <<" " << "55" << " " << "0" << " "<< " " << IJp1Kp1  << " " <<IJKp1  << " " << IJp1K <<endl;
counter1++;

}}
//------------------------------------------------------------------------------------//


//---------------------------------Z-MAX-PLANE----------------------------------//
for(int i=0; i<pnty-1;  i++){
for(int j=0; j<pntx-1;  j++){

IJK	=	i*(pntz*pntx) + j*(pntz)+1 + (pntz-1)	;
Ip1JK	=	IJK + (pntz*pntx)		;
IJp1K	=	IJK + pntz			;
Ip1Jp1K	=	Ip1JK + pntz		;

wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "1" <<" " << "66" << " "  << IJp1K    << " " << IJK << " " <<   Ip1Jp1K   <<endl;
counter1++;
wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "1" <<" " << "66" << " "  <<  IJK << " " <<Ip1JK << " " << Ip1Jp1K <<endl;
counter1++;
}
}
//------------------------------------------------------------------------------------//


cout << "Done  "<< endl;

cout << "Generating Tetrahedrals....";

for(int j=0; j<pnty-1;  j++){
for(int i=0; i<pntx-1;  i++){
for(int k=1; k<=pntz-1; k++){

IJK	=	i*pntz + j*pntx*pntz + k;


Ip1JK	=	IJK 	+ (pntx*pntz)		;
IJp1K	=	IJK 	+ (pntz)		;
Ip1Jp1K	=	IJK 	+ (pntx*pntz) + pntz	;

IJKp1     =	IJK 	+ 1	;
Ip1JKp1   =	Ip1JK 	+ 1	;
IJp1Kp1   =	IJp1K   + 1	;
Ip1Jp1Kp1 =	Ip1Jp1K + 1	;


wrgmsh<< std::fixed << counter1 << " " << "4" << " " << "2" <<" " << "0" << " " << "0" << " " << IJK << " " << IJKp1 << " " << IJp1K << " " << Ip1Jp1K <<endl;
counter1++;

wrgmsh<< std::fixed << counter1 << " " << "4" << " " << "2" <<" " << "0" << " " << "0" << " " << IJKp1 << " " << IJK << " " << Ip1JK << " " << Ip1Jp1K <<endl;
counter1++;


wrgmsh<< std::fixed << counter1 << " " << "4" << " " << "2" <<" " << "0" << " " << "0" << " " << Ip1JKp1 << " " << IJKp1 << " " << Ip1JK << " " << Ip1Jp1K <<endl;
counter1++;

wrgmsh<< std::fixed << counter1 << " " << "4" << " " << "2" <<" " << "0" << " " << "0" << " " << IJKp1 << " " << Ip1JKp1 << " " << Ip1Jp1Kp1 << " " << Ip1Jp1K <<endl;
counter1++;

wrgmsh<< std::fixed << counter1 << " " << "4" << " " << "2" <<" " << "0" << " " << "0" << " " << IJp1Kp1 << " " << IJKp1 << " " << Ip1Jp1Kp1 << " " << Ip1Jp1K <<endl;
counter1++;

wrgmsh<< std::fixed << counter1 << " " << "4" << " " << "2" <<" " << "0" << " " << "0" << " " << IJKp1 << " " << IJp1Kp1 << " " << IJp1K << " " << Ip1Jp1K <<endl;
counter1++;

}
}
}


wrgmsh<< "EndElements" 		<< endl;
cout << "Done  "<< endl;


  }
  else {
    cout << " *=============================================================* \n" 
         << " * ERROR: --mesh input is wrong only msh or mesh is accepted   * \n"  
         << " *=============================================================* \n" 
         << "  							    \n";
  }

    cout << "Program ended successfully" << endl;

return 0;
}
