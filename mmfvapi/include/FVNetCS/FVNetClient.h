/*************************************************************************
*                                                                        *
*  (C) Copyright 2000 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created:         2001.06.25.                                          *
*  Last modifified: 2001.08.22.                                          *
*                                                                        *
*************************************************************************/

#ifndef MM_FVNETCLIENT_H
#define MM_FVNETCLIENT_H

#include "FVNetCS.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef FVNETCLIENT_DECLSPEC
# ifdef _MSC_VER
#  ifdef FVNETCLIENT_EXPORTS
#    define FVNETCLIENT_DECLSPEC __declspec(dllexport)
#  else
#    define FVNETCLIENT_DECLSPEC __declspec(dllimport)
#  endif
# else // !_MSC_VER
#  define FVNETCLIENT_DECLSPEC
# endif
#endif

/** The possible error codes.
  */
enum FVNetClientErrorCode   { /** The request was successfully done,
                                */
                              FVNetClientError_OK,
                              /** Invalid handle.
                                */
                              FVNetClientError_InvalidHandle,
                              /**
                                */
                              FVNetClientError_CannotConnect,
                              /**
                                */
                              FVNetClientError_NotConnected,
                              /**
                                */
                              FVNetClientError_TransferError,
                              /** The server doesn't serve the given language. 
                                */
                              FVNetClientError_LanguageNotServed,
                              /** The speak request failed.
                                */
                              FVNetClientError_CannotSpeak,
                              /** All channels of the server are busy.
                                */
                              FVNetClientError_ServerBusy,
                              /** The client is speaking and can't recieve more request
                                */
                              FVNetClientError_ClientBusy
                            };

/** The type of the callback function,
  * @param param    The 'param' parameter of the \Ref{FVNetClientSpeak} method.
  * @param data     The output data-chunk. Its format depends on the 'format' parameter
  *                 of the \Ref{FVNetClientCreate} method.
  * @param size     The size of the data-chunk in bytes.
  * @return         The return value is reserved for the future. In this state it should be 1
  */
typedef int (*FVNetClientCallback)( void* param, const unsigned char* data, int size );

/** Creates a client that should connect to the server.
  * @return The return value is a handle that should be used to reach the client's 
  *         functionality. The zero value means that the creation failed.
  */
unsigned long FVNETCLIENT_DECLSPEC FVNetClientCreate();

/** Connects the client to the server
  * @param handle           The handle created by the \Ref{FVNetClientCreate}
  * @param host             The server's host (e.g. "localhost")
  * @param port             The port used by the server
  *
  * @return The return value is an error code (see \Ref{FVNetClientErrorCode})
  */
int FVNETCLIENT_DECLSPEC FVNetClientConnect( unsigned long handle
                                           , const char * host
                                           , int port 
                                           );

/** Gives the output format used by the server.
  * This method requires the connection to the server.
  * @param format           The coding of the output (see \Ref{FVNetCSOutputFormat})
  * @param sampleFreq       The sampling frequency
  * @param bitResolution    The bit resolution
  *
  * @return The return value is an error code (see \Ref{FVNetClientErrorCode})
  */
int FVNETCLIENT_DECLSPEC FVNetClientGetFormat( unsigned long handle
                                             , FVNetCSOutputFormat& format
                                             , int& sampleFreq
                                             , int& bitResolution
                                             );


/** Creates speech through the callback function.
  * This method send the text to the server, the server synthesizes the speech and sends the
  * generated data back to the client. The here given callback function is called 
  * periodically to give back the speech data.
  *
  * @param handle           The handle created by the \Ref{FVNetClientCreate} method
  * @param text             The text to be synthesized
    @param language         The language
  * @param callback         The callback function (\Ref{FVNetClientCallback}) that recieves the data.
  * @param param            Any user-specific data that is passed to the callback.
  *
  * @return The return value is an error code (see \Ref{FVNetClientErrorCode})
  */
int FVNETCLIENT_DECLSPEC FVNetClientSpeak   ( unsigned long             handle
                                            , const char *              text
                                            , const char *              language
                                            , FVNetClientCallback       callback
                                            , void *                    param
                                            );

/** Waits for the end of the speech.
  */
int FVNETCLIENT_DECLSPEC FVNetClientWait    ( unsigned long             handle );

/** Destroys a client.
  * @param handle   The handle created by the \Ref{FVNetClientCreate} method
  * @return         The return value is an \Ref{FVNetClientErrorCode} value.
  */
int FVNETCLIENT_DECLSPEC FVNetClientDestroy ( unsigned long handle );

//@}


#ifdef __cplusplus
} // extern "C"
#endif

#endif // MM_FVNETCLIENT_H

