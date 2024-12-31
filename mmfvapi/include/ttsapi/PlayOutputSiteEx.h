/*************************************************************************
*                                                                        *
*  (C) Copyright 2000 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2001.01.15.                                                  *
*                                                                        *
*************************************************************************/

#ifndef MM_TTSAPI_PLAY_OUTPUT_SITE_EX_H
#define MM_TTSAPI_PLAY_OUTPUT_SITE_EX_H

#include "Engine.h"
#include "DLLSpec.h"

struct IAudio;

/** @name PlayOutputSiteEx.h */
//@{

namespace MM_TTSAPI
{

/** A WavePlayOutputSite supports playing waves through a hardware device.
  * A WavePlayOutputSite requires the computer
  * to have a sound output device.
  * It is not necessarily supported on all platforms.
  *
  * WavePlayOutputSites have an associated state. The site can be
  * open or closed. 
  * \Ref{play}ing through the site does not work if the site's state
  * is closed.
  *
  * The rationale is that certain wave devices can only be used by a single
  * source. Opening the site corresponds to locking the wave device
  * and closing the site corresponds to releasing the device.
  * A friendly omplementation closes the device whenever it is not
  * needed. 
  *
  * The initial state of a site is closed.
  * Methods \Ref{open} and \Ref{close} can be used to 
  * manipulate the state.
  *
  * Currently, this class is only supported on Win32 platforms.
  */
class FLEXVOICE_DECLSPEC WavePlayOutputSiteEx : public IWaveOutputSite
{
public:
    /** @name Methods inherited from \Ref{IWaveOutputSite} 
      * @see IWaveOutputSite,INotifyDispatcher
      */
    //@{
    ///
    virtual const IOutputFormat& getOutputFormat() const;

    ///
    virtual void        pause();
    ///
    virtual void        play();
    ///
    virtual void        clear();
    /** Blocks until the content of the buffer is successfully copied.
      */
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
    
    //@}
public:
// WavePlayOutputSiteEx
    /// Construct a play output site supporting the given wave output format.
                        WavePlayOutputSiteEx( const WaveOutputFormat& of, IAudio* audio );
    ///
    virtual             ~WavePlayOutputSiteEx();
    
    /// 
    virtual void        open( WaveOutputFormat& wf );
    ///
    virtual void        close();

    //xx interaction of open, close with play, buffers, exceptions?
    //xx decision is to throw exception in put when object is not yet opened.

private:
                            /// WavePlayOutputSiteEx shouldn't be copied!
                            WavePlayOutputSiteEx( const WavePlayOutputSiteEx& );
                            /// WavePlayOutputSiteEx shouldn't be copied!
    WavePlayOutputSiteEx&   operator=         ( const WavePlayOutputSiteEx& );

    class WavePlayOutputSiteImpl* m_implementation;
};

} // namespace MM_TTSAPI

//@} // PlayOutputSiteEx.h

#endif // MM_TTSAPI_PLAY_OUTPUT_SITE_EX_H
