/***********************************************************************
* Copyright (c) 2000 Mindmaker Ltd.  All rights reserved.              *
*                                                                      *
* Sample file for encoding speech represented by linear samples.       *
*                                                                      *
************************************************************************
* File name    : pcm2code.cpp                                          *
* Environment  : Win32                                                 *
* Compiler     : MSVC++ 6.0                                            *
************************************************************************/

#include <vector>
#include <iostream>
#include <fstream>
#include "ulaw.h"
#include "adpcm.h"

int read_pcm(const char* fn, std::vector<short>& wave, int& samplingfreq);
int write_ulaw(const char* fn, std::vector<unsigned char>& ulaw, int samplingfreq);

void main(int argc, char* argv[])
{
  if(argc < 4)
  {
    std::cout << "Usage: pcm2code.exe pcm_file.wav coded_file coding_type ('ulaw' or 'vox')\n";
    return;
  }
  
  int fs;
  std::vector<short> wave;
  // Read in linearly scaled, 16 bit sample values
  int rc = read_pcm(argv[1], wave, fs);
  if(rc != 0)
  {
    std::cout << "Error reading input pcm file.\n";
    return;
  }

  if(strcmp(argv[3], "ulaw") == 0)
  {
    std::vector<unsigned char> ulaw(wave.size());
    // convert linear, 16 bit sample values to ulaw, 8 bit sample values
    Mindmaker::linear2ulaw(wave.begin(), wave.end(), ulaw.begin());
    // Write out ulaw, 8 bit sample values
    rc = write_ulaw(argv[2], ulaw, fs);
    if(rc != 0)
    {
      std::cout << "Error writing output ulaw file.\n";
      return;
    }
  }
  else
  {
    if(strcmp(argv[3], "vox") == 0)
    {
      Mindmaker::ADPCMStatus coder_stat;
      std::vector<unsigned char> outputSamples;
      
      //Initialize the coder.
      Mindmaker::adpcm_init( &coder_stat );
    
      //perform conversion
      Mindmaker::linear2adpcm(wave, outputSamples, &coder_stat);
    
      //write out output
      std::ofstream os(argv[2], std::ios::binary);
      if(!os.good())
      {
        std::cout << "Error writing output vox file.\n";
        return;
      }
      os.write((char*) &outputSamples[0], outputSamples.size());
    }
    else
    {
      std::cout << "Coding type must be 'ulaw' or 'vox'.\n";
      return;    
    }
  }
}


/** Read in linearly scaled, 16 bit, mono sample values from a Windows PCM wave file.
    @param fn name of wave file to read in.
    @param wave vector of short values to put linearly scaled samples.
    @param samplingfreq Sampling frequency.
    Returns 0 if OK, nonzero otherwise.
*/
int read_pcm(const char* fn, std::vector<short>& wave, int& samplingfreq)
{
  FILE* file = fopen( fn, "rb" );
  if( file == NULL )
  {
    std::cout << "Error opening input file.\n";
    return(-1);
  }

  unsigned long   wavelength;
  unsigned long   waveformatsize;
  unsigned short  format;
  unsigned short  channels;
  unsigned long   fs;
  unsigned long   bytepersec;
  unsigned short  blockalign;
  unsigned short  bitspersample;
  unsigned short  formatextlength;
  unsigned long   datalength;

  // reading header
  char temp[128];
  fread( temp, 1, 4, file );
  //error check
  if( strncmp(temp, "RIFF", 4) != 0 )
  {
    std::cout << "Bad file format.\n";
    return(-1);
  }
  
  fread( &wavelength, sizeof(wavelength), 1, file );

  fread( temp, 1, 4, file );
  //error check
  if( strncmp(temp, "WAVE", 4) != 0 )
  {
    std::cout << "Bad file format.\n";
    return(-1);
  }

  fread( temp, 1, 4, file );
  //error check
  if( strncmp(temp, "fmt", 3) != 0 )
  {
    std::cout << "Bad file format.\n";
    return(-1);
  }

  // reading wave format
  fread( &waveformatsize , sizeof(waveformatsize ), 1, file );
  fread( &format         , sizeof(format         ), 1, file );
  //error check
  if( format != 1)
  {
    std::cout << "Input wave is not in PCM format.\n";
    return(-1);
  }

  fread( &channels       , sizeof(channels       ), 1, file );
  //error check
  if( channels != 1)
  {
    std::cout << "Input wave is not in mono format.\n";
    return(-1);
  }

  unsigned structnum = sizeof(format) + sizeof(channels);
  if (structnum < waveformatsize)
  {
    fread( &fs             , sizeof(fs             ), 1, file );
    structnum += sizeof(fs);
  }
  if (structnum < waveformatsize)
  {
    fread( &bytepersec     , sizeof(bytepersec     ), 1, file );
    structnum += sizeof(bytepersec);
  }
  if (structnum < waveformatsize)
  {
    fread( &blockalign     , sizeof(blockalign     ), 1, file );
    structnum += sizeof(blockalign);
  }
  if (structnum < waveformatsize)
  {
    fread( &bitspersample  , sizeof(bitspersample  ), 1, file );
    structnum += sizeof(bitspersample);
  }
  if (structnum < waveformatsize)
  {
    fread( &formatextlength, sizeof(formatextlength), 1, file );
    structnum += sizeof(formatextlength);
  }

  samplingfreq = fs;

  while(1)
  {
    fread( temp, 1, 4, file );
  //error check
  if( feof(file) != 0)
  {
    std::cout << "Bad file format.\n";
    return(-1);
  }

  /* moves back three bytes if not "data" */
  if(strncmp(temp, "data", 1) != 0)
    fseek(file, -3, SEEK_CUR);
  else
    break;
  }


  fread( &datalength, sizeof(datalength), 1, file );

  wave.resize(datalength / blockalign);
  short s;
  for (unsigned i = 0; i < wave.size(); ++i)
  {
    fread( &s, sizeof(short), 1, file );
    wave[i] = s;
  }

  return(0);
}
 
