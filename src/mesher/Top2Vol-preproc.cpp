#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;


int main(int argc, char *argv[]){

//-----------------------------------------------------------------------------------//
// ---- Logo -----
//-----------------------------------------------------------------------------------//

#include "./../lib/LogoTopiiVolCpp.hpp"

//-----------------------------------------------------------------------------------//
//---- Global Variables -----
//-----------------------------------------------------------------------------------//

    double x 		;
    double y 		;
    double z 		;

//-----------------------------------------------------------------------------------//
//---- Input Parameters -----
//-----------------------------------------------------------------------------------//

    int Scatter = 0     ;
    int Nscat   = 1     ;

    int pntx = 2500     ;
    int pnty = 2251     ;

    int skipx = 150     ;
    int skipy = 150     ;

    string inpurfile = "./../data/DEM_2m_new.xyz" ;
    string outpufile = "out-coarse.xyz"           ;


//-----------------------------------------------------------------------------------//
//---- Comandline Parameters -----
//-----------------------------------------------------------------------------------//

    for(int i=0; i<argc-1; i++){

        string argvdummy  = argv[i]   ;
        string argvdummy1 = argv[i+1] ;

	if( argvdummy == "--xpoints") 
	    pntx = stoi(argvdummy1);

	if( argvdummy == "--ypoints") 
	    pnty = stoi(argvdummy1);

	if( argvdummy == "--xskip") 
	    skipx = stoi(argvdummy1);

	if( argvdummy == "--yskip") 
	    skipy = stoi(argvdummy1);

	if( argvdummy == "--in") 
	    inpurfile = argvdummy1;

	if( argvdummy == "--out") 
	    outpufile = argvdummy1;

	if( argvdummy == "--scatter"){
		if(argvdummy1 == "yes" ) 
	    		Scatter = 1;
		if(argvdummy1 == "no" ) 
	    		Scatter = 0;
	}

	if( argvdummy == "--strips")
		Nscat = stoi(argvdummy1);		
     
   }


//-----------------------------------------------------------------------------------//
//---- Message on commandline -----
//-----------------------------------------------------------------------------------//

    cout << "\n" 
         << " *  This program  will coarsen your .xyz by skipping * \n" 
         << " *  the  specified  number of x and y points to skip.* \n\n"
         << " *===================================================* \n" 
         << " *                  User Input                       * \n"  
         << " *===================================================* \n\n" 
         << "   X points are        ------ " << pntx      << "\n"
         << "   Y points are        ------ " << pnty      << "\n"
         << "   X skip points are   ------ " << skipx     << "\n"
         << "   Y skip points are   ------ " << skipy     << "\n"
         << "   Input file          ------ " << inpurfile << "\n"
         << "   Output file         ------ " << outpufile << "\n\n";

//-----------------------------------------------------------------------------------//
//---- I/O Files -----
//-----------------------------------------------------------------------------------//
 
    ifstream in;
        in.open(inpurfile);

    ofstream wr;


//-----------------------------------------------------------------------------------//
//---- Total points after skip-----
//-----------------------------------------------------------------------------------//

    int pointsYAfterSkip , pointsXAfterSkip;

    if (pnty%skipy==0)
           pointsYAfterSkip=pnty/skipy;
    if (pnty%skipy!=0)
           pointsYAfterSkip=pnty/skipy+1;

    if (pntx%skipx==0)
           pointsXAfterSkip=pntx/skipx;
    if (pntx%skipx!=0)
           pointsXAfterSkip=pntx/skipx+1;


//-----------------------------------------------------------------------------------//
//---- Skipmeshing with scatter-----
//-----------------------------------------------------------------------------------//

    if(Scatter != 1){

        cout << "\n\n"
    	     << " *===================================================* \n" 
             << " *                  Work in progress                 * \n"  
             << " *===================================================* \n\n"
             << "   Top2Vol began coarsing "+inpurfile+"                \n"
             << "   ...";   


        wr.open(outpufile);

        for(int j = 0; j<pnty; j++){
        for(int i = 0; i<pntx; i++){
	      in>>std::fixed>> x  >> y >> z;

	      if(int(j%skipy) == 0 && int(i%skipx) == 0 )
	          wr<< std::fixed << x << "\t" << y << "\t"<< z << "\n";
        }
        }

        wr.close();

        cout << "Finshed !!!                                               \n" 
             << "   Top2Vol is now writing an info file                    \n"
             << "   ...";    

        wr.open("info-"+outpufile+".txt");

        wr   << " *===================================================* \n" 
             << " *                  INFORMATION                      * \n"  
             << " *===================================================* \n"        
             << "\n   Total # Points :: "<< pointsYAfterSkip*pointsXAfterSkip
             << "\n   Total # xPoints:: "<< pointsXAfterSkip
             << "\n   Total # yPoints:: "<< pointsYAfterSkip
             << "\n\n ==================================================== \n";

        wr.close();

        cout << "Finshed !!!                                               \n";
    }

//-----------------------------------------------------------------------------------//
//---- Skip mesh with partitioning -----
//-----------------------------------------------------------------------------------//

   if(Scatter == 1){
     
        int countme  = 0  ; 

	    int k = 0        ;

        wr.open(outpufile+"-info.txt");

        wr   << " *===================================================* \n" 
             << " *                  INFORMATION                      * \n"  
             << " *===================================================* \n"        
             << "\n   Total # strips :: "<< Nscat  
             << "\n   Total # Points :: "<< pointsYAfterSkip*pointsXAfterSkip
             << "\n   Total # xPoints:: "<< pointsXAfterSkip
             << "\n   Total # yPoints:: "<< pointsYAfterSkip
             << "\n\n ==================================================== \n\n";  
 
        wr.close();

		int   TNPts[Nscat];
        for(int j = 0; j<Nscat; j++)
             TNPts[j]=pointsYAfterSkip*pointsXAfterSkip/Nscat;
   
        for(int j = 0; j<int(pointsYAfterSkip*pointsXAfterSkip%Nscat); j++) 
             TNPts[j]=TNPts[j]+1; 

        wr.open(outpufile+"_"+std::to_string(k)+".xyz");

        for(int j = 0; j<pnty; j++){
        for(int i = 0; i<pntx; i++){

	        in>>std::fixed>> x  >> y >> z;

	        if(int(j%skipy) == 0 && int(i%skipx) == 0 ){
                if(countme==TNPts[k]){
		            countme=0; k++;
    		        wr.close();
                    wr.open(outpufile+"_"+std::to_string(k)+".xyz");
		        }
	            wr << std::fixed << x << "\t" << y << "\t"<< z << "\n";
	            countme++;
            }
        }
        }

        wr.close();

/*
        int x1=pointsYAfterSkip*pointsXAfterSkip, y1=pointsYAfterSkip-1, z1=pntz-1;
        int d1,i1;

        d1=1; i1=1;

        cout << "\n\n"
             << " *===================================================* \n" 
             << " *           !!!! IMPORTANT !!!!                     * \n"  
             << " *===================================================* \n" 
             << "\n   top-ii-vol at this moment does not support       "
             << "\n   unbalanced  partitioning, please  run  the       "
             << "\n   top-ii-vol-ParMesher only  with  the below        "
             << "\n   given number of MPI-Processes:       "; 

      for(i1=1;i1<=10000;i1++){
          if(x1%i1==0&&y1%i1==0&&z1%i1==0){
              d1=i1; cout << d1 <<",    ";
             }
     }
        cout <<".\n";
*/

    }


//-----------------------------------------------------------------------------------//
//---- Message on commandline -----
//-----------------------------------------------------------------------------------//

    	cout << "\n\n"
    	     << " *===================================================* \n" 
             << " *                  Information                      * \n"  
             << " *===================================================* \n"        
             << "\n   Total # Points :: "<< pointsXAfterSkip*pointsYAfterSkip
             << "\n   Total # xPoints:: "<< pointsXAfterSkip
             << "\n   Total # yPoints:: "<< pointsYAfterSkip
             << "\n\n ==================================================== \n\n";


return 0;
}

