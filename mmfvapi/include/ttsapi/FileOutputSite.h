/*************************************************************************
*                                                                        *
*  (C) Copyright 2000-2001 Mindmaker Ltd.                                *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2000.10.06.                                                  *
*                                                                        *
*************************************************************************/

#ifndef MM_TTSAPI_FILE_OUTPUT_SITE_H
#define MM_TTSAPI_FILE_OUTPUT_SITE_H

#include "Engine.h"
#include "DLLSpec.h"

/** @name FileOutputSite.h */
//@{

namespace MM_TTSAPI
{

/** A WaveFileOutputSite supports saving speech into a PCM formatted "WAV" file.
  * A WaveFileOutputSite does NOT require a sound output device and is thus
  * supported on all platforms.
  *
  * WaveFileOutputSites have an associated state. The site can be
  * open or closed. 
  * \Ref{play}ing through the site will not work if the site's state
  * is closed.
  *
  * Opening the site corresponds to opening the file
  * and closing the site corresponds to closing the file.
  *
  * The initial state of a site is closed.
  * Methods \Ref{open} and \Ref{close} can be used to 
  * manipulate the state.
  */
class FLEXVOICE_DECLSPEC WaveFileOutputSite : public virtual IWaveOutputSite
{
public:
// IOutputSite
    ///
    virtual const IOutputFormat& getOutputFormat() const;
    ///
    virtual void        pause();
    ///
    virtual void        play();
    ///
    virtual void        clear();
    ///
    virtual void        put( unsigned char * buffer, unsigned int size );
    ///
    virtual void        setBookmark( Bookmark* bookmark );
    /// 
    virtual void        sendBookmark( Bookmark& bookmark );
    ///
    virtual void        registerNotify      ( INotify * notify, const BookmarkTypeList& types );
    ///
    virtual void        unregisterNotify    ( INotify * notify );
    ///
    virtual void        addBookmarkTypes    ( INotify * notify, const BookmarkTypeList& types );
    ///
    virtual void        removeBookmarkTypes ( INotify * notify, const BookmarkTypeList& types );

    ///
    virtual bool        set( const char * name, bool        data, int index=0 );
    ///                 
    virtual bool        get( const char * name, bool&       data, int index=0 ) const;
    ///                 
    virtual bool        set( const char * name, char        data, int index=0 );
    ///                 
    virtual bool        get( const char * name, char&       data, int index=0 ) const;
    ///                 
    virtual bool        set( const char * name, int         data, int index=0 );
    ///                 
    virtual bool        get( const char * name, int&        data, int index=0 ) const;
    ///                 
    virtual bool        set( const char * name, double      data, int index=0 );
    ///                 
    virtual bool        get( const char * name, double&     data, int index=0 ) const;
    ///                 
    virtual bool        set( const char * name, const char* data, int index=0 );
    ///                 
    virtual bool        get( const char * name, char* buffer, int buffsize, int* reqsize=0, int index=0 ) const;
                        
    ///                 
    virtual bool        setArraySize( const char * name, int  size );
    ///                 
    virtual bool        getArraySize( const char * name, int& size ) const;

    ///
    virtual const IAttribute*   getAttribute( const char * name ) const;
    ///
    virtual IAttribute*         getAttribute( const char * name );
    
public:
    enum FileFormat { /** Invalid file format         */ FF_INVALID
                    , /** Raw format (without header) */ FF_RAW
                    , /** Windows's wav format        */ FF_WAV
                    };
    ///
                        WaveFileOutputSite( const WaveOutputFormat& wf );
    ///
    virtual             ~WaveFileOutputSite();
    ///
    virtual void        open( const char * filename, FileFormat ff = FF_WAV  );
    ///
    virtual void        close();

private:
                        /// WaveFileOutputSite shouldn't be copied
                        WaveFileOutputSite( const WaveFileOutputSite& );
                        /// WaveFileOutputSite shouldn't be copied
    WaveFileOutputSite& operator=         ( const WaveFileOutputSite& );

    class WaveFileOutputSiteImpl* m_implementation;
};

} // namespace MM_TTSAPI

//@} // FileOutputSite.h

#endif // MM_TTSAPI_FILE_OUTPUT_SITE_H
