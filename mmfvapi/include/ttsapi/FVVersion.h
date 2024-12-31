/*************************************************************************
*                                                                        *
*  (C) Copyright 2000-2001 Mindmaker Ltd.                                *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2001.03.13.                                                  *
*                                                                        *
*************************************************************************/

#ifndef MM_VERSION_H
#define MM_VERSION_H

#include "DLLSpec.h"

#define FLEX_VOICE_VERSION  "3_01_001"

/** @name FVVersion.h 
*/
//@{

namespace MM_TTSAPI
{

/** Returns version information of the engine loaded into memory.
    Use this function to check if the build version of the headers that
    you are using (in your project) is the same as the version of the engine.
    It might be that these versions are different but your code still compiles.
    Note that the version information changes only when you need to recompile
    your code in order to get your code to work correctly with the new "version".
    There are changes (e.g. bugfixes) that do not require recompilation
    of your code. For such changes the version information will not be
    changed. If the version information is changed the name of the main #dll#
    file (#so# file on Linux; on other operating systems the extension may
    be something else) will be changed at the same time, so your old code will not be 
    broken when someone installs an upgraded package of FlexVoice on the system.
    However, this also means that your old code will not take advantage of
    the new features of the updated FlexVoice package. Upon version 
    number changes, the changes will be highlighted in the \Ref{Release Notes}.
    It is our intention and best interest to minimize the frequency
    of version number changes.

    Since your code may compile even with old include files and a newer
    dll, it is a good practice and is {\em very strongly recommended}
    that you check the
    version information at start-up, e.g. by adding the line
    ##\\
    #assert( strcmp( MM_TTSAPI::getVersion(), FLEX_VOICE_VERSION ) == 0 )#\\
    ##\\
    to the beginning of your program.
    These lines assume that you have included #cassert# and this file (#Version.h#)
    at the beginning of your file.
  */
FLEXVOICE_DECLSPEC const char * getVersion();

}

//@}

#endif //MM_VERSION_H
