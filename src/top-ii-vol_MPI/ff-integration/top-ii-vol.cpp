#include <iostream>
#include <fstream>
#include <iomanip>

#include "ff++.hpp"
#include "AFunction.hpp"
#include "AFunction_ext.hpp"
using namespace Fem2D;

using namespace std;

template<class K>
class partPointCloud_Op : public E_F0mps {
    public:
        Expression filename			;   
        Expression outname			;                             
        Expression ptx				;
        Expression pty				;  
              
        static const int n_name_param = 0		;
        static basicAC_F0::name_and_type name_param[]	;
        Expression nargs[n_name_param]			;
        
        partPointCloud_Op(const basicAC_F0& args		, 
        		Expression param1		, 
        		Expression param2		, 
        		Expression param3		, 
        		Expression param4				
        		) : 
        		filename     (param1)			,
        		outname      (param2)			,  
        		ptx          (param3)			, 
        		pty          (param4)						
        		{
            		args.SetNameParam(n_name_param	, 
            				  name_param	, 
            				  nargs
            				  )		;
        		}
        		
        AnyType operator()(Stack stack) const		;
};

template<class K>
basicAC_F0::name_and_type partPointCloud_Op<K>::name_param[] = { };


template<class K>
class partPointCloud : public OneOperator {
    public:
        partPointCloud() : OneOperator(atype<long>()	, 
        			     atype<string*>()	,
        			     atype<string*>()	,
        			     atype<long>()	    ,        			     
        			     atype<long>()  
        			     ) {}

        E_F0* code(const basicAC_F0& args) const {
            return new partPointCloud_Op<K>(args, 
            				  t[0]->CastTo(args[0]), 
            				  t[1]->CastTo(args[1]),
            				  t[2]->CastTo(args[2]),  
            				  t[3]->CastTo(args[3])           				              				  
            				  );
        }
};


template<class K>
AnyType partPointCloud_Op<K>::operator()(Stack stack) const {      
    string* inputfile = GetAny<string*>((*filename)(stack))	;
    string* outputfile= GetAny<string*>((*outname)(stack))	;
    int     pntx      = GetAny<long>((*ptx)(stack))	        ;
    int     pnty      = GetAny<long>((*pty)(stack))		    ;
    
//    cout << " Px "<< pntx << " py  "<<pnty  << " Name input "<< *inputfile << " Name output "<< *outputfile << endl;

  //-----------------------------------------//
  //---- Program variables -----
  //-----------------------------------------//
    int fileNo  = 0 ;   
    int counter = 0 ;

  //-----------------------------------------//
  //---- Open input & output files -----
  //-----------------------------------------// 
    ifstream in  ; in.open(*inputfile+".xyz");
    ofstream wr  ; wr.open(*outputfile+"_"+std::to_string(fileNo)+".xyz");
    ofstream wr1 ; wr1.open(*outputfile+"_"+std::to_string(fileNo)+".info"); 

//=============================================================================
// ------------- Commandline logo output ------------------
//=============================================================================

    if(mpirank == 0){
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
         << "  # of strips ---------------------- "<< mpisize << "          \n"
         << "                                                               \n"
         << " *============================================================*\n"
         << "                                                               \n"
         << "  top-ii-vol is now stripping the point cloud .......          \n"
         << "                                                               \n"
         << "  writing "<<string(*outputfile+"_"+std::to_string(fileNo)+".xyz")<<" ... ";


//=============================================================================
// ------------- Main loop to read file and return the strips  ---------
//=========================================================================== 

    double xx[pntx] ;   
    double yy[pntx] ;
    double zz[pntx] ;
 
    int cutoff = pnty/mpisize;


    for(int j=1; j<=pnty; j++){

       counter++;

       for(int i=1; i<=pntx; i++){
           in >> std::fixed >> xx[i-1]         >> yy[i-1]         >> zz[i-1];
           wr << std::fixed << xx[i-1] << "  " << yy[i-1] << "  " << zz[i-1] << endl;
       }

       if(j%cutoff==0 && fileNo < mpisize-1){

          fileNo++; 
          wr.close(); 
          cout << "  done\n";
          cout << "  writing "<<string(*outputfile+"_"+std::to_string(fileNo)+".xyz")<<" ... ";
          wr.open(*outputfile+"_"+std::to_string(fileNo)+".xyz");

          wr1 << counter <<  "  " <<  pntx << endl; 
          counter=0;
 
           wr1.close(); wr1.open(*outputfile+"_"+std::to_string(fileNo)+".info");
          counter++;

          for(int i=1; i<=pntx; i++){
            wr<< std::fixed << xx[i-1] << "  " << yy[i-1] << "  " << zz[i-1] << endl;
          }
        }

    }
          
    wr1 << counter <<  "  " <<  pntx << endl;  wr1.close(); in.close();
    cout << "  done\n";


    cout << "                                                               \n"
         << " *============================================================*\n"
         << "                                                               \n";

    }
    return 0L;
}













