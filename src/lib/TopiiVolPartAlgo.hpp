//=============================================================================
// ------------- Algo for top-ii-vol part ------------------
//=============================================================================

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
// ------------- Commandline output ------------------
//=============================================================================

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
           wr << std::fixed << xx[i-1] << "\t" << yy[i-1] << "\t" << zz[i-1] << "\n";
       }

       if(j%cutoff==0 && fileNo < mpisize-1){

          fileNo++; 
          wr.close(); 
          cout << "  done\n";
          cout << "  writing "<<string(*outputfile+"_"+std::to_string(fileNo)+".xyz")<<" ... ";
          wr.open(*outputfile+"_"+std::to_string(fileNo)+".xyz");

          wr1 << counter <<  "  " <<  pntx << "\n"; 
          counter=0;
 
           wr1.close(); wr1.open(*outputfile+"_"+std::to_string(fileNo)+".info");
          counter++;

          for(int i=1; i<=pntx; i++){
            wr<< std::fixed << xx[i-1] << "\t" << yy[i-1] << "\t" << zz[i-1] << "\n";
          }
        }

    }
          
    wr1 << counter <<  "  " <<  pntx;  wr.close(); wr1.close(); in.close(); 
    cout << "  done\n";


    cout << "                                                               \n"
         << " *============================================================*\n";
