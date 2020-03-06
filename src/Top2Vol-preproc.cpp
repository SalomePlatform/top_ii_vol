#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;


int main(int argc, char *argv[]){


    int Scatter = 0 ;

    double x ;
    double y ;
    double z ;

	int Nscat;

    int pntx = 2500 ;
    int pnty = 2251 ;

    int skipx = 150;
    int skipy = 150;

    string inpurfile="./../data/DEM_2m_new.xyz";
    string outpufile="out-coarse.xyz";

    for(int i=0; i<argc-1; i++){

        string argvdummy=argv[i];
        string argvdummy1=argv[i+1];
	//if ( !strtoq(argv[i], "-s")) { cout << "   ASDSADASDASXADDWWDD  %d "<< i; }

	if( argvdummy == "--xpoints") 
	    pntx= stoi(argvdummy1);

	if( argvdummy == "--ypoints") 
	    pnty= stoi(argvdummy1);

	if( argvdummy == "--xskip") 
	    skipx= stoi(argvdummy1);

	if( argvdummy == "--yskip") 
	    skipy= stoi(argvdummy1);

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
		Nscat= stoi(argvdummy1);		
     
   }

/*
    cout << "   Total arguments entered are  "<< argc << endl;

    for(int i=0; i<argc; i++){
        string argvdummy=argv[i];
	//if ( !strtoq(argv[i], "-s")) { cout << "   ASDSADASDASXADDWWDD  %d "<< i; }
	if( argvdummy == "--xpoints") 
	    cout << "   ASDSADASDASXADDWWDD  %d "<< i;}
		
	}
*/



    cout << " *===================================================* \n" 
         << " *                  Top2Vol                          * \n"  
         << " *===================================================* \n"
         << " 							    \n" 
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

/*
    cout << "   Enter total  points in  x  [int]   "; cin >> pntx;
    cout << "   Enter points to skip in x  [int]   "; cin >> skipx;
    cout << "   Enter total  points in  y  [int]   "; cin >> pnty;
    cout << "   Enter points to skip in y  [int]   "; cin >> skipy;
    cout << "   Enter the input .xyz file  [char]  "; cin >> inpurfile;
    cout << "   Enter the output file name [char]  "; cin >> outpufile;
    cout << " 							    \n";
*/



 
    ifstream in;
        in.open(inpurfile);

    ofstream wr;

//-----------------------------------------------------------------------------------//
//---- Skipmeshing -----
//-----------------------------------------------------------------------------------//

    if(Scatter!=1){

        cout << "\n\n";
    	cout << " *===================================================* \n" 
             << " *                  Work in progress                 * \n"  
             << " *===================================================* \n"
             << " 							\n"
             << "   Top2Vol began coarsing "+inpurfile+"                \n";
        cout << "   ...";   

        wr.open(outpufile);
        for(int j=0; j<pnty; j++){
        for(int i=0; i<pntx; i++){
	    in>>std::fixed>> x  >> y >> z;

	    if(int(j%skipy)==0 && int(i%skipx)==0 )
	        wr<< std::fixed << x << "  " << y << "  "<<z << endl;
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
             << "\n   Total # Points :: "<< int((pnty/skipy+1)*(pntx/skipx+1))
             << "\n   Total # xPoints:: "<< int(pntx/skipx+1)
             << "\n   Total # yPoints:: "<< int(pnty/skipy+1)
             << "\n\n ==================================================== \n"  
             << 
         endl;
        wr.close();


        cout << "Finshed !!!                                               \n";
    }

//-----------------------------------------------------------------------------------//
//---- Skip mesh with partitioning -----
//-----------------------------------------------------------------------------------//

   if(Scatter==1){

        
        int countme=0, countme2=0;

        cout << "\n\n";

        cout << " *===================================================* \n" 
             << " *           !!!! IMPORTANT !!!!                     * \n"  
             << " *===================================================* \n" 
             << "\n   To get perfect scaling we recommend  choosing      "
             << "\n   the striping parameter (number of partitions)      "
             << "\n   so that it divides << "<< int((pnty/skipy+1)*(pntx/skipx+1))
             << " >>   and << "<< int(pnty/skipy)
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

	int k=0;

        wr.open(outpufile+"-info.txt");

        wr   << " *===================================================* \n" 
             << " *                  INFORMATION                      * \n"  
             << " *===================================================* \n"        
             << "\n   Total # strips :: "<< Nscat  
             << "\n   Total # Points :: "<< int((pnty/skipy+1)*(pntx/skipx+1))
             << "\n   Total # xPoints:: "<< int(pntx/skipx+1)
             << "\n   Total # yPoints:: "<< int(pnty/skipy+1)
             << "\n\n ==================================================== \n"  
             << 
         endl;
        wr.close();


        wr.open(outpufile+"_"+std::to_string(k)+".xyz");

        for(int j=0; j<pnty; j++){
        for(int i=0; i<pntx; i++){

	    in>>std::fixed>> x  >> y >> z;

	    if(int(j%skipy)==0 && int(i%skipx)==0 ){
                if(countme==int(int(pntx/skipx+1)*int(pnty/skipy+1))/Nscat){
		countme=0; k++;
    		wr.close();
                wr.open(outpufile+"_"+std::to_string(k)+".xyz");
		}
	        wr<< std::fixed << x << "  " << y << "  "<<z << endl;
	        countme++;
            }
        }
        }

    }

    	cout << "\n\n";

    	cout << " *===================================================* \n" 
             << " *                  Information                      * \n"  
             << " *===================================================* \n"        
             << "\n   Total # Points :: "<< int((pnty/skipy+1)*(pntx/skipx+1))
             << "\n   Total # xPoints:: "<< int(pntx/skipx+1)
             << "\n   Total # yPoints:: "<< int(pnty/skipy+1)
             << "\n\n ==================================================== \n"
             << 
    	endl;

return 0;
}

