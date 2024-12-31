/*************************************************************************
*                                                                        *
*  (C) Copyright 2000 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2001.08.27.                                                  *
*                                                                        *
*************************************************************************/

///////////////////////////////////////////////////////////////////////////////
//
// FVNetClient_VBFile.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef MM_FVNETCLIENT_VBFILE_H
#define MM_FVNETCLIENT_VBFILE_H

#include "FVNetCS.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef FVNETCLIENT_VBFILE_DECLSPEC
# ifdef _MSC_VER
#  ifdef FVNETCLIENT_VBFILE_EXPORTS
#    define FVNETCLIENT_VBFILE_DECLSPEC __declspec(dllexport)
#  else
#    define FVNETCLIENT_VBFILE_DECLSPEC __declspec(dllimport)
#  endif
#  if !defined( PASCAL )
#    error macro PASCAL must be defined!
#  endif
# else // !_MSC_VER
#  define FVNETCLIENT_VBFILE_DECLSPEC
#  define PASCAL
# endif
#endif


/** @name Interface for VisualBasic
  */
//@{

/** @name Enumerations
  */
//@{

/** The possible error codes.
  */
enum FVNetClientVBFile_ErrorCode
{ 
    /** The request was successfully done.
      */
        FVNetClientVBFile_Error_OK
,
    /** General failure.
      */
        FVNetClientVBFile_Error_Fail
,
    /** Out of memory.
      */
        FVNetClientVBFile_Error_OutOfMemory
,
    /** Invalid handle.
      */
        FVNetClientVBFile_Error_InvalidHandle
,
    /** Error occured during server connection.
      */
        FVNetClientVBFile_Error_CannotConnect
,
    /** Client is not connected to the server. (see \Ref{FVNetClientVBFile_Connect}).
      */
        FVNetClientVBFile_Error_NotConnected
,
    /** The speak request failed.
      */
        FVNetClientVBFile_Error_CannotSpeak
,
    /** All channels of the server are busy.
      */
        FVNetClientVBFile_Error_ServerBusy
,
    /** The client is speaking and can't recieve more request.
      */
        FVNetClientVBFile_Error_ClientBusy
,
    /** Invalid argument(s).
        One or more arguments passed to the method are invalid.
      */
        FVNetClientVBFile_Error_InvalidArgument
,
    /** Invalid file format.
        The given fileformat is invalid or not supported.
      */
        FVNetClientVBFile_Error_InvalidFileFormat
,
    /** Not implemented.
        The given method is not yet implemented.
      */
        FVNetClientVBFile_Error_NotImplemented
};

/** The supported output formats.
  */
enum FVNetClientVBFile_FileFormat  { /** Invalid file format         
                                       */ 
                                     FF_INVALID,
                                     /** Raw format (without header) 
                                       */ 
                                     FF_RAW,
                                     /** Windows's wav format        
                                       */ 
                                     FF_WAV    
                                   };

//@}

/** @name Functions
  */
//@{

/** Creates a net client that should connect to the server.
    @return The return value is a handle that should be used to reach the client's 
            functionality. The zero value means that the creation failed.
  */
unsigned long FVNETCLIENT_VBFILE_DECLSPEC PASCAL FVNetClientVBFile_Create();


/** Connects the client to the server
    @param handle           The handle created by the \Ref{FVNetClientVBFile_Create}
    @param host             The server's host (e.g. "localhost"). Cannot be NULL or empty.
    @param port             The port used by the server

    @return The return value is an FVNetClientVBFile_ErrorCode value.
  */
int FVNETCLIENT_VBFILE_DECLSPEC PASCAL FVNetClientVBFile_Connect
(           unsigned long                   handle
,   const   char *                          host
,           int                             port 
);


