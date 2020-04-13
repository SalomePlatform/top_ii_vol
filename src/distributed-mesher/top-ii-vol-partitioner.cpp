#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;


int main(int argc, char *argv[]){

//=============================================================================
// ------------- Logo ------------------
//=============================================================================

#include "./../lib/LogoTopiiVolCpp.hpp"

//=============================================================================
// ------------- Initial Common Variables ------------------
//=============================================================================

  //-----------------------------------------//
  //---- For timing the program -----
  //-----------------------------------------//
    std::clock_t c_start = std::clock();        

  //-----------------------------------------//
  //---- Points in x & y on the surface -----
  //-----------------------------------------// 
                   //10m  20m  40m  80m 160m      
    int pntx = 32; //500; 250; 125; 63; 32;
    int pnty = 29; //451; 226; 113; 57; 29;

  //-----------------------------------------//
  //---- Number of strips -----
  //-----------------------------------------// 

    int mpisize = 2   ;

  //-----------------------------------------//
  //---- Input & output file names -----
  //-----------------------------------------// 

    string * inputfile  = new string();
    string * outputfile = new string();

                                 //_10m";  _20m"; _40m"; _80m"; _160m";
    *inputfile  = "./../../data/DEM_160m" ; 
    *outputfile = "point-cloud-strip"     ;

  //-----------------------------------------//
  //---- Comandline Parameters -----
  //-----------------------------------------//

    for(int i=0; i<argc-1; i++){

        string argvdummy=argv[i];
        string argvdummy1=argv[i+1];

		if( argvdummy == "--xpoints") 
	    	pntx= stoi(argvdummy1);

		if( argvdummy == "--ypoints") 
	    	pnty= stoi(argvdummy1);

		if( argvdummy == "--strips") 
	    	mpisize = stoi(argvdummy1);

		if( argvdummy == "--in") 
	    	*inputfile = argvdummy1;

		if( argvdummy == "--out") 
	    	*outputfile = argvdummy1;

     }

//=============================================================================
// ------------- main algo ------------------
//=============================================================================

#include "./../lib/TopiiVolPartAlgo.hpp"

//=============================================================================
//---- For timing the program -----
//=============================================================================

    std::clock_t c_end = std::clock();

    cout << std::fixed << std::setprecision(4) 
         <<  "  The program finshed in : " 
         << 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC << " ms\n"
         << " *============================================================*\n";

return 0;
}