/** Write out ulaw, 8 bit, mono sample values to a Windows ulaw wave file.
    @param fn name of wave file to write out.
    @param wave vector of unsigned char values to put ulaw coded samples.
    @param samplingfreq Sampling frequency.
*/
int write_ulaw(const char* fn, std::vector<unsigned char>& ulaw, int samplingfreq)
{
  FILE * file = fopen( fn, "wb" );
  if( file == NULL )
  {
    std::cout << "Error opening output file.\n";
    return(-1);
  }

  unsigned long   waveformatsize  = 5*sizeof(unsigned short)+2*sizeof(unsigned long);//sizeof( WAVEFORMATEX ) = 18
  unsigned short  format          = 7; //ulaw
  unsigned short  channels        = 1; //Mono
  unsigned long   fs              = long(samplingfreq);
  unsigned short  bitspersample   = 8;
  unsigned short  blockalign      = channels * bitspersample / 8;
  unsigned long   bytepersec      = blockalign * fs;

  unsigned short  formatextlength = 0;
  unsigned long   datalength      = blockalign * ulaw.size();
  unsigned long   wavelength = 8 // "WAVEfmt "
                             + sizeof(waveformatsize) + waveformatsize
                             + 4 // "data"
                             + sizeof(datalength) + datalength
                             ;
  

  // writing header
  fwrite( "RIFF", 1, 4, file );
  
  fwrite( &wavelength, sizeof(unsigned long), 1, file );

  fwrite( "WAVE", 1, 4, file );
  fwrite( "fmt ", 1, 4, file );

  // writing wave format
  fwrite( &waveformatsize , sizeof(waveformatsize ), 1, file );
  
  fwrite( &format         , sizeof(format         ), 1, file );
  fwrite( &channels       , sizeof(channels       ), 1, file );
  fwrite( &fs             , sizeof(fs             ), 1, file );
  fwrite( &bytepersec     , sizeof(bytepersec     ), 1, file );
  fwrite( &blockalign     , sizeof(blockalign     ), 1, file );
  fwrite( &bitspersample  , sizeof(bitspersample  ), 1, file );
  fwrite( &formatextlength, sizeof(formatextlength), 1, file );

  fwrite( "data", 1, 4, file );
  fwrite( &datalength, sizeof(datalength), 1, file );

  unsigned char s;
  for (unsigned i = 0; i < ulaw.size(); ++i)
  {
    s = ulaw[i];
    fwrite( &s, sizeof(unsigned char), 1, file );
  }

  return(0);
}