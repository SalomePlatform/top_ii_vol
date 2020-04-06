#include "ff++.hpp"

using namespace std;

template<class K>
class partPointCloud_Op : public E_F0mps {
    public:
        Expression filename			;    
              
        static const int n_name_param = 3		;
        static basicAC_F0::name_and_type name_param[]	;
        Expression nargs[n_name_param]			;
        
        partPointCloud_Op(const basicAC_F0& args		, 
        		Expression param1				
        		) : 
        		filename     (param1)								
        		{
            		args.SetNameParam(n_name_param	, 
            				  name_param	, 
            				  nargs
            				  )		;
        		}
        		
        AnyType operator()(Stack stack) const		;
};

template<class K>
basicAC_F0::name_and_type partPointCloud_Op<K>::name_param[] = { 
    {"outfile", &typeid(std::string*)},
    {"pointsx", &typeid(long)},
    {"pointsy", &typeid(long)}
};


template<class K>
class partPointCloud : public OneOperator {
    public:
        partPointCloud() : OneOperator(atype<long>()	, 
        			     atype<string*>()	
        			     ) {}

        E_F0* code(const basicAC_F0& args) const {
            return new partPointCloud_Op<K>(args, 
            				  t[0]->CastTo(args[0])           				              				  
            				  );
        }
};


template<class K>
AnyType partPointCloud_Op<K>::operator()(Stack stack) const {      
    string* inputfile = GetAny<string*>((*filename)(stack))	;
    string* outputfile= nargs[0] ? GetAny<std::string*>((*nargs[0])(stack)) : NULL;
    int     pntx      = nargs[1] ? GetAny<long>((*nargs[1])(stack)) : -1;
    int     pnty      = nargs[2] ? GetAny<long>((*nargs[2])(stack)) : -1;
    
//    cout << " Px "<< pntx << " py  "<<pnty  << " Name input "<< *inputfile << " Name output "<< *outputfile << endl;


if(mpirank == 0){

#include "./../LogoTopiiVolCpp.hpp"
#include "./../TopiiVolPartAlgo.hpp"

    }
    return 0L;
}


template<class K>
class meshPointCloud_Op : public E_F0mps {
    public:
        Expression filename			;     
              
        static const int n_name_param = 3		;
        static basicAC_F0::name_and_type name_param[]	;
        Expression nargs[n_name_param]			;
        
        meshPointCloud_Op(const basicAC_F0& args		, 
        		Expression param1				
        		) : 
        		filename     (param1)							
        		{
            		args.SetNameParam(n_name_param	, 
            				  name_param	, 
            				  nargs
            				  )		;
        		}
        		
        AnyType operator()(Stack stack) const		;
};

template<class K>
basicAC_F0::name_and_type meshPointCloud_Op<K>::name_param[] = { 
    {"outfile", &typeid(std::string*)},
    {"pointsz", &typeid(long)},
    {"zdepth", &typeid(double)}
};


template<class K>
class meshPointCloud : public OneOperator {
    public:
        meshPointCloud() : OneOperator(atype<long>()	, 
        			     atype<string*>()	
        			     ) {}

        E_F0* code(const basicAC_F0& args) const {
            return new meshPointCloud_Op<K>(args, 
            				  t[0]->CastTo(args[0])          				              				  
            				  );
        }
};

template<class K>
AnyType meshPointCloud_Op<K>::operator()(Stack stack) const {      
    string* inputfile = GetAny<string*>((*filename)(stack))	;
    string* outputfile= nargs[0] ? GetAny<std::string*>((*nargs[0])(stack)) : NULL;
    int     pntz      = nargs[1] ? GetAny<long>((*nargs[1])(stack)) : -1;
    double  zmax      = nargs[2] ? GetAny<double>((*nargs[2])(stack)) : -1.;


//    string* outputfile= GetAny<string*>((*outname)(stack))	;
//    int     pntz      = GetAny<long>((*ptz)(stack))	        ;
//    double  zmax      = GetAny<double>((*dpth)(stack))	    ;

    int pnty;
    int pntx; 

    if(mpirank==0)
     #include "./../LogoTopiiVolCpp.hpp"
     #include "./../TopiiVolMeshAlgo.hpp"
};

static void InitFF()
{
  Global.Add("topiivolpart", "(", new partPointCloud<double>);
  Global.Add("topiivolmesh", "(", new meshPointCloud<double>);  
}
LOADFUNC(InitFF) 