/** Creates speech.
    This method sends the text to the server. The server synthesizes the speech 
    and sends back the generated data to the client. The client 
    stores this data in the speech data buffer. 
    Using the FVNetClientVBFile_GetSpeechData method, the caller is able to 
    read out speech data from the speech data buffer.
    The method waits for the end of the speech generation and returns only after 
    closing the file.
    
    @param handle           The handle created by the \Ref{FVNetClientVBFile_Create} method
    @param text             The text to be synthesized
    @param language         The language (see \Ref{FVNetCSLanguage})
    @param filename         The name of the output file
    @param fileformat       The format of the output file (see \Ref{FVNetClientVBFile_FileFormat})
    @param speaker          The name of requested speaker. If NULL or empty, then
                            there is no requested speaker.
    @param relVolume        The relative volume (in percent)
    @param relSpeed         The relative speed (in percent)
    @param relPitch         The relative pitch (in percent)
    
    @param writeSpeed       The speed of the wave generation (in percent)

    @return The return value is an error code (see \Ref{FVNetClientVBFile_ErrorCode})
  */
int FVNETCLIENT_VBFILE_DECLSPEC PASCAL FVNetClientVBFile_SpeakToFile
(           unsigned long                   handle
,   const   char *                          text
,   const   char *                          language
,   const   char *                          filename
,           FVNetClientVBFile_FileFormat    fileformat
,   const   char *                          speaker
,           int                             relVolume
,           int                             relSpeed
,           int                             relPitch
,           int                             writeSpeed
);

/** Creates speech.
    This method sends the text to the server. The server synthesizes the speech 
    and sends back the generated data to the client. The client 
    stores this data in the speech data buffer. 
    Using the FVNetClientVBFile_GetSpeechData method, the caller is able to 
    read out speech data from the speech data buffer.
    The method doesn't wait for the end of the speech generation it returns 
    immediately. To wait its end you can call the \Ref{FVNetClientVBFile_Wait} method.

    @param handle           The handle created by the \Ref{FVNetClientVBFile_Create} method
    @param text             The text to be synthesized
    @param language         The language (see \Ref{FVNetCSLanguage})
    @param filename         The name of the output file
    @param speaker          The name of requested speaker. If NULL or empty, then
                            there is no requested speaker.
    @param relVolume        The relative volume (in percent)
    @param relSpeed         The relative speed (in percent)
    @param relPitch         The relative pitch (in percent)
    
    @param writeSpeed       The speed of the wave generation (in percent)

    @return The return value is an error code (see \Ref{FVNetClientVBFile_ErrorCode})
  */
int FVNETCLIENT_VBFILE_DECLSPEC PASCAL FVNetClientVBFile_SpeakToFileRequest
(           unsigned long                   handle
,   const   char *                          text
,   const   char *                          language
,   const   char *                          filename
,           FVNetClientVBFile_FileFormat    fileformat
,   const   char *                          speaker
,           int                             relVolume
,           int                             relSpeed
,           int                             relPitch
,           int                             writeSpeed
);

/** Stops the file generation
    @param handle   Client handle created by the \Ref{FVNetClientVBFile_Create} method.
    @return         The return value is an \Ref{FVNetClientVBFile_ErrorCode} value.
  */
int FVNETCLIENT_VBFILE_DECLSPEC PASCAL FVNetClientVBFile_Stop( unsigned long handle );

/** Waits for the end of the speech generation.
    The method returns only after closing the file.
    @param handle   Client handle created by the \Ref{FVNetClientVBFile_Create} method.
    @return         The return value is an \Ref{FVNetClientVBFile_ErrorCode} value.
  */
int FVNETCLIENT_VBFILE_DECLSPEC PASCAL FVNetClientVBFile_Wait( unsigned long handle );

/** Destroys a client.
    @param handle   Client handle created by the \Ref{FVNetClientVBFile_Create} method.
    @return         The return value is an \Ref{FVNetClientVBFile_ErrorCode} value.
  */
int FVNETCLIENT_VBFILE_DECLSPEC PASCAL FVNetClientVBFile_Destroy( unsigned long handle );

/** Sets that the client should be destroyed automatically after 
    finishing the speech generation. After calling this method, 
    the handle should not be used!
*/
int FVNETCLIENT_VBFILE_DECLSPEC PASCAL FVNetClientVBFile_AutoDestroy( unsigned long handle );



//@}

//@}

#ifdef __cplusplus
} // extern "C"
#endif

#endif//MM_FVNETCLIENT_VBFILE_H