template<class K>
class meshPointCloud_Op : public E_F0mps {
    public:
        Expression filename			;   
        Expression outname			;                             
        Expression ptz				;
        Expression dpth				;  
              
        static const int n_name_param = 0		;
        static basicAC_F0::name_and_type name_param[]	;
        Expression nargs[n_name_param]			;
        
        meshPointCloud_Op(const basicAC_F0& args		, 
        		Expression param1		, 
        		Expression param2		, 
        		Expression param3		, 
        		Expression param4				
        		) : 
        		filename     (param1)			,
        		outname      (param2)			,  
        		ptz          (param3)			, 
        		dpth         (param4)						
        		{
            		args.SetNameParam(n_name_param	, 
            				  name_param	, 
            				  nargs
            				  )		;
        		}
        		
        AnyType operator()(Stack stack) const		;
};

template<class K>
basicAC_F0::name_and_type meshPointCloud_Op<K>::name_param[] = { };


template<class K>
class meshPointCloud : public OneOperator {
    public:
        meshPointCloud() : OneOperator(atype<long>()	, 
        			     atype<string*>()	,
        			     atype<string*>()	,
        			     atype<long>()	    ,        			     
        			     atype<double>()  
        			     ) {}

        E_F0* code(const basicAC_F0& args) const {
            return new meshPointCloud_Op<K>(args, 
            				  t[0]->CastTo(args[0]), 
            				  t[1]->CastTo(args[1]),
            				  t[2]->CastTo(args[2]),  
            				  t[3]->CastTo(args[3])           				              				  
            				  );
        }
};

