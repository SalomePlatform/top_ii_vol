#include "ff++.hpp"

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
basicAC_F0::name_and_type meshPointCloud_Op<K>::name_param[] = { 
};


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

