#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <cmath>
#include <climits>

#include "readWriteTakashiFiles.h"

#pragma once

using namespace std;


// compute p-weighted Enstrophy of file.
// E_p(w) = (1/2)||w||_p^p
// Usual enstrophy value is given by E_2(w)

float pWeightedEnstrophy( char* filename, float p )
{

    //convert Takashi file into a bitmap:
    std::pair< std::vector<unsigned> , std::vector<float> > fileParameters = readFile( (char*)filename );

    float p_sum = 0;

    //sum the p-th powers of values of the bitmap values
    for( int i=0; i!= fileParameters.second.size(); i++ )
    {
        p_sum = p_sum + pow(fileParameters.second[i],p);
    }

    return p_sum*(1./pow(256.f,3.f))*(1./2.);

}




