#include <iostream>
#include <fstream>
#include <iomanip>
//#include <chrono>
using namespace std;


int main(){

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
    int pntx = 500 ; //250; //125; //63; //32;
    int pnty = 451 ; //226; //113; //57; //29;

  //-----------------------------------------//
  //---- Number of strips -----
  //-----------------------------------------// 
    int proc = 4   ;

  //-----------------------------------------//
  //---- Input & output file names -----
  //-----------------------------------------// 
    string inputfile = "./../../data/DEM_10m.xyz";  //"./../data/DEM_20m.xyz";//"./../data/DEM_40m.xyz";//"./../data/DEM_80m.xyz";//"./../data/DEM_160m.xyz";
    string outpufile = "point-cloud-strip"    ;

    ifstream in  ; in.open(inputfile);
    ofstream wr  ; wr.open(outpufile+"_"+std::to_string(fileNo)+".xyz");
    ofstream wr1 ; wr1.open(outpufile+"_"+std::to_string(fileNo)+".info"); 

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
         << "  writing "<<string(outpufile+"_"+std::to_string(fileNo)+".xyz")<<" ... ";


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
          cout << "  writing "<<string(outpufile+"_"+std::to_string(fileNo)+".xyz")<<" ... ";
          wr.open(outpufile+"_"+std::to_string(fileNo)+".xyz");

          wr1<< counter << endl; 
          counter=0;
 
           wr1.close(); wr1.open(outpufile+"_"+std::to_string(fileNo)+".info");
          counter++;

          for(int i=1; i<=pntx; i++){
            wr<< std::fixed << xx[i-1] << "  " << yy[i-1] << "  " << zz[i-1] << endl;
          }
        }
    }
          
    wr1 << counter << endl; wr1.close();
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

