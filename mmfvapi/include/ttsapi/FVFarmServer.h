/*************************************************************************
*                                                                        *
*  (C) Copyright 2000 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2001.12.													 *
*                                                                        *
*************************************************************************/

#ifndef MM_FVSERVER_H
#define MM_FVSERVER_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef FVSERVER_DECLSPEC
# ifdef _MSC_VER
#  ifdef FVFARMSERVER_EXPORTS
#   define FVSERVER_DECLSPEC __declspec(dllexport)
#  else
#    define FVSERVER_DECLSPEC __declspec(dllimport)
#  endif
# else
#  define FVSERVER_DECLSPEC
# endif
#endif

/** @name FVFarmServer.h
  */
/*@{*/

/** The possible error codes.
  */
enum FVServerErrorCode      { /** The request was successfully done,
                                */
                              FVServerError_OK,
                              /** Attempt to initialize the server that was previously done.
                                */
                              FVServerError_AlreadyInitialized,
                              /** Attempt to use the server without initializing.
                                */
                              FVServerError_NotInitialized,
                              /** The engine cannot be initialized. 
                                * This problem can be caused a bad installation or 
                                * the very low system resources
                                */
                              FVServerError_CannotInitialize,
                              /** The engine cannot be uninitialized.
                                */
                              FVServerError_CannotUninitialize,
                              /** The given language cannot be loaded
                                */
                              FVServerError_LanguageNotLoaded,
                              /** The given language is not loaded yet
                                */
                              FVServerError_CannotLoadLanguage,
                              /** The engine cannot speak.
                                */ 
                              FVServerError_CannotSpeak,
                              /** All channels are busy.
                                */
                              FVServerError_Busy
                            };

/** The possible output formats.
  */
enum FVServerOutputFormat   { /** Invalid output format.
                                * Used to indicate that the format hasn't been set yet
                                */
                              FVServerOutputFormat_Invalid,
                              /** Signed PCM format.
                                * The output is given as samples (with the given bit resolution)
                                * and its value should be treat as signed.
                                */
                              FVServerOutputFormat_PCM_Signed,
                              /** Unsigned PCM format.
                                * The output is given as samples (with the given bit resolution)
                                * and its value should be treat as unsigned.
                                */
                              FVServerOutputFormat_PCM_Unsigned,
                              /** ULAW format.
                                * The output data contains the samples in a coded format.
                                * The bit resolution should be set to 16 that will be coded
                                * to 8 bit.
                                */
                              FVServerOutputFormat_ULAW,
                              /** ALAW format.
                                * The output data contains the samples in a coded format.
                                * The bit resolution should be set to 16 that will be coded
                                * to 8 bit.
                                */
                              FVServerOutputFormat_ALAW,
                              /** VOX format
                                * The output data contains the samples in a coded format.
                                * The bit resolution should be set to 16 that will be coded
                                * to 8 bit.
                                */
                              FVServerOutputFormat_VOX,
                              /** VIS format
                                * The output data contains the samples in a coded format.
                                * The bit resolution should be set to 16
                                */
                              FVServerOutputFormat_VIS
                            };

/** The type of the callback function,
  * This function is called for passing the data from the server to the client.
  * @param param    The 'param' parameter of the \Ref{FVServerSpeak} method.
  * @param data     The output data-chunk. Its format depends on the 'format' parameter
  *                 of the \Ref{FVServerSpeak} method.
  * @param size     The size of the data-chunk in bytes.
  * @return         The return value indicates if the speak is to stop or not.
  *                 In case of returning with 0 the speak will be stopped.
  */
typedef int     (*FVServerCallback)( void* param, const unsigned char* data, int size );


/** Initializes the server.
  * @param path         The path where FlexVoice's datas can be found
  * @return The return value is an error code (see \Ref{FVServerErrorCode})
  */
int FVSERVER_DECLSPEC FVServerInitialize    ( const char *  path );

/** Loads a language.
  * @param  lang        The language to load.
  * @param instances    The maximum number of instances that the server should handle
  *                     This value depends of the computer's power, the best was is
  *                     to measure it.
  * @param format           The required output format (see \Ref{FVServerOutputFormat})
  * @param sampleFreq       The sampling frequency
  * @param bitResolution    The bit resolution (mainly for PCM output)
  * @return The return value is an error code (see \Ref{FVServerErrorCode})
  */
int FVSERVER_DECLSPEC FVServerLoadLanguage  ( const char*       lang 
                                            , int				instances 
                                            , int               format
                                            , int               sampleFreq
                                            , int               bitResolution
											 );

/** Uninitializes the server
  * @return The return value is an error code (see \Ref{FVServerErrorCode})
  */
int FVSERVER_DECLSPEC FVServerUninitialize  ();

/** Creates speech through the callback function.
  * The here given callback function is called periodically to give back the speech data.
  * (see \Ref{FVServerCallback})
  *
  * @param text             The text to be synthesized
  * @param callback         The callback function (\Ref{FVServerCallback}) that recieves the data.
  * @param param            Any user-specific data that is passed to the callback.
  *
  * @return The return value is an error code (see \Ref{FVServerErrorCode})
  */
int FVSERVER_DECLSPEC FVServerSpeak         ( const char *      text
                                            , const char*       language
                                            , FVServerCallback  callback
                                            , void *            param );

/** Waits untill all requests is done.
  * @return The return value is an error code (see \Ref{FVServerErrorCode})
  */
int FVSERVER_DECLSPEC FVServerWait          ();


/*@}*/

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MM_FVSERVER_H
