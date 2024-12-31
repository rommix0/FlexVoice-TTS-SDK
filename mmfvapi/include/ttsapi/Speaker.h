/*************************************************************************
*                                                                        *
*  (C) Copyright 2000-2001 Mindmaker Ltd.                                *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2000.10.05.                                                  *
*                                                                        *
*************************************************************************/

#ifndef MM_TTSAPI_SPEAKER_H
#define MM_TTSAPI_SPEAKER_H

#include "DLLSpec.h"
#include "Attribute.h"

/** @name Speaker.h
 */

namespace MM_TTSAPI
{

/** Speaker class.
 *  The valid parameter-names are the following:
 *  ( The quoted value should be used as the name of the parameter,
 *    the next field is the parameter type. In case of strings the number after the 
 *    type shows the maximal size of the parameter )
 *  
 *  "version"               string[100]     The name of the speaker
 *  "name"                  string[50]      The name of the speaker
 *  "filename"              string[500]     The last used filename
 *  "gender"                string[20]      
 *  "age"                   string[20]
 *  "synthType"             string[20]
 *  "voiceDescr"            string[50]
 *  "boundaryModel"         string[50]
 *  "boundaryPostModel"     string[50]
 *  "prominenceModel"       string[50]
 *  "durationModel"         string[50]
 *  "pitchModel"            string[50]
 *  "loudnessModel"         string[50]
 *  "vsShape"               string[50]
 *  "styleDescr"            string[50]
 *  "volumeSmoothWindow"    int
 *  "volume"                double
 *  "speechRate"            double
 *  "speedWPM"              int
 *  "defaultPitch"          int
 *  "pitchMin"              int
 *  "pitchMax"              int
 *  "pitchRate"             double
 *  "singingPitchRate"      double
 *  "intonationLevel"       double
 *  "headsize"              double
 *  "tilt"                  double
 *  "richness"              double
 *  "breathiness"           double
 *  "creakiness"            double
 *  "smoothness"            double
 *  "fricationRate"         double
 *  "plosiveRate"           double
 *   
 *  "subharmonicFilter"         structure:Filter
 *  "voicingSourceHarmFilter"   structure:Filter
 *  "voicingSourceNoiseFilter"  structure:Filter
 *  "equalizer"                 structure:Equalizer
 *  "vsNoiseParams"             structure:NoiseParams
 *  
 *  Structures:
 *
 *  Filter:
 *      "lowpass"               bool
 *      "d1"                    double
 *      "d2"                    double
 *      "f1"                    int
 *      "f2"                    int
 *
 *  Equalizer:
 *      "f0"                    int-array
 *      "gain"                  double-array
 *      "bw"                    double-array
 *      "channels"              int
 *
 *      Usage example:
 *      ITTSAttribute * equalizer = attr->getAttribute( "equalizer" );    
 *      if( equalizer != NULL )
 *      {
 *          equalizer->set( "channels"     , 10                    );
 *          for( int i=0; i < 10; ++i )
 *          {
 *              equalizer->set( "f0"  , 0  , i );
 *              equalizer->set( "gain", 0.0, i );
 *              equalizer->set( "bw"  , 100, i );
 *          }
 *      }
 *       ITTSAttribute * equalizer = attr->getAttribute( "equalizer" );    
 *      if( equalizer != NULL )
 *      {
 *          int count=0;
 *          equalizer->get( "channels", count );
 *          speaker.m_equalizer.setChannels( count );
 *          for( int i=0; i < count; ++i )
 *          {
 *              int     f0;
 *              double  bw;
 *              double  gain;
 *      
 *              equalizer->get( "f0"  , f0  , i );
 *              equalizer->get( "gain", bw  , i );
 *              equalizer->get( "bw"  , gain, i  );
 *              speaker.m_equalize->setChannelParams( f0, gain, bw );
 *          }
 *      }
 *
 *  NoiseParams:
 *      "peekAmpl"              double
 *      "startPos"              double
 *      "floor"                 double
 *
 *  Obsolate parameters: (no meaning anymore, garantied that it will 
 *                        be not used in the future)
 *  - durationDescr
 *  - durationStat
 *  - pitchDescr
 *  - pitchQDescr
 *  - volumeDescr
 *  - volumeStat
 */
class FLEXVOICE_DECLSPEC Speaker : public IAttribute
{
public:
// Speaker
                        Speaker();
                        Speaker( const Speaker& from );
                        ~Speaker();
    ///
    Speaker&            operator =  ( const Speaker& );
    ///
    bool                operator <  ( const Speaker& ) const;
    ///
    bool                operator == ( const Speaker& ) const;
    /// loads the speaker definition from a file
    void                load( const char * filename );
    /// saves the given speaker definition to a file
    void                save( const char * filename );

// IAttribute
    virtual bool        set( const char * name, bool  data, int index=0 );
    virtual bool        get( const char * name, bool& data, int index=0 ) const;
                        
