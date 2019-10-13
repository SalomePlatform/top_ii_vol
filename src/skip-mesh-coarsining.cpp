#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;


int main(){

    int Scatter = 1 ;

    double x ;
    double y ;
    double z ;

    int pntx = 2500 ;
    int pnty = 2251 ;

    int skipx = 270;
    int skipy = 260;

    ifstream in;
        in.open("./../data/DEM_2m_new.xyz");

//-----------------------------------------------------------------------------------//
//---- Skipmeshing -----
//-----------------------------------------------------------------------------------//

    if(Scatter!=1){
    	ofstream wr;
        wr.open("CoarseMesh-Skip500.xyz");
        for(int j=0; j<pnty; j++){
        for(int i=0; i<pntx; i++){
	    in>>std::fixed>> x  >> y >> z;

	    if(int(j%skipy)==0 && int(i%skipx)==0 )
	        wr<< std::fixed << x << "  " << y << "  "<<z << endl;
        }
        }
    }

//-----------------------------------------------------------------------------------//
//---- Skip mesh with partitioning -----
//-----------------------------------------------------------------------------------//

   if(Scatter==1){

        int Nscat;// = 2;
        int countme=0, countme2=0;

        cout << "\n\n";

        cout << " *===================================================* \n" 
             << " *                  INFORMATION                      * \n"  
             << " *===================================================* \n"        
             << "\n   Total # Points :: "<< int((pnty/skipy+1)*(pntx/skipx+1))
             << "\n   Total # xPoints:: "<< int(pntx/skipx+1)
             << "\n   Total # yPoints:: "<< int(pnty/skipy+1)
             << "\n\n ==================================================== \n"  
             << 
         endl;

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
             << " *   Note that when using the parallel GFTM-mesher   * \n" 
             << " *   please use << "<< Nscat <<" >> MPI processing units for     * \n"
             << " *   optimal performance of the mesher in parallel   * \n"
             << " *===================================================* \n"  
             << 
         endl;

	int k=0;
	ofstream wr;

        wr.open("CoarseMesh-Skip500-info.txt");

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


        wr.open("CoarseMesh-Skip500_"+std::to_string(k)+".xyz");

        for(int j=0; j<pnty; j++){
        for(int i=0; i<pntx; i++){

	    in>>std::fixed>> x  >> y >> z;

	    if(int(j%skipy)==0 && int(i%skipx)==0 ){
                if(countme==int(int(pntx/skipx+1)*int(pnty/skipy+1))/Nscat){
		countme=0; k++;
    		wr.close();
		wr.open("CoarseMesh-Skip500_"+std::to_string(k)+".xyz");
		}
	        wr<< std::fixed << x << "  " << y << "  "<<z << endl;
	        countme++;
            }
        }
        }

    }

    cout << "Program ended successfully" << endl;

return 0;
}

