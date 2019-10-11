#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;


int main()
{

//----------------------------------------------------------------------//
//---- Global Variables -----
//----------------------------------------------------------------------//

int counter1	;

double xx	;
double yy	;
double zz	;
double delz	;
double zznew	;


//----------------------------------------------------------------------//
//---- Global Parameters -----
//----------------------------------------------------------------------//
double 	zmin=240.0000		,
       	zmax=-1920.0000		;

int 	pntx=5		        ,
	pnty=4		 	,
	pntz=3			;

//----------------------------------------------------------------------//
//---- Other Parameters -----
//----------------------------------------------------------------------//

int 	IJK		,
	Ip1JK		,
	IJp1K		,
	IJKp1		,
	Ip1JKp1		,
	IJp1Kp1		,
	Ip1Jp1K		,
	Ip1Jp1Kp1	;

//----------------------------------------------------------------------//
//---- Calculating Parameters -----
//----------------------------------------------------------------------//

int TotNodes = pntx * pnty * pntz		;
int TotTrigl = ((pntz-1) * (pntx-1) * 4)  + ((pnty-1) * (pntz-1) * 4) + ((pntx-1) * (pnty-1) * 4);
int TotTetra = (pntx-1) * (pnty-1) * (pntz-1) * 6;
int TotElemt = TotTetra + TotTrigl;



//----------------------------------------------------------------------//
//---- I/O Files -----
//----------------------------------------------------------------------//

ifstream in;
//in.open("CoarseMesh-Skip100.xyz");
in.open("./../data/CoarseMeshoo.xyz");

ofstream wrgmsh;
wrgmsh.open("output-mesh.mesh");


//----------------------------------------------------------------------//
//---- Header for mesh -----
//----------------------------------------------------------------------//

wrgmsh<< "MeshVersionFormatted 1" 		<< endl;
wrgmsh<< "" 					<< endl;
wrgmsh<< "Dimension 3" 				<< endl;
wrgmsh<< "" 					<< endl;


//----------------------------------------------------------------------//
//---- Generating points -----
//----------------------------------------------------------------------//

cout << "Generating points....";

wrgmsh<< "Vertices" 				<< endl;
wrgmsh<< TotNodes 				<< endl;

for(int i=0; i<pntx*pnty; i++){
  in >> std::fixed >> xx  >> yy >> zz;
  wrgmsh << std::fixed << xx << "  " << yy << "  "<<zz << "  0"  << endl;
  zznew=zz; 
  delz= (zmax-zz)/(pntz-1);
  for(int j=0; j<pntz-1; j++){
    zznew  = zznew + delz;
    wrgmsh<< std::fixed << xx << "  " << yy << "  "<<zznew << "  0" << endl;
  }
}

wrgmsh<< "" 						<< endl;
cout  << "Done  "					<< endl;

//----------------------------------------------------------------------//
//---- Generating Tetrahedra -----
//----------------------------------------------------------------------//

cout << "Generating Tetrahedra...."			       ;

wrgmsh<< "Tetrahedra" 					<< endl;
wrgmsh<< TotTetra        				<< endl;


for(int j=0; j<pnty-1;  j++){
for(int i=0; i<pntx-1;  i++){
for(int k=1; k<=pntz-1; k++){

  IJK	    =	i*pntz  + j*pntx*pntz + k	;
  Ip1JK	    =	IJK 	+ (pntx*pntz)		;
  IJp1K	    =	IJK 	+ (pntz)		;
  Ip1Jp1K   =	IJK 	+ (pntx*pntz) + pntz	;
  IJKp1     =	IJK 	+ 1	;
  Ip1JKp1   =	Ip1JK 	+ 1	;
  IJp1Kp1   =	IJp1K   + 1	;
  Ip1Jp1Kp1 =	Ip1Jp1K + 1	;


  wrgmsh<< std::fixed << IJK << " " << IJKp1 << " " << IJp1K << " " << Ip1Jp1K << "  0"<<endl;
  wrgmsh<< std::fixed << IJKp1 << " " << IJK << " " << Ip1JK << " " << Ip1Jp1K <<"  0"<<endl;
  wrgmsh<< std::fixed << Ip1JKp1 << " " << IJKp1 << " " << Ip1JK << " " << Ip1Jp1K <<"  0"<<endl;
  wrgmsh<< std::fixed << IJKp1 << " " << Ip1JKp1 << " " << Ip1Jp1Kp1 << " " << Ip1Jp1K <<"  0"<<endl;
  wrgmsh<< std::fixed << IJp1Kp1 << " " << IJKp1 << " " << Ip1Jp1Kp1 << " " << Ip1Jp1K <<"  0"<<endl;
  wrgmsh<< std::fixed << IJKp1 << " " << IJp1Kp1 << " " << IJp1K << " " << Ip1Jp1K <<"  0"<<endl;
}
}
}

wrgmsh<< "" 						<< endl;
cout << "Done  "					<< endl;

//----------------------------------------------------------------------//
//---- Generating Triangles -----
//----------------------------------------------------------------------//

cout << "Generating Triangles...."			       ;

wrgmsh<< "Triangles" 					<< endl;
wrgmsh<< TotTrigl        				<< endl;

//----X-MIN-PLANE---//

for(int i=0; i<pnty-1;  i++){
for(int j=0; j<pntz-1;  j++){

  IJK	  =	i*(pntz*pntx) + j+1	;
  IJKp1	  =	IJK + 1			;
  Ip1JK	  =	IJK + (pntz*pntx)	;
  Ip1JKp1 =	Ip1JK + 1		;

  wrgmsh<< std::fixed << IJKp1  << " " <<  IJK << " " << Ip1JK   <<  "  1"<<endl;
  wrgmsh<< std::fixed << Ip1JKp1 << " " << IJKp1 << " " << Ip1JK <<  "  1"<<endl;
}
}

//----Y-MIN-PLANE----//
for(int i=0; i<pntx-1;  i++){
for(int j=0; j<pntz-1;  j++){

IJK	=	i*pntz + j+1	;
IJKp1	=	IJK + 1		;
IJp1K	=	IJK + pntz	;
IJp1Kp1	=	IJp1K + 1	;

wrgmsh<< std::fixed << IJK << " " << IJKp1 << " " << IJp1K << "  2"<<endl;
wrgmsh<< std::fixed << IJKp1 << " " << IJp1Kp1 << " " << IJp1K << "  2"<<endl;

}}


//----Z-MIN-PLANE----//
for(int i=0; i<pnty-1;  i++){
for(int j=0; j<pntx-1;  j++){

IJK	=	i*(pntz*pntx) + j*(pntz)+1	;
Ip1JK	=	IJK + (pntz*pntx)		;
IJp1K	=	IJK + pntz			;
Ip1Jp1K	=	Ip1JK + pntz		;

wrgmsh<< std::fixed << IJK  << " " <<  IJp1K  << " " <<   Ip1Jp1K   << "  3"<<endl;
wrgmsh<< std::fixed << Ip1JK << " " << IJK << " " << Ip1Jp1K << "  3"<<endl;
}
}


//----X-MAX-PLANE----//
for(int i=0; i<pnty-1;  i++){
for(int j=0; j<pntz-1;  j++){

IJK	=	i*(pntz*pntx) + j+1 + (pntx-1)*(pntz)	;
IJKp1	=	IJK + 1			;
Ip1JK	=	IJK + (pntz*pntx)	;
Ip1JKp1	=	Ip1JK + 1		;

wrgmsh<< std::fixed <<  IJK  << " " << IJKp1  << " " << Ip1JK << "  4"<<endl;
wrgmsh<< std::fixed <<  IJKp1 << " " << Ip1JKp1 << " " << Ip1JK << "  4"<<endl;


}
}

//----Y-MAX-PLANE----//
for(int i=0; i<pntx-1;  i++){
for(int j=0; j<pntz-1;  j++){

IJK	=	i*pntz + j+1 + (pntx*pntz*(pnty-1))	;
IJKp1	=	IJK + 1		;
IJp1K	=	IJK + pntz	;
IJp1Kp1	=	IJp1K + 1	;

wrgmsh<< std::fixed << IJKp1 << " " << IJK << " " << IJp1K << "  5"<<endl;
wrgmsh<< std::fixed << IJp1Kp1  << " " <<IJKp1  << " " << IJp1K << "  5"<<endl;

}}


//----Z-MAX-PLANE----//
for(int i=0; i<pnty-1;  i++){
for(int j=0; j<pntx-1;  j++){

IJK	=	i*(pntz*pntx) + j*(pntz)+1 + (pntz-1)	;
Ip1JK	=	IJK + (pntz*pntx)		;
IJp1K	=	IJK + pntz			;
Ip1Jp1K	=	Ip1JK + pntz		;

wrgmsh<< std::fixed << IJp1K    << " " << IJK << " " <<   Ip1Jp1K   << "  6"<<endl;
wrgmsh<< std::fixed <<  IJK << " " <<Ip1JK << " " << Ip1Jp1K << "  6"<<endl;
}
}

cout << "Done  "<< endl;

//----------------------------------------------------------------------//
//---- Finishing footer -----
//----------------------------------------------------------------------//

wrgmsh<< "" 						<< endl;
wrgmsh<< "End" 						<< endl;

cout << "Program ended successfully"<< endl;

return 0;
}
