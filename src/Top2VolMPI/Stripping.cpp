#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
using namespace std;


int main(){


auto start = std::chrono::system_clock::now();


    double x ;
    double y ;
    double z ;

    int pntx = 500;//250;//125;//63;//32  ;
    int pnty = 451;//226;//113;//57;//29  ;

    int skipx = 32 ;
    int skipy = 29 ;
    int proc  =  4 ;

    string inputfile = "./../data/DEM_10m.xyz";//"./../data/DEM_20m.xyz";//"./../data/DEM_40m.xyz";//"./../data/DEM_80m.xyz";//"./../data/DEM_160m.xyz";
    string outpufile = "point-cloud-strip"      ;

    cout << " *===================================================* \n" 
         << " *                  Top2Vol                          * \n"  
         << " *===================================================* \n"
         << " 							  						    \n" 
         << " *  This program  will coarsen your .xyz by skipping * \n" 
         << " *  the  specified  number of x and y points to skip.* \n" 
         << " 							   						    \n";

    cout << " *===================================================* \n" 
         << " *                  User Input                       * \n"  
         << " *===================================================* \n" 
         << "  							                            \n"; 


    ifstream in;
    in.open(inputfile);

    ofstream wr  ;
    ofstream wr1 ;


    int fileNo=0; int counter=0;


    wr.open(outpufile+"_"+std::to_string(fileNo)+".xyz");
    wr1.open(outpufile+"_"+std::to_string(fileNo)+".info");  
     
    double xx[pntx],   yy[pntx], zz[pntx]; 
    int cutoff=pnty/proc;

        for(int j=1; j<=pnty; j++){
           counter++;
           for(int i=1; i<=pntx; i++){
 	           in>>std::fixed>> xx[i-1]  >> yy[i-1] >> zz[i-1];
	           wr<< std::fixed << xx[i-1] << "  " << yy[i-1] << "  "<<zz[i-1] << endl;
           }

         //  for(int i=1; i<=pntx; i++){      
         //     cout <<" FILE " << fileNo << " For j "<< j <<" Write    " << i << endl;
         //  }

           if(j%cutoff==0 && fileNo < proc-1){fileNo++; 
    	      wr.close();
              wr.open(outpufile+"_"+std::to_string(fileNo)+".xyz");

	          wr1<< counter << endl; counter=0;
    	      wr1.close();
              wr1.open(outpufile+"_"+std::to_string(fileNo)+".info");

              counter++;
              for(int i=1; i<=pntx; i++){
	            wr<< std::fixed << xx[i-1] << "  " << yy[i-1] << "  "<<zz[i-1] << endl;
           }
           }
        }

	          wr1<< counter << endl;
    	      wr1.close();
/*
    cout << "   Enter total  points in  x  [int]   "; cin >> pntx;
    cout << "   Enter points to skip in x  [int]   "; cin >> skipx;
    cout << "   Enter total  points in  y  [int]   "; cin >> pnty;
    cout << "   Enter points to skip in y  [int]   "; cin >> skipy;
    cout << "   Enter the input .xyz file  [char]  "; cin >> inputfile;
    cout << "   Enter the output file name [char]  "; cin >> outpufile;
    cout << " 							    \n";
*/
 
//    ifstream in;
//        in.open(inputfile);

//    ofstream wr;

/*
//-----------------------------------------------------------------------------------//
//---- Stripping -----
//-----------------------------------------------------------------------------------//

        int Nscat;
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


        cout << "\n\n";
        cout << "Enter the striping parameter [int] :: ";cin  >> Nscat ;	
        cout << "\n\n";
 
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
*/

auto end = std::chrono::system_clock::now();

int elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>
                         (end - start).count();
std::cout << "Elapsed time Top2VolScatter : " << elapsed_seconds << " seconds" << std::endl;
return 0;
}

