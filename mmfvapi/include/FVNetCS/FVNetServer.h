/*************************************************************************
*                                                                        *
*  (C) Copyright 2000 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2001.06.26.                                                  *
*                                                                        *
*************************************************************************/

#ifndef MM_FVNETSERVER_H
#define MM_FVNETSERVER_H

#include "FVNetCS.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef FVNETSERVER_DECLSPEC
# ifdef _MSC_VER
#  ifdef FVNETSERVER_EXPORTS
#   define FVNETSERVER_DECLSPEC __declspec(dllexport)
#  else
#    define FVNETSERVER_DECLSPEC __declspec(dllimport)
#  endif
# else
#  define FVNETSERVER_DECLSPEC
# endif
#endif

/** @name FVNetServer.h
  */
//@{


/** The possible error codes.
  */
enum FVNetServerErrorCode   { /** The request was successfully done,
                                */
                              FVNetServerError_OK,
                              /** Invalid handle
                                */
                              FVNetServerError_InvalidHandle,
                              /** Cannot initialize engine.
                                */
                              FVNetServerError_CannotInitEngine,
                              /** Server was not initialized
                                */
                              FVNetServerError_NotInitialized,
                              /** Cannot load language
                                */
                              FVNetServerError_CannotLoadLanguage,
                              /** No language was loaded
                                */
                              FVNetServerError_NoLanguageLoaded,
                              /** The given port cannot be used.
                                */
                              FVNetServerError_CannotOpenPort
                            };

/** Creates a server.
  * @return The return value is a handle that should be used to reach the server's 
  *         functionality. The zero value means that the creation failed.
  */
unsigned long FVNETSERVER_DECLSPEC FVNetServerCreate    ();

/** Initializes the server.
  * @param handle           The handle created by the \Ref{FVNetServerCreate}
  * @param path             The path where FlexVoice's datas can be found
  * @param port             The port where the server should listen
  * @param instances        The maximum number of instances that the server should handle
  *                         This value depends of the computer's power, the best was is
  *                         to measure it.
  * @param format           The required output format (see \Ref{FVNetCSOutputFormat})
  * @param sampleFreq       The sampling frequency
  * @param bitResolution    The bit resolution (mainly for PCM output)
  *
  * @return The return value is an error code (see \Ref{FVNetClientErrorCode})
  */
int FVNETSERVER_DECLSPEC FVNetServerInitialize( unsigned long       handle
                                              , const char *        path
                                              , int                 port 
                                              , int                 instances 
                                              , FVNetCSOutputFormat format
                                              , int                 sampleFreq
                                              , int                 bitResolution
                                              );

/** Loads a language.
  * @param handle           The handle created by the \Ref{FVNetServerCreate}
  * @param language         the language to be loaded 
  * @return                 The return value is an error code (see \Ref{FVNetClientErrorCode})
  */
int FVNETSERVER_DECLSPEC FVNetServerLoadLanguage( unsigned long handle
                                                , const char* language
                                                );

/** Starts the listening
  * @param handle       The handle created by the \Ref{FVNetServerCreate}
  * @return The return value is an error code (see \Ref{FVNetClientErrorCode})
  */
int FVNETSERVER_DECLSPEC FVNetServerStartListening( unsigned long handle );

/** Destroys the server.
  * The destruction will wait until all the client finishes the communication.
  * @param handle       The handle created by the \Ref{FVNetServerCreate}
  */
int FVNETSERVER_DECLSPEC FVNetServerDestroy( unsigned long handle );

//@}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MM_FVNETSERVER_H

