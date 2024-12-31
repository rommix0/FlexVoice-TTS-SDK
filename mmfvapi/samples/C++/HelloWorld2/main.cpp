/*************************************************************************
*                                                                        *
*  (C) Copyright 2001 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2001.02.07.                                                  *
*                                                                        *
*************************************************************************/
#include "WavFileWriter.h"

int main()
{
  SpeechParams sp;
  sp.speaker = "Julie";
  sp.samplingFreq = 16000;

  WavFileWriter wfw( "Hello World", "hello.wav", sp );

  if (wfw.writeFile()) return 0;
  return -1;
}
