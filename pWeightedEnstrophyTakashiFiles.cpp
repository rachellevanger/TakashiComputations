

#include "pWeightedEnstrophyTakashiFiles.h"

int main(int argc, char *argv[])
{
    try
    {

        if ( argc == 1 )
        {
            cerr << "This is a program to perform computations on Takashi style files. To use it type: ./a.out name_of_file p_value\n";
            cerr << "Be also aware that this code works only for Takashi style bitmaps with 4-bit floats\n.";
            cerr << "By Rachel Levanger, from base file cutterTakashiFiles.h provided by Pawel Dlotko\n";
            return 0;
        }
        //those will be command line commands:
        char* filename(argv[1]); //filename
        float p( atof(argv[2]) ); //p-value for weight

        float pEnstrophy = pWeightedEnstrophy(filename, p);

        cout << pEnstrophy ;

        return pEnstrophy;


    }
    catch (std::exception& e)
    {
        cout << "Catch exception : " << e.what() << ". Program terminate \n";
    }

}