template<class K>
AnyType meshPointCloud_Op<K>::operator()(Stack stack) const {      
    string* inputfile = GetAny<string*>((*filename)(stack))	;
    string* outputfile= GetAny<string*>((*outname)(stack))	;
    int     pntz      = GetAny<long>((*ptz)(stack))	        ;
    double  zmax      = GetAny<double>((*dpth)(stack))	    ;

//-----------------------------------------------------------------------------------//
//---- Global Variables -----
//-----------------------------------------------------------------------------------//

    double xx	  ;
    double yy	  ;
    double zz	  ;
    double delz	  ;
    double zznew  ;

    int IJK	  ;
    int Ip1JK	  ;
    int IJp1K	  ;
    int IJKp1	  ;
    int Ip1JKp1	  ;
    int IJp1Kp1	  ;
    int Ip1Jp1K	  ;
    int Ip1Jp1Kp1 ;

    int pnty;
    int pntx;  


//-----------------------------------------------------------------------------------//
//---- Calculating Parameters -----
//-----------------------------------------------------------------------------------//

    ifstream in;
    in.open(*inputfile+"_"+std::to_string(mpirank)+".xyz");

    ifstream in1;
    in1.open(*inputfile+"_"+std::to_string(mpirank)+".info");

    ofstream wrgmsh;
    wrgmsh.open(*outputfile+"_"+std::to_string(mpirank)+".mesh");

    in1 >> pnty >> pntx;
    in1.close() ;
    

    if(mpirank==(mpisize-1))pnty=pnty-1; // LAST rank does not have comman partion 


    cout << "                                                               \n"
         << "           MPI Processs # "<<mpirank <<"                          \n"
         << "                                                               \n"
         << "  # points in the .xyz ------------- "<< pntx*pnty << "        \n" 
         << "  # points in X -------------------- "<< pntx << "             \n"
         << "  # points in Y -------------------- "<< pnty << "             \n"
         << "                                                               \n"
         << " *============================================================*\n";

//-----------------------------------------------------------------------------------//
//---- Calculating Parameters -----
//-----------------------------------------------------------------------------------//

    int NPnt = pntx * pnty * pntz					;
    int NTri = 4*((pntz-1)*(pntx-1)+(pnty-1)*(pntz-1)+(pntx-1)*(pnty-1));
    int NTet = (pntx-1) * (pnty-1) * (pntz-1) * 6			;


//-----------------------------------------------------------------------------------//
//---- Header for mesh -----
//-----------------------------------------------------------------------------------//

    wrgmsh << "MeshVersionFormatted 1" 		<< endl;
    wrgmsh << "" 				<< endl;
    wrgmsh << "Dimension 3" 			<< endl;
    wrgmsh << "" 				<< endl;

//-----------------------------------------------------------------------------------//
//---- Generating points -----
//-----------------------------------------------------------------------------------//

    cout   << "Generating points ........"		               ;
    wrgmsh << "Vertices" 				<< endl;
    wrgmsh <<  NPnt	 				<< endl;

    for(int i=0; i<pntx*pnty; i++){
        in     >> std::fixed >> xx >>        yy        >> zz                 ;
        wrgmsh << std::fixed << xx << " " << yy << " " << zz << " 0"  << endl;

        zznew=zz;  delz= (zmax-zz)/(pntz-1);
        for(int j=0; j<pntz-1; j++){
            zznew  = zznew + delz;
            wrgmsh << std::fixed << xx << " " << yy << " " << zznew << " 0" << endl;
        }
    }

    wrgmsh << "" 					<< endl;
    cout   << " finished for MPI rank : " << mpirank << endl;

//-----------------------------------------------------------------------------------//
//---- Generating Tetrahedra -----
//-----------------------------------------------------------------------------------//

    cout   << "Generating Tetrahedra ...."			       ;
    wrgmsh << "Tetrahedra" 					<< endl;
    wrgmsh <<  NTet	        				<< endl;

    for(int j=0; j<pnty-1;  j++){
    for(int i=0; i<pntx-1;  i++){
    for(int k=1; k<=pntz-1; k++){

        IJK	    =	i*pntz  + j*pntx*pntz + k	;
        Ip1JK	    =	IJK 	+ (pntx*pntz)		;
        IJp1K	    =	IJK 	+ (pntz)		;
        Ip1Jp1K   =	IJK 	+ (pntx*pntz) + pntz	;
        IJKp1     =	IJK 	+ 1			;
        Ip1JKp1   =	Ip1JK 	+ 1			;
        IJp1Kp1   =	IJp1K   + 1			;
        Ip1Jp1Kp1 =	Ip1Jp1K + 1			;

        wrgmsh << std::fixed << IJK     << " " << IJKp1   << " " << IJp1K     << " " << Ip1Jp1K << " 0" << endl;
        wrgmsh << std::fixed << IJKp1   << " " << IJK     << " " << Ip1JK     << " " << Ip1Jp1K << " 0" << endl;
        wrgmsh << std::fixed << Ip1JKp1 << " " << IJKp1   << " " << Ip1JK     << " " << Ip1Jp1K << " 0" << endl;
        wrgmsh << std::fixed << IJKp1   << " " << Ip1JKp1 << " " << Ip1Jp1Kp1 << " " << Ip1Jp1K << " 0" << endl;
        wrgmsh << std::fixed << IJp1Kp1 << " " << IJKp1   << " " << Ip1Jp1Kp1 << " " << Ip1Jp1K << " 0" << endl;
        wrgmsh << std::fixed << IJKp1   << " " << IJp1Kp1 << " " << IJp1K     << " " << Ip1Jp1K << " 0" << endl;
    }
    }
    }

    wrgmsh << "" 					    << endl;
    cout   << " finished for MPI rank : " << mpirank << endl;

//-----------------------------------------------------------------------------------//
//---- Generating Triangles -----
//-----------------------------------------------------------------------------------//

    cout   << "Generating Triangles ....."			       ;
    wrgmsh << "Triangles" 					<< endl;
    wrgmsh << NTri	        				<< endl;

//----X-MIN-PLANE---//

    for(int i=0; i<pnty-1;  i++){
    for(int j=0; j<pntz-1;  j++){

        IJK	  =	i*(pntz*pntx) + j+1	;
        IJKp1	  =	IJK + 1			;
        Ip1JK	  =	IJK + (pntz*pntx)	;
        Ip1JKp1 =	Ip1JK + 1		;

        wrgmsh << std::fixed << IJKp1   << " " <<  IJK  << " " << Ip1JK << " 1" << endl;
        wrgmsh << std::fixed << Ip1JKp1 << " " << IJKp1 << " " << Ip1JK << " 1" << endl;
    }
    }


//----Y-MIN-PLANE----//
int labymin=99099;

if(mpirank==0)labymin=2;

    for(int i=0; i<pntx-1;  i++){
    for(int j=0; j<pntz-1;  j++){

        IJK	  =	i*pntz + j+1	;
        IJKp1	  =	IJK + 1		;
        IJp1K	  =	IJK + pntz	;
        IJp1Kp1 =	IJp1K + 1	;

        wrgmsh << std::fixed << IJK   << " " << IJKp1   << " " << IJp1K << " " << labymin << endl;
        wrgmsh << std::fixed << IJKp1 << " " << IJp1Kp1 << " " << IJp1K << " " << labymin << endl;

    }
    }


//----Z-MIN-PLANE----//
    for(int i=0; i<pnty-1;  i++){
    for(int j=0; j<pntx-1;  j++){

        IJK	  =	i*(pntz*pntx) + j*(pntz)+1	;
        Ip1JK	  =	IJK + (pntz*pntx)		;
        IJp1K	  =	IJK + pntz			;
        Ip1Jp1K =	Ip1JK + pntz			;

        wrgmsh << std::fixed << IJK   << " " << IJp1K << " " << Ip1Jp1K << " 3" << endl;
        wrgmsh << std::fixed << Ip1JK << " " << IJK   << " " << Ip1Jp1K << " 3" << endl;
    }
    }

//----X-MAX-PLANE----//
    for(int i=0; i<pnty-1;  i++){
    for(int j=0; j<pntz-1;  j++){

        IJK	  =	i*(pntz*pntx) + j+1 + (pntx-1)*(pntz)	;
        IJKp1	  =	IJK + 1					;
        Ip1JK	  =	IJK + (pntz*pntx)			;
        Ip1JKp1 =	Ip1JK + 1				;

        wrgmsh << std::fixed << IJK   << " " << IJKp1   << " " << Ip1JK << " 4" << endl;
        wrgmsh << std::fixed << IJKp1 << " " << Ip1JKp1 << " " << Ip1JK << " 4" << endl;


    }
    }


//----Y-MAX-PLANE----//
int labymax=99099;

if(mpirank==(mpisize-1))labymax=5;

    for(int i=0; i<pntx-1;  i++){
    for(int j=0; j<pntz-1;  j++){

        IJK	  =	i*pntz + j+1 + (pntx*pntz*(pnty-1))	;
        IJKp1	  =	IJK + 1					;
        IJp1K	  =	IJK + pntz				;
        IJp1Kp1 =	IJp1K + 1				;

        wrgmsh << std::fixed << IJKp1   << " " << IJK   << " " << IJp1K << " " << labymax << endl;
        wrgmsh << std::fixed << IJp1Kp1 << " " << IJKp1 << " " << IJp1K << " " << labymax << endl;

    }
    }
//}

//----Z-MAX-PLANE----//
    for(int i=0; i<pnty-1;  i++){
    for(int j=0; j<pntx-1;  j++){

        IJK	  =	i*(pntz*pntx) + j*(pntz)+1 + (pntz-1)	;
        Ip1JK	  =	IJK + (pntz*pntx)			;
        IJp1K	  =	IJK + pntz				;
        Ip1Jp1K =	Ip1JK + pntz				;

        wrgmsh << std::fixed << IJp1K << " " << IJK   << " " << Ip1Jp1K << " 6" << endl;
        wrgmsh << std::fixed << IJK   << " " << Ip1JK << " " << Ip1Jp1K << " 6" << endl;
    }
    }

    cout   << " finished for MPI rank : " << mpirank << endl;

//-----------------------------------------------------------------------------------//
//---- Finishing footer -----
//-----------------------------------------------------------------------------------//
    wrgmsh << "" 					        << endl;
    wrgmsh << "End" 						<< endl;  
};

static void InitFF()
{
  Global.Add("topiivolpart", "(", new partPointCloud<double>);
  Global.Add("topiivolmesh", "(", new meshPointCloud<double>);  
}
LOADFUNC(InitFF) 

