/*************************************************************************
*                                                                        *
*  (C) Copyright 2000 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2002.05.03.                                                  *
*                                                                        *
*************************************************************************/

#ifndef MM_TTSAPI_FVLANGUAGE_H
#define MM_TTSAPI_FVLANGUAGE_H

#include "DLLSpec.h"

namespace MM_TTSAPI
{

/** Languages known by the engine.
  */
typedef int Language;

/** Currently supported languages
 */

const Language LNG_INVALID      = 0x0000;
const Language LNG_ENGLISH      = 0x0409;
const Language LNG_HUNGARIAN    = 0x040e;

FLEXVOICE_DECLSPEC Language     getLangID   ( const char* langname );
FLEXVOICE_DECLSPEC const char*  getLangName ( Language language );

}

#endif // MM_TTSAPI_FVLANGUAGE_H
