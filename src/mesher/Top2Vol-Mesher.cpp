#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;


int main(int argc, char *argv[]){

//-----------------------------------------------------------------------------------//
//---- Logo -----
//-----------------------------------------------------------------------------------//
     #include "./../lib/LogoTopiiVolCpp.hpp" 

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
//---- Message on commandline -----
//-----------------------------------------------------------------------------------//

    cout << " 							    \n" 
         << " *  This program  will take your .xyz cloud topology * \n" 
         << " *  mesh and and generate a  pseudo-structured tetra * \n"
         << " *  mesh in medit's '.mesh' format                   * \n"  
         << " 							    \n";

//-----------------------------------------------------------------------------------//
//---- Input Parameters -----
//-----------------------------------------------------------------------------------//

    double zmax = -1920.0  	;

    int pntx = 11          	;
    int pnty = 10		;
    int	pntz = 10		;

    int binfile = 0     ;

    string * inputfile  = new string();
    string * outputfile = new string();

    * inputfile  = "out-coarse.xyz"	;
    * outputfile = "test.mesh"	    ;


    string meshtype  = "mesh"	    ;

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
	    	*inputfile = argvdummy1;

		if( argvdummy == "--out") 
	    	*outputfile = argvdummy1;

		if( argvdummy == "--mesh") 
	    	meshtype = argvdummy1;

		if( argvdummy == "--depth") 
	    	zmax= stod(argvdummy1);
	
     }


//-----------------------------------------------------------------------------------//
//---- I/O Files -----
//-----------------------------------------------------------------------------------//

    ifstream in;
        in.open(*inputfile)      ;

    ofstream wrgmsh;
        wrgmsh.open(*outputfile)	;

//-----------------------------------------------------------------------------------//
//---- Message on commandline -----
//-----------------------------------------------------------------------------------//

    cout << " *===================================================* \n" 
         << " *                  User Input                       * \n"  
         << " *===================================================* \n\n";


    cout << "   X points are        ------ " << pntx        << "\n"
         << "   Y points are        ------ " << pnty        << "\n"
         << "   Z points are        ------ " << pntz        << "\n"
         << "   Z depth provided    ------ " << zmax        << "\n"
         << "   Input file          ------ " << *inputfile  << "\n"
         << "   Output file         ------ " << *outputfile << "\n\n";

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

    cout << "   Meshing the topology in Medit's *.mesh format"	<< "\n";

//-----------------------------------------------------------------------------------//
//---- Header for mesh -----
//-----------------------------------------------------------------------------------//

    wrgmsh << "MeshVersionFormatted 1\n\n"
           << "Dimension 3\n\n";

//-----------------------------------------------------------------------------------//
//---- Generating points -----
//-----------------------------------------------------------------------------------//

    cout   << "   Generating points....";
    wrgmsh << "Vertices\n"
           <<  NPnt<< "\n";

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
    cout   << "   Done\n";

