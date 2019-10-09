#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;


int main()
{

double xmin=873101.0000,	xmax=878099.0000,	xx;
double ymin=158000.0000,	ymax=1625999.0000,	yy;
double zmin=240.0000,		zmax=-400,		zz;

int pntx=2251;
int pnty=2500;

int skipx=500;
int skipy=500;


ifstream in;
in.open("DEM_2m_new.xyz");

ofstream wr;
wr.open("CoarseMesh.xyz");

double zznew;


for(int i=0; i<pntx; i++){
for(int j=0; j<pnty; j++){
	in>>std::fixed>> xx  >> yy >> zz;

	if(int(j%skipy)==0 && int(i%skipx)==0 ){
	wr<< std::fixed << xx << "\t" << yy << "\t"<<zz << endl;
	}
}
}

cout << "Program ended successfully";

return 0;
}

