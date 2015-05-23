#include <string>
#include <vector>
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;


//I am intriducing this file to abstract away, as far as possible, process of reading files and the actuall process of subdividing them, converting them to bitmaps and doin other computations.
//Doing so, when new file format will come to the consideration, one could simply sunstitite this file with file which reads new format and move on from there with the software that already exist.
//The parameter sizesOfBitmap is added here for test to check if the input file is correct.

#pragma once


class writeToFileStrucutre
{
public:
    char* filename;
    std::vector<float> dataToFile;
};



void readSizesOfBitmap( string filename , std::vector< unsigned >& sizesOfBitmap )
{
    bool dbg = false;
    if ( dbg ){cout << "readSizesOfBitmap procedure \n";}
    ifstream in;
    filename += "_sizes";
    in.open(filename.c_str());

    if ( !in.good() )
    {
        cerr << "Wrong filename : " << filename << " in the procedure readSizesOfBitmap." << endl;
        throw std::runtime_error("Wrong filename in the procedure readSizesOfBitmap.");
    }

    unsigned dimension = 3;
    if ( dbg ){cout << "Dimension of bitmap : " << dimension << endl;}
    for ( size_t i = 0 ; i != dimension ; ++i )
    {
        int val;
        in >> val;
        sizesOfBitmap.push_back( val );
    }
    in.close();
}//readSizesOfBitmap


bool doesFileExist(const char *filename)
{
  ifstream ifile(filename);
  return ifile;
}

std::pair< std::vector<unsigned> , std::vector<float> > readFile( const char* filename , bool normalize = false )
{
    bool dbg = false;

    //first check if file with binning data exist:
    bool binning = doesFileExist("minMax");
    float min_, max_;
    size_t numberOfBins;
    float dx;

    if ( binning )
    {
        ifstream file;
        file.open("minMax");
        file >> min_;
        file >> max_;
        file >> numberOfBins;
        file.close();
        dx = (max_ - min_)/(float)numberOfBins;

        cerr << "Binning the data. Min : " << min_ << ", max : " << max_ << ", number of bins: " << numberOfBins << endl;

    }

     std::vector< unsigned > sizesOfBitmap(3);
     //readSizesOfBitmap( filename , sizesOfBitmap );
    for(int i=0; i<3; ++i){
        sizesOfBitmap[i] = 256;
    }

     ifstream file( filename , ios::binary | ios::ate );
     unsigned realSizeOfFile = file.tellg();
     file.close();
     realSizeOfFile = realSizeOfFile/sizeof(float);

     unsigned w, h, d;

     w = h = d = ceil(pow( realSizeOfFile , (double)(1/(double)3) ));

     //cout << "Sizes of bitmap : " << w << " x " << h << " x " << d << endl;

     float* slice = new float[w*h*d];
     if (slice == NULL)
     {
        cerr << "Allocation error, cannot allocate " << w*h*d*sizeof(float) << " bytes to store the data from the file. The program will now terminate \n";
        exit(EXIT_FAILURE);
     }

     FILE* fp;
     if ((fp=fopen( filename, "rb" )) == NULL )
     {
         cerr << "Cannot open the file: " << filename << ". The program will now terminate \n";
         exit(1);
     }
     fread( slice,4,w*h*d,fp );
     fclose(fp);


     std::vector<float> data(slice,slice+w*h*d);
     delete[] slice;//????

     if ( normalize )
     {
         //first find min and max of this slice
         float minInThisSlice = INT_MAX;
         float maxInThisSlice = -INT_MAX;
         for ( size_t i = 0 ; i != data.size() ; ++i )
         {
             if ( data[i] < minInThisSlice ){minInThisSlice = data[i];}
             if ( data[i] > maxInThisSlice ){maxInThisSlice = data[i];}
         }
         //and now rescale the data to the [0,1] interval
         for ( size_t i = 0 ; i != data.size() ; ++i )
         {
             data[i] = (data[i]-minInThisSlice)/(maxInThisSlice - minInThisSlice);

             //TODO -- for test only, remove after!!
             if ( (data[i] < 0) || (data[i] > 1) ){cerr << "Error in normalization : " << data[i] << endl;getchar();}


         }
     }


     if ( binning )
     {
         cerr << "Binning the input data \n";
         for ( size_t i = 0 ; i != data.size() ; ++i )
         {
              if ( dbg )
              {
                  cerr << "Now considering : " << data[i] << " for binning " << endl;
              }
              size_t whichBin = (data[i]-min_)/dx;
              data[i] = 0.5*((whichBin*dx+min_)+((whichBin+1)*dx+min_));
              if ( dbg )
              {
                  cerr << "It goes into the bin : [" << (whichBin*dx+min_) << " , " << (whichBin+1)*dx+min_ << "] " << " and therefore get the value : " << data[i] << endl;
                  getchar();
              }
         }
     }

    return std::make_pair( sizesOfBitmap , data );
}//readFile






void writeFileInTakashiFormat( const writeToFileStrucutre& s )
{
    ofstream myfile;
    myfile.open (s.filename, ios::out | ios::binary);
    myfile.write(reinterpret_cast<const char*>(&(s.dataToFile[0])),s.dataToFile.size()*sizeof(float));
    myfile.close();
}//writeFileInTakashiFormat
