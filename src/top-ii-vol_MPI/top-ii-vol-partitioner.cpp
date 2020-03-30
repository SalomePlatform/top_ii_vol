#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;


int main(int argc, char *argv[]){

//=============================================================================
// ------------- Initial Variables ------------------
//=============================================================================

  //-----------------------------------------//
  //---- For timing the program -----
  //-----------------------------------------//
    std::clock_t c_start = std::clock();        

  //-----------------------------------------//
  //---- Program variables -----
  //-----------------------------------------//
    int fileNo  = 0 ;   
    int counter = 0 ;

  //-----------------------------------------//
  //---- Points in x & y on the surface -----
  //-----------------------------------------// 
                   //10m  20m  40m  80m 160m      
    int pntx = 32; //500; 250; 125; 63; 32;
    int pnty = 29; //451; 226; 113; 57; 29;

  //-----------------------------------------//
  //---- Number of strips -----
  //-----------------------------------------// 
    int proc = 2   ;

  //-----------------------------------------//
  //---- Input & output file names -----
  //-----------------------------------------// 
                                      //_10m";  _20m"; _40m"; _80m"; _160m";
    string inputfile = "./../../data/DEM_160m"; 
    string outputfile = "point-cloud-strip"    ;

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
	    	proc= stoi(argvdummy1);

		if( argvdummy == "--in") 
	    	inputfile = argvdummy1;

		if( argvdummy == "--out") 
	    	outputfile = argvdummy1;
     }

   cout << "file name "<< inputfile << endl;
   
  //-----------------------------------------//
  //---- Open input & output files -----
  //-----------------------------------------// 
    ifstream in  ; in.open(inputfile+".xyz");
    ofstream wr  ; wr.open(outputfile+"_"+std::to_string(fileNo)+".xyz");
    ofstream wr1 ; wr1.open(outputfile+"_"+std::to_string(fileNo)+".info"); 

//=============================================================================
// ------------- Commandline logo output ------------------
//=============================================================================

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

    cout << "                                                               \n"
         << "  # points in the .xyz ------------- "<< pntx*pnty << "        \n" 
         << "  # points in X -------------------- "<< pntx << "             \n"
         << "  # points in Y -------------------- "<< pnty << "             \n"
         << "  # of strips ---------------------- "<< proc << "             \n"
         << "                                                               \n"
         << " *============================================================*\n"
         << "                                                               \n"
         << "  top-ii-vol is now stripping the point cloud .......          \n"
         << "                                                               \n"
         << "  writing "<<string(outputfile+"_"+std::to_string(fileNo)+".xyz")<<" ... ";


//=============================================================================
// ------------- Main loop to read file and return the strips  ---------
//=========================================================================== 

    double xx[pntx] ;   
    double yy[pntx] ;
    double zz[pntx] ;
 
    int cutoff = pnty/proc;

    for(int j=1; j<=pnty; j++){

       counter++;
       for(int i=1; i<=pntx; i++){
           in >> std::fixed >> xx[i-1]         >> yy[i-1]         >> zz[i-1];
           wr << std::fixed << xx[i-1] << "  " << yy[i-1] << "  " << zz[i-1] << endl;
       }

       if(j%cutoff==0 && fileNo < proc-1){

          fileNo++; 
          wr.close(); 
          cout << "  done\n";
          cout << "  writing "<<string(outputfile+"_"+std::to_string(fileNo)+".xyz")<<" ... ";
          wr.open(outputfile+"_"+std::to_string(fileNo)+".xyz");

          wr1 << counter <<  "  " <<  pntx << endl; 
          counter=0;
 
           wr1.close(); wr1.open(outputfile+"_"+std::to_string(fileNo)+".info");
          counter++;

          for(int i=1; i<=pntx; i++){
            wr<< std::fixed << xx[i-1] << "  " << yy[i-1] << "  " << zz[i-1] << endl;
          }
        }
    }
          
    wr1 << counter <<  "  " <<  pntx << endl;  wr1.close();
    cout << "  done\n";


    cout << "                                                               \n"
         << " *============================================================*\n"
         << "                                                               \n";

    std::clock_t c_end = std::clock();


    cout << std::fixed << std::setprecision(4) <<  "  Total time taken for point cloud stripping : " << 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC << " ms"<< endl;

    cout << "                                                               \n"
         << " *============================================================*\n"
         << "                                                               \n";
return 0;
}

