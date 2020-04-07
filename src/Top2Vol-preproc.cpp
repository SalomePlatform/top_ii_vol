#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;


int main(int argc, char *argv[]){

//-----------------------------------------------------------------------------------//
// ---- Logo -----
//-----------------------------------------------------------------------------------//

#include "LogoTopiiVolCpp.hpp"

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

    cout << " 							    \n" 
         << " *  This program  will coarsen your .xyz by skipping * \n" 
         << " *  the  specified  number of x and y points to skip.* \n" 
         << " 							    \n";

    cout << " *===================================================* \n" 
         << " *                  User Input                       * \n"  
         << " *===================================================* \n" 
         << "  							    \n"; 


    cout << "   X points are        ------ " << pntx      << endl;
    cout << "   Y points are        ------ " << pnty      << endl;
    cout << "   X skip points are   ------ " << skipx     << endl;
    cout << "   Y skip points are   ------ " << skipy     << endl;
    cout << "   Input file          ------ " << inpurfile << endl;
    cout << "   Output file         ------ " << outpufile << endl;
    cout << " 							    \n";

//-----------------------------------------------------------------------------------//
//---- I/O Files -----
//-----------------------------------------------------------------------------------//
 
    ifstream in;
        in.open(inpurfile);

    ofstream wr;


//-----------------------------------------------------------------------------------//
//---- Totql points after skip-----
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

        cout << "\n\n";
    	cout << " *===================================================* \n" 
             << " *                  Work in progress                 * \n"  
             << " *===================================================* \n"
             << " 							\n"
             << "   Top2Vol began coarsing "+inpurfile+"                \n";
        cout << "   ...";   

        wr.open(outpufile);
        for(int j = 0; j<pnty; j++){
        for(int i = 0; i<pntx; i++){
	    in>>std::fixed>> x  >> y >> z;

	    if(int(j%skipy) == 0 && int(i%skipx) == 0 )
	        wr<< std::fixed << x << "  " << y << "  "<<z << "\n";
        }
        }
        wr.close();

        cout << "Finshed !!!                                               \n" 
             << "   Top2Vol is now writing an info file                    \n";
        cout << "   ...";    

        wr.open("info-"+outpufile+".txt");

        wr   << " *===================================================* \n" 
             << " *                  INFORMATION                      * \n"  
             << " *===================================================* \n"        
             << "\n   Total # Points :: "<< pointsYAfterSkip*pointsXAfterSkip
             << "\n   Total # xPoints:: "<< pointsXAfterSkip
             << "\n   Total # yPoints:: "<< pointsYAfterSkip
             << "\n\n ==================================================== \n"  
             << 
         endl;
        wr.close();


        cout << "Finshed !!!                                               \n";
    }

//-----------------------------------------------------------------------------------//
//---- Skip mesh with partitioning -----
//-----------------------------------------------------------------------------------//

   if(Scatter == 1){

        
        int countme  = 0  ;
        int countme2 = 0  ;

        cout << "\n\n";

        cout << " *===================================================* \n" 
             << " *           !!!! IMPORTANT !!!!                     * \n"  
             << " *===================================================* \n" 
             << "\n   To get perfect scaling we recommend  choosing      "
             << "\n   the striping parameter (number of partitions)      "
             << "\n   so that it divides << "<< pointsYAfterSkip*pointsXAfterSkip
             << " >>   and << "<< pointsYAfterSkip
             << " >>" 
             << "\n\n ==================================================== \n"  
             << 
         endl;

/*
        cout << "\n\n";
        cout << "Enter the striping parameter [int] :: ";cin  >> Nscat ;	
        cout << "\n\n";
*/

        cout << " *===================================================* \n" 
             << " *                  IMPORTANT                        * \n"  
             << " *===================================================* \n"        
             << " *   Note that when using the parallel Top2Vol-mesher* \n" 
             << " *   please use << "<< Nscat <<" >> MPI processing units for     * \n"
             << " *   optimal performance of the mesher in parallel   * \n"
             << " *===================================================* \n"  
             << 
         endl;

	int k = 0 ;

        wr.open(outpufile+"-info.txt");

        wr   << " *===================================================* \n" 
             << " *                  INFORMATION                      * \n"  
             << " *===================================================* \n"        
             << "\n   Total # strips :: "<< Nscat  
             << "\n   Total # Points :: "<< pointsYAfterSkip*pointsXAfterSkip
             << "\n   Total # xPoints:: "<< pointsXAfterSkip
             << "\n   Total # yPoints:: "<< pointsYAfterSkip
             << "\n\n ==================================================== \n"  
             << 
         endl;
        wr.close();


        wr.open(outpufile+"_"+std::to_string(k)+".xyz");

        for(int j = 0; j<pnty; j++){
        for(int i = 0; i<pntx; i++){

	    in>>std::fixed>> x  >> y >> z;

	    if(int(j%skipy) == 0 && int(i%skipx) == 0 ){
                if(countme==int(int(pntx/skipx+1)*int(pnty/skipy+1))/Nscat){
		countme=0; k++;
    		wr.close();
                wr.open(outpufile+"_"+std::to_string(k)+".xyz");
		}
	        wr << std::fixed << x << "  " << y << "  "<< z << "\n";
	        countme++;
            }
        }
        }

        wr.close();

    }


//-----------------------------------------------------------------------------------//
//---- Message on commandline -----
//-----------------------------------------------------------------------------------//

    	cout << "\n\n";
    	cout << " *===================================================* \n" 
             << " *                  Information                      * \n"  
             << " *===================================================* \n"        
             << "\n   Total # Points :: "<< pointsXAfterSkip*pointsYAfterSkip
             << "\n   Total # xPoints:: "<< pointsXAfterSkip
             << "\n   Total # yPoints:: "<< pointsYAfterSkip
             << "\n\n ==================================================== \n"
             << 
    	endl;

return 0;
}