    virtual bool        set( const char * name, char  data, int index=0 );
    virtual bool        get( const char * name, char& data, int index=0 ) const;
                        
    virtual bool        set( const char * name, int   data, int index=0 );
    virtual bool        get( const char * name, int&  data, int index=0 ) const;
                        
    virtual bool        set( const char * name, double  data, int index=0 );
    virtual bool        get( const char * name, double& data, int index=0 ) const;
                        
    virtual bool        set( const char * name, const char * data, int index=0 );
    virtual bool        get( const char * name, char* buffer, int buffsize, int* reqsize=0, int index=0 ) const;

    virtual bool        setArraySize( const char * name, int  size );
    virtual bool        getArraySize( const char * name, int& size ) const;

    virtual const IAttribute*   getAttribute( const char * name ) const;
    virtual IAttribute*         getAttribute( const char * name );


public:
// This member is for hide the implementation details not for the users. 
// It is public only because the engine itself should reach it from different
// classes that cannot be listed here as friend class.
    class SpeakerImpl *  m_implementation;
};

///
class FLEXVOICE_DECLSPEC SpeakerMapIterator
{
public:
                        ///
                        SpeakerMapIterator();
                        ///
                        SpeakerMapIterator( const SpeakerMapIterator& from );
                        
                        ~SpeakerMapIterator();
        
    ///
    const char *        getKey    () const;

    ///
    Speaker&            operator* ();
    ///
    Speaker*            operator->();
    ///
    SpeakerMapIterator& operator= ( const SpeakerMapIterator& from );
    ///
    SpeakerMapIterator  operator++();
    ///
    SpeakerMapIterator  operator++(int);
    ///
    bool                operator==( const SpeakerMapIterator& comp );
    ///
    bool                operator!=( const SpeakerMapIterator& comp );

private:
    class SpeakerMapIteratorImpl *  m_implementation;

    friend class ConstSpeakerMapIterator;
    friend class SpeakerMap;
};

class FLEXVOICE_DECLSPEC ConstSpeakerMapIterator
{
public:
                        ///
                        ConstSpeakerMapIterator();
                        ///
                        ConstSpeakerMapIterator( const ConstSpeakerMapIterator& from );
                        ///
                        ConstSpeakerMapIterator( const SpeakerMapIterator& from );
                        ~ConstSpeakerMapIterator();
        
    ///
    const char *        getKey    () const;

    ///
    const Speaker&              operator* () const;
    ///
    const Speaker*              operator->() const;
    ///
    ConstSpeakerMapIterator&    operator= ( const ConstSpeakerMapIterator& from );
    ///
    ConstSpeakerMapIterator     operator++();
    ///
    ConstSpeakerMapIterator     operator++(int);
    ///
    bool                        operator==( const ConstSpeakerMapIterator& comp );
    ///
    bool                        operator!=( const ConstSpeakerMapIterator& comp );

private:
    class ConstSpeakerMapIteratorImpl *  m_implementation;

    friend class SpeakerMap;
};

///
class FLEXVOICE_DECLSPEC SpeakerMap
{
public:
    ///
    typedef const char*             key_type;
    ///
    typedef Speaker                 mapped_type;
    ///
    typedef Speaker                 referent_type;
    
    ///
    typedef SpeakerMapIterator      iterator;
    ///
    typedef ConstSpeakerMapIterator const_iterator;

                    ///
                    SpeakerMap();
                    ///
                    SpeakerMap( const SpeakerMap& from );
                    ///
                    ~SpeakerMap();

    ///
    SpeakerMap&     operator=( const SpeakerMap& from );

    ///
    const_iterator  begin () const;
    ///
    const_iterator  end   () const;

    ///
    iterator        begin ();
    ///
    iterator        end   ();
    
    ///
    iterator        erase ( iterator it );
    ///
    void            erase ( iterator first, iterator last );
    ///
    void            erase ( const char * speaker );

    ///
    void            clear ();
    ///
    int             size  () const;
    ///
    bool            empty () const;

    ///
    iterator        insert( const char * key, const Speaker& value );
    ///
    const_iterator  find  ( const char * key ) const;
    ///
    iterator        find  ( const char * key );

    ///
    Speaker&        operator[]( const char * key );

    /** This is the normal way to initialize a speaker-map (besides copying an existing one).
      * Note that this does not load the speaker definitions. 
      * @param filename    name of the speaker map file.
      * @param defaultpath the speaker map file contains names of speaker filenames.
      *                    this parameter instructs 
      *                    path used in searching for the speaker 
      */
    void    load( const char * filename, const char * defaultpath );

    ///
    void    save( const char * filename, const char * defaultpath );

private:
    class SpeakerMapImpl * m_implementation;
};

} // MM_TTSAPI

#endif // MM_TTSAPI_SPEAKER_H