//-----------------------------------------------------------------------------------//
//---- Generating Tetrahedra -----
//-----------------------------------------------------------------------------------//

    cout   << "   Generating Tetrahedra....";
    wrgmsh << "Tetrahedra\n"
           <<  NTet  << "\n";

    for(int j=0; j<pnty-1;  j++){
    for(int i=0; i<pntx-1;  i++){
    for(int k=1; k<=pntz-1; k++){

        IJK	  =	i*pntz  + j*pntx*pntz + k	;
        Ip1JK	  =	IJK 	+ (pntx*pntz)		;
        IJp1K	  =	IJK 	+ (pntz)		;
        Ip1Jp1K   =	IJK 	+ (pntx*pntz) + pntz	;
        IJKp1     =	IJK 	+ 1			;
        Ip1JKp1   =	Ip1JK 	+ 1			;
        IJp1Kp1   =	IJp1K   + 1			;
        Ip1Jp1Kp1 =	Ip1Jp1K + 1			;

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
    cout   << "   Done\n";

//-----------------------------------------------------------------------------------//
//---- Generating Triangles -----
//-----------------------------------------------------------------------------------//

    cout   << "   Generating Triangles...."			       ;
    wrgmsh << "Triangles\n"
           << NTri  << "\n";

//----X-MIN-PLANE---//

    for(int i=0; i<pnty-1;  i++){
    for(int j=0; j<pntz-1;  j++){

        IJK	  =	i*(pntz*pntx) + j+1	;
        IJKp1	  =	IJK + 1			;
        Ip1JK	  =	IJK + (pntz*pntx)	;
        Ip1JKp1   =	Ip1JK + 1		;

        wrgmsh << std::fixed << IJKp1   << "\t" <<  IJK  << "\t" << Ip1JK << " 1\n"
                             << Ip1JKp1 << "\t" << IJKp1 << "\t" << Ip1JK << " 1\n";
    }
    }

//----Y-MIN-PLANE----//
    for(int i=0; i<pntx-1;  i++){
    for(int j=0; j<pntz-1;  j++){

        IJK	  =	i*pntz + j+1	;
        IJKp1	  =	IJK + 1		;
        IJp1K	  =	IJK + pntz	;
        IJp1Kp1   =	IJp1K + 1	;

        wrgmsh << std::fixed << IJK   << "\t" << IJKp1   << "\t" << IJp1K << " 2\n"
                             << IJKp1 << "\t" << IJp1Kp1 << "\t" << IJp1K << " 2\n";

    }
    }

//----Z-MIN-PLANE----//
    for(int i=0; i<pnty-1;  i++){
    for(int j=0; j<pntx-1;  j++){

        IJK	  =	i*(pntz*pntx) + j*(pntz)+1	;
        Ip1JK	  =	IJK + (pntz*pntx)		;
        IJp1K	  =	IJK + pntz			;
        Ip1Jp1K   =	Ip1JK + pntz			;

        wrgmsh << std::fixed << IJK   << " " << IJp1K << " " << Ip1Jp1K << " 3\n"
                             << Ip1JK << " " << IJK   << " " << Ip1Jp1K << " 3\n";
    }
    }

//----X-MAX-PLANE----//
    for(int i=0; i<pnty-1;  i++){
    for(int j=0; j<pntz-1;  j++){

        IJK	  =	i*(pntz*pntx) + j+1 + (pntx-1)*(pntz)	;
        IJKp1	  =	IJK + 1					;
        Ip1JK	  =	IJK + (pntz*pntx)			;
        Ip1JKp1   =	Ip1JK + 1				;

        wrgmsh << std::fixed << IJK   << " " << IJKp1   << " " << Ip1JK << " 4\n"
                             << IJKp1 << " " << Ip1JKp1 << " " << Ip1JK << " 4\n";


    }
    }

//----Y-MAX-PLANE----//
    for(int i=0; i<pntx-1;  i++){
    for(int j=0; j<pntz-1;  j++){

        IJK	  =	i*pntz + j+1 + (pntx*pntz*(pnty-1))	;
        IJKp1	  =	IJK + 1					;
        IJp1K	  =	IJK + pntz				;
        IJp1Kp1   =	IJp1K + 1				;

        wrgmsh << std::fixed << IJKp1   << " " << IJK   << " " << IJp1K << " 5\n"
                             << IJp1Kp1 << " " << IJKp1 << " " << IJp1K << " 5\n";

    }
    }

//----Z-MAX-PLANE----//
    for(int i=0; i<pnty-1;  i++){
    for(int j=0; j<pntx-1;  j++){

        IJK	  =	i*(pntz*pntx) + j*(pntz)+1 + (pntz-1)	;
        Ip1JK	  =	IJK + (pntz*pntx)			;
        IJp1K	  =	IJK + pntz				;
        Ip1Jp1K   =	Ip1JK + pntz				;

        wrgmsh << std::fixed << IJp1K << " " << IJK   << " " << Ip1Jp1K << " 6\n"
                             << IJK   << " " << Ip1JK << " " << Ip1Jp1K << " 6\n";
    }
    }

    cout << "   Done\n";

//-----------------------------------------------------------------------------------//
//---- Finishing footer -----
//-----------------------------------------------------------------------------------//

    wrgmsh << "\n"
           << "End"<< endl;
  }

//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
//---- Writing mesh in Gmsh's .msh format -----
//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//

  else if (meshtype == "msh"){
    cout << "   Meshing the topology in Gmsh's *.msh format\n";

//--------------------------------------//
//---- Header for msh2 -----
//--------------------------------------//

    wrgmsh<< "$MeshFormat\n"
          << "2.2 0 8\n"
          << "$EndMeshFormat\n"
          << "$Nodes\n"
          << NPnt<<"\n" ;

    int counter1 = 1;

    cout << "   Generating points....";

    for(int i=0; i<pntx*pnty; i++){
        in >> std::fixed >> xx  >> yy >> zz;
	wrgmsh << std::fixed << counter1 << "\t" << xx << "\t" << yy << "\t"<< zz << "\n" ;
	counter1++ ;
	zznew = zz ; delz= (zmax-zz)/(pntz-1);
	for(int j=0; j<pntz-1; j++){
	    zznew  = zznew + delz ;
	    wrgmsh << std::fixed << counter1 << "\t" << xx << "\t" << yy << "\t"<< zznew << "\n" ;
	    counter1++;

	}
    }

    wrgmsh<< "$EndNodes\n";
    cout << "   Done\n";


//-----------------------------------------------------------------------------------//
//---- Generating Tetrahedra -----
//-----------------------------------------------------------------------------------//

    cout << "   Generating Triangles...."  ;

    wrgmsh << "$Elements\n"
           << int(NTet+NTri)<< "\n";

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

    }
    }
//------------------------------------------------------------------------------------//


//---------------------------------Z-MIN-PLANE----------------------------------//

    for(int i=0; i<pnty-1;  i++){
    for(int j=0; j<pntx-1;  j++){

        IJK	=	i*(pntz*pntx) + j*(pntz)+1	;
        Ip1JK	=	IJK + (pntz*pntx)		;
        IJp1K	=	IJK + pntz			;
        Ip1Jp1K	=	Ip1JK + pntz		        ;

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
        IJKp1	=	IJK + 1			                ;
        Ip1JK	=	IJK + (pntz*pntx)	                ;
        Ip1JKp1	=	Ip1JK + 1		                ;

        wrgmsh<< std::fixed << counter1   << " 2 1 44 " <<  IJK  << " " << IJKp1    << " " << Ip1JK <<"\n"
                            << counter1+1 << " 2 1 44 " <<  IJKp1 << " " << Ip1JKp1 << " " << Ip1JK <<"\n";
        counter1=counter1+2;

    }
    }
//------------------------------------------------------------------------------------//

//---------------------------------Y-MAX-PLANE----------------------------------//

    for(int i=0; i<pntx-1;  i++){
    for(int j=0; j<pntz-1;  j++){

        IJK	=	i*pntz + j+1 + (pntx*pntz*(pnty-1))	;
        IJKp1	=	IJK + 1		                        ;
        IJp1K	=	IJK + pntz	                        ;
        IJp1Kp1	=	IJp1K + 1	                        ;

        wrgmsh<< std::fixed << counter1   << " 2 2 55 0  " <<  IJKp1   << " " << IJK << " "   << IJp1K <<"\n"
                            << counter1+1 << " 2 2 55 0  " << IJp1Kp1  << " " <<IJKp1  << " " << IJp1K <<"\n";
        counter1=counter1+2;

    }
    }
//------------------------------------------------------------------------------------//


//---------------------------------Z-MAX-PLANE----------------------------------//

    for(int i=0; i<pnty-1;  i++){
    for(int j=0; j<pntx-1;  j++){

        IJK	=	i*(pntz*pntx) + j*(pntz)+1 + (pntz-1)	;
        Ip1JK	=	IJK + (pntz*pntx)		        ;
        IJp1K	=	IJK + pntz			        ;
        Ip1Jp1K	=	Ip1JK + pntz		                ;

        wrgmsh<< std::fixed << counter1    << " 2 1 66 "  << IJp1K << " " << IJK   << " " << Ip1Jp1K   <<"\n"
                            << counter1+1  << " 2 1 66 "  << IJK   << " " << Ip1JK << " " << Ip1Jp1K   <<"\n";
        counter1=counter1+2;
    }
    }
//------------------------------------------------------------------------------------//


    cout << "Done  "<< endl;

    cout << "Generating Tetrahedrals....";

    for(int j=0; j<pnty-1;  j++){
    for(int i=0; i<pntx-1;  i++){
    for(int k=1; k<=pntz-1; k++){

        IJK	=	i*pntz + j*pntx*pntz + k        ;


        Ip1JK	=	IJK 	+ (pntx*pntz)		;
        IJp1K	=	IJK 	+ (pntz)		;
        Ip1Jp1K	=	IJK 	+ (pntx*pntz) + pntz	;

        IJKp1     =	IJK 	+ 1	                ;
        Ip1JKp1   =	Ip1JK 	+ 1	                ;
        IJp1Kp1   =	IJp1K   + 1	                ;
        Ip1Jp1Kp1 =	Ip1Jp1K + 1	                ;


        wrgmsh<< std::fixed << counter1   << " 4 2 0 0 " << IJK     << " " << IJKp1   << " " << IJp1K << " " << Ip1Jp1K <<"\n"
                            << counter1+1 << " 4 2 0 0 " << IJKp1   << " " << IJK     << " " << Ip1JK << " " << Ip1Jp1K <<"\n"
                            << counter1+2 << " 4 2 0 0 " << Ip1JKp1 << " " << IJKp1   << " " << Ip1JK << " " << Ip1Jp1K <<"\n"
                            << counter1+3 << " 4 2 0 0 " << IJKp1   << " " << Ip1JKp1 << " " << Ip1Jp1Kp1 << " " << Ip1Jp1K <<"\n"
                            << counter1+4 << " 4 2 0 0 " << IJp1Kp1 << " " << IJKp1    << " " << Ip1Jp1Kp1 << " " << Ip1Jp1K <<"\n"
                            << counter1+5 << " 4 2 0 0 " << IJKp1   << " " << IJp1Kp1  << " " << IJp1K << " " << Ip1Jp1K <<"\n";
        counter1=counter1+6;

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

return 0;
}
