/*****************************************************************************

             This file is a part of top-ii-vol meshing tools.

     -------------------------------------------------------------------

     Author(s): Mohd Afeef Badri
     Email    : mohd-afeef.badri@hotmail.com
     Date     : 2019‑12‑09

     -------------------------------------------------------------------

     top-ii-vol  provides  sequential  and  parallel tools for  creating
     volumic tetrahedral meshes from a topology defined by a point cloud.
     top-ii-vol  is  distributed in the hope that it will be useful, but
     WITHOUT  ANY  WARRANTY; or  without  even  the  implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*******************************************************************************/


#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;


int main(int argc, char *argv[])
{

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

    int pointsYAfterSkip;
    int pointsXAfterSkip;

//-----------------------------------------------------------------------------------//
//---- Input Parameters -----
//-----------------------------------------------------------------------------------//

    int pntx = 2500     ;
    int pnty = 2251     ;

    int skipx = 150     ;
    int skipy = 150     ;

    string inpurfile = "./../data/DEM_2m_new.xyz" ;
    string outpufile = "out-coarse.xyz"           ;

//-----------------------------------------------------------------------------------//
//---- Comandline Parameters -----
//-----------------------------------------------------------------------------------//

    for(int i=0; i<argc-1; i++)
        {

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
         << "   Output file         ------ " << outpufile << "\n";

//-----------------------------------------------------------------------------------//
//---- I/O Files -----
//-----------------------------------------------------------------------------------//

    ifstream in;
    in.open(inpurfile);

    ofstream wr;

//-----------------------------------------------------------------------------------//
//---- For timing the program -----
//-----------------------------------------------------------------------------------//

    std::clock_t c_start = std::clock();

//-----------------------------------------------------------------------------------//
//---- Total points after skip-----
//-----------------------------------------------------------------------------------//

    if (pnty%skipy==0)
        pointsYAfterSkip=pnty/skipy;
    if (pnty%skipy!=0)
        pointsYAfterSkip=pnty/skipy+1;

    if (pntx%skipx==0)
        pointsXAfterSkip=pntx/skipx;
    if (pntx%skipx!=0)
        pointsXAfterSkip=pntx/skipx+1;

//-----------------------------------------------------------------------------------//
//---- Skipmeshing ----
//-----------------------------------------------------------------------------------//

    cout << "\n\n"
         << " *===================================================* \n"
         << " *                  Work in progress                 * \n"
         << " *===================================================* \n\n"
         << "   Top2Vol began coarsing "+inpurfile+"                \n"
         << "   ...";


    wr.open(outpufile);

    for(int j=0; j<pnty; j++)
        {
            for(int i=0; i<pntx; i++)
                {
                    in>>std::fixed>> x  >> y >> z;

                    if(int(j%skipy) == 0 && int(i%skipx) == 0)
                        wr<< std::fixed << x << "\t" << y << "\t"<< z << "\n";
                }
        }

    wr.close();

    cout << "Finshed !!!                                               \n"
         << "   Top2Vol is now writing an info file                    \n"
         << "   ...";

    wr.open("info-"+outpufile+".txt");

    wr   << " *===================================================* \n"
         << " *      Information on processed point cloud         * \n"
         << " *===================================================* \n"
         << "\n   Total # points :: "<< pointsYAfterSkip*pointsXAfterSkip
         << "\n   Total # xPoints:: "<< pointsXAfterSkip
         << "\n   Total # yPoints:: "<< pointsYAfterSkip
         << "\n\n ==================================================== \n";

    wr.close();

    cout << "Finshed !!!                                               \n";

//-----------------------------------------------------------------------------------//
//---- Message on commandline -----
//-----------------------------------------------------------------------------------//

    cout << "\n"
         << " *===================================================* \n"
         << " *      Information on processed point cloud         * \n"
         << " *===================================================* \n"
         << "\n   Total # points :: "<< pointsXAfterSkip*pointsYAfterSkip
         << "\n   Total # xPoints:: "<< pointsXAfterSkip
         << "\n   Total # yPoints:: "<< pointsYAfterSkip
         << "\n\n ==================================================== \n\n";

//-----------------------------------------------------------------------------------//
//---- For timing the program -----
//-----------------------------------------------------------------------------------//

    std::clock_t c_end = std::clock();

    cout << " *============================================================*\n"
         << std::fixed << std::setprecision(4) <<  "  The program finshed in : " << (c_end-c_start)/ CLOCKS_PER_SEC << " s\n"
         << " *============================================================*\n"
         << "                                                               \n";

    return 0;
}

