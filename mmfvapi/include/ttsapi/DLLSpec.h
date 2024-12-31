/*************************************************************************
*                                                                        *
*  (C) Copyright 2000-2001 Mindmaker Ltd.                                *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2000.11.17.                                                  *
*                                                                        *
*************************************************************************/

#ifndef MM_TTSAPI_DLL_SPEC_H
#define MM_TTSAPI_DLL_SPEC_H

#ifndef FLEXVOICE_DECLSPEC
# ifdef _MSC_VER
#  ifdef FLEXVOICE_EXPORTS
#   define FLEXVOICE_DECLSPEC __declspec(dllexport)
#  else
#   ifndef FLEXVOICE_ALLSTATIC
#     define FLEXVOICE_DECLSPEC __declspec(dllimport)
#   else
#     define FLEXVOICE_DECLSPEC
#   endif
#  endif
# else
#  define FLEXVOICE_DECLSPEC
# endif
#endif

// Needed for instantiation of exported template classes
#ifdef _MSC_VER
# ifndef EXPIMP_TEMPLATE
#  ifdef FLEXVOICE_EXPORTS
#   define EXPIMP_TEMPLATE 
#  else
#   define EXPIMP_TEMPLATE extern
#  endif // FLEXVOICE_EXPORTS
# endif // EXPIMP_TEMPLATE
#endif //_MSC_VER

#endif // MM_TTSAPI_DLL_SPEC_H
