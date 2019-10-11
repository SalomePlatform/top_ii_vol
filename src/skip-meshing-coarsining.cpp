#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;


int main()
{

double 	xx	,
	yy	,
	zz	;

double xmin=873101.0000,	xmax=878099.0000	;
double ymin=158000.0000,	ymax=1625999.0000	;

int pntx=2251;
int pnty=2500;

int skipx=25;
int skipy=25;


ifstream in;
in.open("./../data/DEM_2m_new.xyz");

ofstream wr;
wr.open("CoarseMesh-Skip100.xyz");

double zznew;


for(int i=0; i<pntx; i++){
for(int j=0; j<pnty; j++){
	in>>std::fixed>> xx  >> yy >> zz;

	if(int(j%skipy)==0 && int(i%skipx)==0 ){
	wr<< std::fixed << xx << "  " << yy << "  "<<zz << endl;
	}
}
}

cout << "Program ended successfully" << endl;

return 0;
}

