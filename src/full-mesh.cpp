#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;


int main()
{

double xmin=0.0000,	xmax=3.0000,	xx;
double ymin=0.0000,	ymax=3.0000,	yy;
double zmin=0.0000,	zmax=-3.0000,	zz;

int pntx=4;
int pnty=4;
int pntz=4;

double delz= (zmax-zmin)/(pntz-1);

ifstream in;
in.open("./../data/mesh-in.xyz");


double zznew;
int TotNodes = pntx * pnty * pntz;
int TotTets  = (pntx-1) * (pnty-1) * (pntz-1) * 6;
int TotTri   = (pntx-1) * (pnty-1) * 4 + (pntx-1) * (pntz-1) * 4 + (pnty-1) * (pntz-1) * 4;
int TotElemt = TotTets+TotTri;//(pntx-1) * (pnty-1) * (pntz-1) * 6;
int counter1,counter2,counter3;

ofstream wrgmsh;
wrgmsh.open("output-mesh.msh");

wrgmsh<< "$MeshFormat" 		<< endl;
wrgmsh<< "2.2 0 8" 		<< endl;
wrgmsh<< "$EndMeshFormat" 	<< endl;
wrgmsh<< "$Nodes" 		<< endl;
wrgmsh<< TotNodes 		<< endl;

counter1=1;
for(int i=0; i<pntx*pnty; i++){
	in>>std::fixed>> xx  >> yy >> zz;
	wrgmsh<< std::fixed << counter1 << "\t" << xx << "\t" << yy << "\t"<<zz << endl;
	counter1++;
	zznew=zz;
	for(int j=0; j<pntz; j++){
		zznew  = zznew + delz;
		if(abs(abs(zznew) - abs(zmax)) <= 1e-5){
			wrgmsh<< std::fixed << counter1 << "\t" << xx << "\t" << yy << "\t"<<zmax << endl;
			counter1++;
			break;
		}else
		{
			wrgmsh<< std::fixed << counter1 << "\t" << xx << "\t" << yy << "\t"<<zznew << endl;
			counter1++;
		}

	}
}

wrgmsh<< "$EndNodes" 		<< endl;
wrgmsh<< "$Elements" 		<< endl;
wrgmsh<< TotElemt 		<< endl;



int IJK,Ip1JK,IJp1K,Ip1Jp1K,IJKp1,Ip1JKp1,IJp1Kp1,Ip1Jp1Kp1;

//---------------------------------- Triangles ---------------------------------//

//---------------------------------X-PLANE (YminZmin)----------------------------------//


counter1=1;
for(int i=0; i<pntz-1;  i++){
for(int j=0; j<pnty-1;  j++){

IJK	=	i*pntz + j+1	;
IJKp1	=	IJK + 1		;
IJp1K	=	IJK + pntz	;
IJp1Kp1	=	IJp1K + 1	;

wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "1" <<" " << "11" << " " << " " << IJK << " " << IJKp1 << " " << IJp1K <<endl;
counter1++;
wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "1" <<" " << "11" << " " << " " << IJKp1 << " " << IJp1Kp1 << " " << IJp1K <<endl;
counter1++;

}}
//------------------------------------------------------------------------------------//


//---------------------------------Y-PLANE (XminZmin)----------------------------------//
for(int i=0; i<pntx-1;  i++){
for(int j=0; j<pntz-1;  j++){

IJK	=	i*(pntz*pnty) + j+1	;
IJKp1	=	IJK + 1			;
Ip1JK	=	IJK + (pntz*pnty)	;
Ip1JKp1	=	Ip1JK + 1		;

wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "1" <<" " << "22" << " " << IJKp1  << " " <<  IJK << " " << Ip1JK <<endl;
counter1++;
wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "1" <<" " << "22" <<  " " << Ip1JKp1 << " " << IJKp1 << " " << Ip1JK <<endl;
counter1++;

}
}
//------------------------------------------------------------------------------------//



//---------------------------------Z-PLANE (XminYmin)----------------------------------//
for(int i=0; i<pntx-1;  i++){
for(int j=0; j<pnty-1;  j++){

IJK	=	i*(pntz*pnty) + j*(pnty)+1	;
Ip1JK	=	IJK + (pntz*pnty)		;
IJp1K	=	IJK + pntz			;
Ip1Jp1K	=	IJp1K + (pntz*pnty)		;

wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "1" <<" " << "33" << " "  << IJK  << " " <<  IJp1K  << " " <<   Ip1Jp1K   <<endl;
counter1++;
wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "1" <<" " << "33" << " "  << Ip1JK << " " << IJK << " " << Ip1Jp1K <<endl;
counter1++;

}
}
//------------------------------------------------------------------------------------//


//---------------------------------X-PLANE-MAX (YminZmin)-----------------------------//

for(int i=0; i<pntz-1;  i++){
for(int j=0; j<pnty-1;  j++){

IJK	=	i*pntz + j+1 +(pntz*pnty*(pntx-1))	;
IJKp1	=	IJK + 1		;
IJp1K	=	IJK + pntz	;
IJp1Kp1	=	IJp1K + 1	;

wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "1" <<" " << "44" <<  " " << IJKp1 << " " <<  IJK << " " << IJp1K <<endl;
counter1++;
wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "1" <<" " << "44" << " " << IJp1Kp1 << " " << IJKp1 << " " << IJp1K <<endl;
counter1++;

}
}
//------------------------------------------------------------------------------------//


//---------------------------------Y-PLANE-max (XminZmin)----------------------------------//

for(int i=0; i<pntx-1;  i++){
for(int j=0; j<pntz-1;  j++){

IJK	=	i*(pntz*pnty) + j+1 +(pntz*(pnty-1))	;
IJKp1	=	IJK + 1			;
Ip1JK	=	IJK + (pntz*pnty)	;
Ip1JKp1	=	Ip1JK + 1		;

wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "1" <<" " << "55" <<  " " << IJK << " " << IJKp1 << " " << Ip1JK <<endl;
counter1++;
wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "1" <<" " << "55" <<  " " << IJKp1 << " " << Ip1JKp1 << " " << Ip1JK <<endl;
counter1++;

}
}
//------------------------------------------------------------------------------------//


//---------------------------------Z-PLANE-max (XminYmin)----------------------------------//
for(int i=0; i<pntx-1;  i++){
for(int j=0; j<pnty-1;  j++){

IJK	=	i*(pntz*pnty) + j*(pnty)+1 + (pntz-1);
Ip1JK	=	IJK + (pntz*pnty)		;
IJp1K	=	IJK + pntz			;
Ip1Jp1K	=	IJp1K + (pntz*pnty)		;

wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "1" <<" " << "66" <<  " " << IJK  << " " << Ip1JK   << " " <<   Ip1Jp1K   <<endl;
counter1++;
wrgmsh<< std::fixed << counter1 << " " << "2" << " " << "1" <<" " << "66" <<  " " << IJp1K << " " << IJK << " " << Ip1Jp1K <<endl;
counter1++;

}
}
//------------------------------------------------------------------------------------//


//---------------------------------- Tetrahedra ---------------------------------//
//counter1=1;
for(int i=0; i<pntx-1;  i++){
for(int j=0; j<pnty-1;  j++){
for(int k=1; k<=pntz-1; k++){

IJK	=	i*pntx*pntz + j*pnty + k;


Ip1JK	=	IJK 	+ (pnty*pntz)		;
IJp1K	=	IJK 	+ (pntz)		;
Ip1Jp1K	=	IJK 	+ (pnty*pntz) + pntz	;

IJKp1     =	IJK 	+ 1	;
Ip1JKp1   =	Ip1JK 	+ 1	;
IJp1Kp1   =	IJp1K   + 1	;
Ip1Jp1Kp1 =	Ip1Jp1K + 1	;


wrgmsh<< std::fixed << counter1 << " " << "4" << " " << "1" <<" " << "1" << " "  << IJK << " " << IJKp1 << " " << IJp1K << " " << Ip1Jp1K <<endl;
counter1++;

wrgmsh<< std::fixed << counter1 << " " << "4" << " " << "1" <<" " << "1" << " "  << IJKp1 << " " << IJK << " " << Ip1JK << " " << Ip1Jp1K <<endl;
counter1++;


wrgmsh<< std::fixed << counter1 << " " << "4" << " " << "1" <<" " << "1" <<  " " << Ip1JKp1 << " " << IJKp1 << " " << Ip1JK << " " << Ip1Jp1K <<endl;
counter1++;

wrgmsh<< std::fixed << counter1 << " " << "4" << " " << "1" <<" " << "1" <<  " " << IJKp1 << " " << Ip1JKp1 << " " << Ip1Jp1Kp1 << " " << Ip1Jp1K <<endl;
counter1++;

wrgmsh<< std::fixed << counter1 << " " << "4" << " " << "1" <<" " << "1" <<  " " << IJp1Kp1 << " " << IJKp1 << " " << Ip1Jp1Kp1 << " " << Ip1Jp1K <<endl;
counter1++;

wrgmsh<< std::fixed << counter1 << " " << "4" << " " << "1" <<" " << "1" << " " << IJKp1 << " " << IJp1Kp1 << " " << IJp1K << " " << Ip1Jp1K <<endl;
counter1++;

}
}
}


wrgmsh<< "EndElements" 		<< endl;




cout << "Program ended successfully";

return 0;
}
