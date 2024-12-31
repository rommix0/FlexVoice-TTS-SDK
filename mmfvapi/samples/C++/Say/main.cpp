/*************************************************************************
*                                                                        *
*  (C) Copyright 2000 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2001.03.29.                                                  *
*                                                                        *
*************************************************************************/
#include "TextToWav.h"

#include <TTSAPI/FVVersion.h>
#include <iostream>

int main( int argc, char **argv )
{
    if( strcmp( FLEX_VOICE_VERSION, MM_TTSAPI::getVersion() ) != 0 )
    {
        std::cout << "Version error!\n";
        std::cout << "FlexVoice DLL version " << MM_TTSAPI::getVersion();
        std::cout << " does not match\n";
        std::cout << "the required version: " << FLEX_VOICE_VERSION << std::endl;
        std::cout << "Please report this error to the technical support, thanks.\n";
        exit(1);
    }
    
    TextToWav t2w(argc, argv);

    bool ok = true;
    if( ok )
        ok = t2w.parseCmdLine();

    if( ok)
        ok = t2w.prepareText();
    
    if( ok ) 
        ok = t2w.writeWav();

    return !ok;
}
