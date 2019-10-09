#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;


int main()
{

double xx;
double yy;
double zmin=240.0000,		zmax=-1920.0000,	zz;

int pntx=50;
int pnty=46;
int pntz=20;

double delz= (zmax-zmin)/(pntz-1);

ifstream in;
in.open("./../data/CoarseMesh.xyz");


double zznew;
int TotNodes = pntx * pnty * pntz;
int TotElemt = (pntx-1) * (pnty-1) * (pntz-1) * 6;
int counter1;

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
	zznew=zz; delz= (zmax-zz)/(pntz-1);
	for(int j=0; j<pntz; j++){
		zznew  = zznew + delz;
		cout << " zz new " << zznew  <<endl;
		if(abs(abs(zznew) - abs(zmax)) <= 100){
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

counter1=1;

int IJK,Ip1JK,IJp1K,Ip1Jp1K,IJKp1,Ip1JKp1,IJp1Kp1,Ip1Jp1Kp1;


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




cout << "Program ended successfully";

return 0;
}
