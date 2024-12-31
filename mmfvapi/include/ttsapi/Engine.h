/*************************************************************************
*                                                                        *
*  (C) Copyright 2000-2001 Mindmaker Ltd.                                *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2000.09.29.                                                  *
*                                                                        *
*************************************************************************/

#ifdef _MSC_VER
# ifndef _CPPRTTI
#  error "Run-Time Type Information (RTTI) must be enabled!"
# endif
#endif

#ifndef MM_TTSAPI_ENGINE_H
#define MM_TTSAPI_ENGINE_H

#include <memory>

#include "DLLSpec.h"
#include "Bookmark.h"
#include "Notifier.h"
#include "Errors.h"
#include "Phoneme.h"
#include "Attribute.h"
#include "FVLanguage.h"

/** @name Engine.h
 *  A TTS Engine is used to produce speech from text.
 *  Common operations are: 
 *  \begin{itemize}
 *  \item create an engine factory (factories are used for speeding up 
 *  the initialization of engines, see \Ref{EngineFactory}),
 *  \item create a notification object (typically a \Ref{Notifier} instance) in order to receive notifications about speak-events,
 *  \item create an output site object to be used with the engine 
 *  (see \Ref{IOutputSite} for the interface; \Ref{WaveFileOutputSite} is a fully implemented class that is capable of writing output to files),
 *  \item pass the notification object to the destination object,
 *  \item create a new engine instance (see \Ref{EngineFactory} and \Ref{Engine}), 
 *  \item pass it the destination object,
 *  \item send requests to the engine, 
 *  \item manipulate the engine state, in particular the properties of the speaker
 *  \item process notification messages arriving as the engine speaks.
 *  \end{itemize}
 */
//@{

namespace MM_TTSAPI
{
	
/** @name Basic Types.
 */
//@{

/** Character code sets known by the engine - NOT YET SUPPORTED.
  * @see speakRequest
  */
enum CharCodeSet    { /// Unicode's Latin 1 page
                      CCS_LATIN1    
                      /// Unicode. 
                    , CCS_UNICODE   
                    };

/** Document units for controlling \Ref{skip} and \Ref{pause} - NOT YET SUPPORTED.
  *
  * The text passed to the TTS Engine can be thought of a "document", composed
  * of document units, organized hierarchically.
  * The \Ref{skip} method can be used to skip forward or backward within and
  * between document units. 
  * The timing of \Ref{pause} can also be controlled by declaring the document 
  * unit at which the speech should stop next.
  */
enum DocumentUnit   { /** Text given in a single \Ref{speakRequest} forms a DU_TEXT
                        * unit.
                        */ 
                      DU_TEXT      
                    , /** */ 
                      DU_SECTION
                    , /** */ 
                      DU_PARAGRAPH
                    , /** */ 
                      DU_SENTENCE
                    , /** */ 
                      DU_PHRASE
                    , /** */ 
                      DU_EXPRESSION
                    , /** */ 
                      DU_WORD
                    , /** When used in a \Ref{pause} call instructs the engine 
                        * to stop immediately.
                        */ 
                      DU_NULL_UNIT
                    };

//@}

class FLEXVOICE_DECLSPEC IOutputSite;
class FLEXVOICE_DECLSPEC Engine;
class FLEXVOICE_DECLSPEC DictionaryHandle;
class FLEXVOICE_DECLSPEC Speaker;
class FLEXVOICE_DECLSPEC SpeakerMap;

/** @name Output Formats. 
  * A TTS engine can produce either normalized text output,
  * phoneme output with prosody information, or
  * speech (wave).
  *
  * Upon construction an \Ref{IOutputSite} instance is passed to
  * the TTS Engine (see \Ref{createEngine}). In order to determine the type
  * of output expected, the TTS Engine calls
  * the \Ref{getOutputFormat} method of the passed IOutputSite instance, which in turn
  * returns an \Ref{IOutputFormat} instance. The engine then sets itself up
  * so that it will be able to support the given IOutputSite instance.
  */
//@{
/** Abstract base class for all TTS output formats.
  * Defines \Ref{OutputType} that enumerates the major output formats.
  */
class FLEXVOICE_DECLSPEC IOutputFormat
{
public:
    /// 
    enum OutputType { /** NOT YET IMPLEMENTED */ NORMALIZED_TEXT
                    , /** */ PHONEME_STRING
                    , /** */ PHONEME_STRING_WITH_PROSODY
                    , /** */ IPA_PHONEME_STRING_WITH_PROSODY
                    , /** */ WAVE
                    };

    ///
    virtual ~IOutputFormat() {};
    ///
    virtual OutputType  outputType() const = 0;
//xx    virtual bool embedNotif() const = 0;
};

/** \Ref{IOutputSite} instances having a \Ref{WaveOutputFormat} inform the engine
  * that they expect wave input from the engine.
  * This class provides detailed control on the format of the output speech.
  * For example, \Ref{WaveFileOutputSite}::getOutputFormat() returns 
  * \Ref{WaveOutputFormat}.
  */
struct FLEXVOICE_DECLSPEC WaveOutputFormat : public virtual IOutputFormat
{
    enum WaveCoding { WC_INVALID
                    , WC_PCM_SIGNED
                    , WC_PCM_UNSIGNED
                    , WC_ULAW
                    , WC_ALAW           
                    , WC_VOX            
                    , WC_VIS            
                    };
  /** Construction.
    *
    * @param sf Sampling frequency of output speech in Herz (Hz). 
    *           The output speech signal, in theory, should be defined as
    *           a function of time everywhere (in every time instance).
    *           However, digital hardware puts a limit on how frequently (how 
    *           many times per second) the signal value can be changed.
    *           This number is called the sampling frequency.
    *
    *           Standard values are 8000, 11025, 16000, 22050, 32000, 44100 
    *           (or 8kHz, 11kHz, etc.).
    *           The result of changing the sampling frequency is very audible. A smaller sampling
    *           frequency results in less clear speech. Sounds like 's','z' (fricatives,
    *           in general) will loose most of their distinctive energy when played at a sampling
    *           frequency below 16kHz, resulting in slightly distorted speech.
    *
    *           Certain wave player devices (e.g. telephony board cards) are restricted
    *           in the range of sampling frequencies that they can support.
    *           For telephony applications 8kHz is recommended (11/Nov/2000) since currently
    *           telephone devices and networks do not transmit {\em any} energy
    *           in the frequency range above 8kHz.
    *           In general, producing speech at higher sampling frequencies 
    *           consumes more CPU time. Therefore, for telephony applications it is
    *           a waste of resources to produce output with a sampling frequency
    *           above 8kHz. 
    *           
    *           A very pleasant voice can be produced at 16kHz - this is the recommended
    *           sampling frequency to be used with FlexVoice.
    *
    * @param br Bit resolution or bit depth. The number of bits representing a sample
    *           in the digitized output speech. Standard values are 8, 16.
    *           Telephony devices might require a bit resolution of 8 bits; for
    *           other applications we recommend 16 bits. The effect of sampling
    *           at 8 bits is not as audible as that of sampling at low
    *           frequencies.
    *
    *           If the bit depth is set to 8 bits, the output values are 
    *           #unsigned char#s producing values in the range #[0,256)#.
    *           The actual signal value equals #v-128#, where #w# is the value of
    *           the signal produced by the TTS engine.
    *
    *           If the bit depth is set to 16, the output value is #signed short#,
    *           i.e., a signed value and corresponds to the actual signal value.
    *
    * @param wc Wave coding. Specifies the coding of the output data
    */
    WaveOutputFormat( int sf, int br, WaveCoding wc )
                    : sampleFreq    ( sf )
                    , bitResolution ( br )
                    , waveCoding    ( wc )
    {}

    /// @return WAVE
    virtual OutputType  outputType() const { return WAVE; }

    /// sampling frequency
    int         sampleFreq;
    /// bit resolution
    int         bitResolution;
    /// wave coding
    WaveCoding  waveCoding;
};

/** \Ref{IOutputSite} instances having a \Ref{PhonemeOutputFormat} inform the engine
  * that they expect phoneme input from the engine.
  * See also \Ref{IPhonemeOutputSite}.
  */
struct FLEXVOICE_DECLSPEC PhonemeOutputFormat : public virtual IOutputFormat
{
    /// @return PHONEME_STRING
    virtual OutputType    outputType() const { return PHONEME_STRING; }
};

/** \Ref{IOutputSite} instances having a \Ref{AnnPhonemeOutputFormat} inform the engine
  * that they expect phoneme input from the engine.
  * See also \Ref{IAnnPhonemeOutputSite}.
  */
struct FLEXVOICE_DECLSPEC AnnPhonemeOutputFormat : public virtual IOutputFormat
{
    /// @return PHONEME_STRING
    virtual OutputType    outputType() const { return PHONEME_STRING_WITH_PROSODY; }
};

/** \Ref{IOutputSite} instances having a \Ref{AnnIPAPhonemeOutputFormat} inform the engine
  * that they expect phoneme input from the engine.
  * See also \Ref{IAnnIPAPhonemeOutputSite}.
  */
struct FLEXVOICE_DECLSPEC AnnIPAPhonemeOutputFormat : public virtual IOutputFormat
{
    /// @return PHONEME_STRING
    virtual OutputType    outputType() const { return IPA_PHONEME_STRING_WITH_PROSODY; }
};

/** NOT YET IMPLEMENTED. When an \Ref{IOutputSite} instance is created using a TextOutputFormat instance,
  * the output is bound to the output of the so-called text normalization
  * process of the TTS engine.
  */
struct FLEXVOICE_DECLSPEC TextOutputFormat : public virtual IOutputFormat
{
    /// @return NORMALIZED_TEXT
    virtual OutputType    outputType() const { return NORMALIZED_TEXT; }
};

//@}

/** @name Output Sites. */
//@{

/** Interface called by the {\em TTS engine} to send out data when it becomes available.
  * The TTS engine calls the appropriate put method and/or \Ref{setBookmark} upon    
  * availability of output data. 
  * It is the responsibility of the IOutputSite to take over these and redispatch
  * them to the appropriate device or object. An IOutputSite must also implement the
  * \Ref{INotifyDispatcher} interface, i.e., it is the responsability of the
  * IOutputSite to manage \Ref{INotify} objects that are waiting for the notifications
  * to arrive. 
  *
  * An IOutputSite also has a state, defined by whether the site is ready to play
  * (forward) incoming data.
  *
  * In a typical implementation the IOutputSite communicates the data to the low-level
  * audio-device object and the audio-device object sends back notifications that the
  * IOutputSite fowards to the registered \Ref{INotify} objects.
  *
  * A full implementation of IOutputSite, albeit of a special type, is provided by
  * \Ref{IWaveOutputSite}. The #put# method is not put here, but is provided for all
  * the derived classes (e.g. \Ref{IWaveOutputSite}, \Ref{IPhonemeOutputSite}).
  * The reason for this is that the argument of put changes with the formatting of the output
  * site and we decided to avoid using polymorphic classes at this level of the interface
  * (these functions are expected to be called frequently).
  *
  */
class FLEXVOICE_DECLSPEC IOutputSite : public virtual INotifyDispatcher
                                     , public virtual IAttribute
{
public:
    /// Returns the format of the output site.
    virtual const IOutputFormat&    getOutputFormat() const       = 0;

    /** @name State control */
    //@{
    /// Pauses playing.
    virtual void        pause()                         = 0;
    /// Resumes playing.
    virtual void        play()                          = 0;
    /// Clears all internal buffers.
    virtual void        clear()                         = 0;
    //@}

    /** Called by the TTS engine to pass a bookmark to the output site.
      * See \Ref{INotifyDispatcher} on how the bookmarks get dispatched to
      * the listeners.
      *
      * Ownership of the bookmark is passed to the output site.
      *
      * The order of calls to this function and the put method determines the
      * order in which the data and the bookmarks will follow each other 
      * when sent to the output device and, in turn, to the listeners.
      *
      * The caller (the TTS Engine) will and should take care of the appropriate
      * ordering.
      *
      * This method is asynchronous: the bookmark is typically put in a queue and
      * neither the output device nor the listeners will get the bookmark immediately.
      *
      */
    virtual void        setBookmark( Bookmark* bookmark )       = 0;
    /** Called by the TTS Engine  - the IOutputSite should send out the 
      * received bookmark immediately (in a synchronous way) to the listeners
      * without ever trying to send it to the output device. 
      * Used to send \Ref{BM_STOP} notifications by the engine.
      */
    virtual void        sendBookmark( Bookmark& bookmark ) = 0;
};

/** Interface called by the {\em TTS engine} to send out {\em wave} 
  * data when it becomes available.
  *
  * An IWaveOutputSite does not necessarily require the computer
  * to have a sound output device.
  *
  * Compared to \Ref{IOutputSite}, only one method, \Ref{put} is provided.
  * This method is called by the engine when new data becomes available.
  *
  * For a detailed description of how and when the engine calls the methods
  * of IWaveOutputSite see \Ref{IOutputSite}.
  */
class FLEXVOICE_DECLSPEC IWaveOutputSite : public virtual IOutputSite
{
public:

    /** Called by the TTS engine to pass a data chunk to the output site.
      * @param buffer data, formatted according to the format supported by the output size.
      * @param size   length of the data buffer (in bytes)
	  * The method does NOT take ownership of the buffer but is expected to copy it.
	  *
	  * It is expected block until it copies the data to the
	  * output device (file or lower level sound-driver).
      */
    virtual void        put( unsigned char * buffer, unsigned int size ) = 0;
};

/** Specialization of IWaveOutputSite in order to support a fake implementation
  * of the IAttribute part.
  */
template class FLEXVOICE_DECLSPEC FakeAttribute<IWaveOutputSite>;

/** Interface called by the {\em TTS engine} to send out {\em phoneme}
  * data (in the form of phoneme codes) when it becomes available.
  */
class FLEXVOICE_DECLSPEC IPhonemeOutputSite : public virtual IOutputSite
{
public:
    /** Called by the engine upon the arrival of the next phoneme.
	  * Normally, this is a blocking call.
      * The coding of the phonemes can be set by calling the engine's
      * \Ref{setPhonemeCoder} method
      */
    virtual void            put( int phonemeCode ) = 0;
};

/** Interface called by the {\em TTS engine} to send out {\em annotated phoneme}
  * data (using phoneme codes) when it becomes available.
  */
class FLEXVOICE_DECLSPEC IAnnPhonemeOutputSite : public virtual IOutputSite
{
public:
    /** Called by the engine upon the arrival of the next phoneme.
	  * Normally, this is a blocking call.
      */
    virtual void            put( const AnnPhoneme& phoneme ) = 0;
};

/** Interface called by the {\em TTS engine} to send out {\em annotated IPA phoneme}
  * data when it becomes available.
  */
class FLEXVOICE_DECLSPEC IAnnIPAPhonemeOutputSite : public virtual IOutputSite
{
public:
    ///
    virtual void            put( const AnnIPAPhoneme& phoneme ) = 0;
};

//@} // Output Sites


/** The Speaker cache strategy determines the Speaker Data managemenet strategy, i.e.,
  * when the speaker data (which can be huge) is loaded in or is discarded from memory.
  * Note that presently only the SCS_LOAD_IMMEDIATELY_DO_NOT_DELETE strategy is implemented.
  */
enum SpeakerCacheStrategy { /** */ SCS_LOAD_IMMEDIATELY_DO_NOT_DELETE
                          , /** */ SCS_LOAD_IMMEDIATELY_DELETE_WHEN_NOT_USED    //xx NOT USED !!!
                          , /** */ SCS_LOAD_IMMEDIATELY_DELETE_TIMEOUT          //xx NOT USED !!!
                          , /** */ SCS_LOAD_WHEN_NEEDED_DO_NOT_DELETE           //xx NOT USED !!!
                          , /** */ SCS_LOAD_WHEN_NEEDED_DELETE_WHEN_NOT_USED    //xx NOT USED !!!
                          , /** */ SCS_LOAD_WHEN_NEEDED_DELETE_TIMEOUT          //xx NOT USED !!!
                          };


/** Not implemented yet!
 */
class SyncEngine;

/** Not implemented yet!
 */
class StreamEngine;

/** @name Factory. */
//@{
/** Responsible for managing \Ref{Engine} and instances, \Ref{Speaker} resources 
  * and Dictionaries (\Ref{DictionaryHandle}).
  * A factory is used to instantiate engines.
  * Also, factories manage resources, such as speaker binaries.
  * The factory must be kept alive until the last engine instance is destroyed.
  */
class FLEXVOICE_DECLSPEC EngineFactory // final class
{
public:
    /** @name Construction, destruction.
       */
    //@{
    /** Constructs an engine factory using the data at the given path.
      * If path is NULL then the constructor tries to find out the correct path.
      * First it tries to get the environment variable "FLEXVOICE_DATA". If it exists
      * the constructor will use it.. 
      * On Windows the setup program of the FlexVoice engine will place the data path
      * to the registry, so in this case the environment variable setting is not requred.
      * If the given path is incorrect or there are files that cannot be loaded, the 
      * constructor throws an exception:CannotInitializeFactory.
      *
      * @param path
      
      * @exception CannotInitializeFactory if the required resources could not be read (could not be found or are corrupted).
      */
    EngineFactory( const char* path );
    /** Destroys the factory - the behavior is undefined if the factory is destructed
	  * while there is a living \Ref{Engine} instance.
      */
    ~EngineFactory();
      //xx removed CannotDestroyFactory: destructors should never throw!!
      //xx Solution is to have a dynamic ref counted factory inside
      //xx that goes away when the last engine instance goes away.
      //xx This means that the EngineFactory is just a handle to the "true" one.

    //@}

    /** Gives the default data path.
     *  This is the path from where the factory will be initialized if the user will
     *  supply NULL in the dataPath parameter of the constructor. 
     *  It is usefull for finding the VoiceList.tvl or Default.tav without creating
     *  an EngineFactory object which is an expensive operation.
     */
    static const char * getDefaultDataPath();

    /** Gives the data path.
     *  This path is from where the factory initialized the given language.
     *  It is useful if the user gave a NULL pointer to the constructor and wants 
     *  to get the path in order to find the VoiceList.tvl or the Default.tav files. 
     *  The return value can never be NULL when the construction succeeded.
     */
    const char * getLangDataPath( Language lang );

    /** Loads the resources for the given language.
     *  This call is required for creating an Engine with the given language.
     *  @throws If the resources for the given language is not available in the 
     *          path that was specified in the constructor it throws an 
     *          exception:CannotLoadLanguage.
     */
    void loadLanguage( Language lang );

    /** Unloads the given language.
      * Before this call all the engine created with this language should be
      * destroyed. 
      */
    void unloadLanguage( Language lang );

    /** Creates an engine instance that will use the given output site and the given speaker.
      * Engines must be instantiated by calling \Ref{create}.
      *
      * Engines can be destroyed normally, using the delete operator -
      * the delete operator of engines is overriden to call the factory's 
      * (protected) \Ref{destroy} function. This ensures the proper counting of engine
      * instances.
      *
      * Also, in Windows the delete operator associated with the DLL containing the 
      * implementation of create is called. This is only important when the user
      * compiles a debug version of the code.
      *
      * Ownership of the output site is NOT transferred to the constructed engine instance.
      * The preferred wave format of the given output site is used as the output format.
      * 
      * @exception OutputFormatNotSupported
      * @exception CannotCreateEngine
      */
    ::std::auto_ptr<Engine> createEngine( IOutputSite* output, const Speaker& speaker, Language lang );

    /** Not implemented yet!
     */
    ::std::auto_ptr<SyncEngine> createSyncEngine( IOutputSite* output, const Speaker& speaker, Language lang );
    
    /** Not implemented yet!
     */
    ::std::auto_ptr<StreamEngine> createStreamEngine( IOutputSite* output, const Speaker& speaker, Language lang );

    /** Adds a speaker to the factory.
     * The speaker definition is loaded in the resource pool managed by this
     * factory.
     *
     * The method may execute longer depending on the properties of the speaker
     * and the state of the factory: loading a proprietary speaker for the first
     * time may take a few seconds.
     * This method can be used to preload speaker resources before the
     * construction of any engine instance - useful e.g. in a service when engine
     * construction times are critical.
     *
     * @param lang            the given language.
     * @param speaker         speaker to be added.
     * @param cacheStrategy   cache strategy used with the given speaker
     * @return                unique id identifying the speaker
     */
    //xx cacheStrategy is NOT USED !!!
    long    addSpeaker( Language lang, const Speaker& speaker, SpeakerCacheStrategy cacheStrategy );
    //xx The resources that are needed by the speaker are reference-counted and their lifetime
    //xx are managed by the factory.

    /** Removes the speaker identified by its ID.
      * The actual timing of the removal of associated resources from memory
      * depends on the caching strategy associated with the speaker.
      * Also, if the speaker resources are in use by some engine then they
      * are not freed.
      * @param id
      */
    void    removeSpeaker( long id );

    /** Creates a new dictionary and returns with a handle of it.
      * The returned handle will be valid until the dictionary is removed.
      */
    DictionaryHandle        createDictionary( Language lang );
    /** Removes and deletes a dictionary referred by the given handle.
      * After removing a dictionary, the handles referring to this dictionary shouldn't
      * be used.
      */
    void                    removeDictionary( DictionaryHandle handle);

protected:
    // /* Engine destruction - called by the delete operator of engine instances.
    // */
    void destroy( Engine* engine );

private:
    /// engine mustn't be copied
                    EngineFactory( const EngineFactory& from );
    /// engine mustn't be copied
    EngineFactory&  operator=( const EngineFactory& from );

protected:
    class EngineFactoryImpl * m_implementation;

    friend class Engine;
    friend class EngineImpl;
    friend class EngineFactoryImpl;
};

//@}


/** @name Engine. */
//@{
/**
*/
class FLEXVOICE_DECLSPEC Engine
{
public:
    /** @name Speak requests.
      * Requests are queued up and are processed in their order of
      * appearance.
      * They can be cancelled by either calling to \Ref{stop} or
      * \Ref{cancel}.
      *
      * The input text may contain so-called embedded commands.
      */
    //@{
    /** Adds a text fragment to the next request.
      * The request {\em must} be closed by calling \Ref{speakRequest}.
      * The engine copies the text.
      */
    void        addFragment( const char * text );

    /** Adds a user bookmark.
      * Ownership of the user bookmark is transferred to the engine.
      * The bookmarks are let through the system and will be sent
      * back to the client if an \Ref{INotify} instance is passed
      * to the \Ref{IOutputSite} given to this engine.
      *
      * @see speakRequest, INotify, IOutputSite
      */
    void        addBookmark( Bookmark * bookmark );
    //xx: TODO: add description of when will come the notification

    /** Closes the request that was given by fragments.
	  * repeatCount determines the number of repetitions of the requests.
	  * if this parameter is zero the text will be repeated infinitly
      */
    void        speakRequest( int repeatCount );
    /** Passes the given text to the engine for processing.
      * The engine copies the text.
	  * repeatCount determines the number of repetitions of the requests.
	  * if this parameter is zero the text will be repeated infinitly
      */
    void        speakRequest(const char * text, int repeatCount=1);
    /** Cancels all the pending requests, including the current one.
      */
    void        stop        ();
    /** NOT YET IMPLEMENTED - Cancels the first 'requests' pending requests, including the current one.
      */
    void        cancel      ( int requests = 1 );
    /** Blocks the calling thread until all the requests are processed. 
      */
    void        wait        ();
    //@}

    /** @name State control.
      * The state of the engine is identified with the state of an imagined
      * pause button: it can be `paused', or `resumed' (pause on/off).
      * The methods below provide a means of transfer in between these states.
      * The initial state of the engine is `resumed'.
      */
    //@{

    /** Pauses the playing of the current request.
      *
      *  First, calls pause on the output site associated with this Engine instance
      *  and then pauses the processing of the current request. As a result, the
      *  output should stop playing immediately after calling this function.
      *  Changes the state to `paused'.
      *
      *  The pause happens at the first items document unit.
      *  If e.g. items = 0, and documentUnit = WORD, the pause happens at
      *  the end of the currently spoken word. Negative values are clipped at zero.
      *  documentUnit = NULL_UNIT instructs the engine to stop immediately (ASAP).
      *
      *  documentUnit and items are NOT USED.
      *
      */
    void        pause       ( DocumentUnit documentUnit = DU_NULL_UNIT , int items = 0 );

    /** Resumes play.
      *  First calls resume on the output site associated with this Engine instance
      *  and then resumes the processing of the current request. As a result the
      *  output should start playing immediately after calling this function.
      *  Changes the state to `resumed'.
      */
    void        resume      ();

    //@}


    /** Skips a number of units with the request under play.
      * Skip is a synchronous call. First, it
      * stores the current state of the engine, then puts it in the paused state.
      * Then it executes the skip and, finally, it resumes the stored
      * playing state of the engine (e.g. if the state was paused before, the
      * engine state remains paused after the call, as well).
      *
      * Document units are ordered. A unit is called larger than another one if
      * its range over the text is larger (e.g. a section unit is larger than
      * a sentence unit).
      * Requests are units themselves.
      *
      * A skip command issued for a given unit will work only within the smallest enclosing unit
      * of the current speech cursor (whose position is the position of the last bookmark
      * for which the engine received a "played" notification from the output site).
      * Thus, if skip( sen, -3 ) is issued when the speech cursor is in the middle of the second
      * sentence of a paragraph, then the speech cursor is put to the beginning of the paragraph,
      * exactly on the unit boundaries. The call will return by -2.
      * Now, another skip( sen, -1 ) accomplishes the original call of skipping 3 sentences backwards
      * provided that there are enough sentences in the previous (paragraph) unit.
      *
      * The purpose of limiting the range of skip is to avoid skipping a large number of units.
      * Users are rather suggested to use document structure markers and walking over the document
      * structure. The rational of this is that skip is an expensive operation, at least at the level
      * of sentences.
      *
      * @param unit  type of units to be skipped.
      * @param items determines the number of items to skip; if items=0, restarts reading the current unit,
      *              if item>0 skips forward, and if item<0 it skips backward the corresponding number of units
      * @return number of items actually skipped.
      *
      * Currently documentUnit must be DU_SENTENCE or DU_WORD
	  */
    int         skip        (DocumentUnit unit, int items );
    //xx: Use SkipDocumentUnit instead of DocumentUnit

    /** @name Speaker access methods.
      *
      * The methods below access the parameters of the current speaking
      * voice (or speaker), as well as the list of speakers available in the engine to be used
      * e.g. in embedded commands.
      *
      * Note that the lifetime of "speaker" resources is managed globally by the factory, but
      * for convenience, we allow the user to manage speaker definitions through engine
      * instances.
      *
      * A change to a speaker used in an engine instance with a given name will not affect
      * speakers with the same name of other engine instances.
      * In other words, speaker names are used only engine-wide.
      */
    //@{

    /** Copies the given speaker map to the engine's internal map
      * The engine can use only those speakers that are in this map (except the default speaker)
      * Currently cacheStrategy is NOT USED.
      */
    void        setSpeakers ( const SpeakerMap& speakerMap, SpeakerCacheStrategy cacheStrategy );
    /** Adds a speaker to the engine's internal speaker map.
      * @param speakerName
      * @param speaker
      * @param cacheStrategy
      * Currently cacheStrategy is NOT USED.
      */
    void        addSpeaker  ( const char *   speakerName
                            , const Speaker& speaker
                            , SpeakerCacheStrategy cacheStrategy );
    /** Removes the speaker from the engine's internal speaker map.
      * @param speakerName
      * LIMITATIONS: There is a known issue with function.
      */
    void        removeSpeaker( const char * speakerName );

    /** Updates the changes of the speaker in the engine's internal speaker map.
      * @param speakerName
      * @param speaker
      */
    void        updateSpeaker( const char * speakerName, const Speaker& speaker );

    /** Sets the current speaker to the one provided in the argument.
      *
      * If the engine is speaking, the changes will take place as
      * soon as it is possible.
      * It is guaranteed that the changes will take place all at once.
      * The actual position of change will depend on what parameters
      * are changed - the ``smaller'' the changes the sooner they
      * will takes place.
      * E.g., the changes will take place almost immediately if
      * the value of the baseline pitch is changed, but will be slightly delayed if
      * the intonation model is changed.
      *
      * If setSpeaker is called when the engine is in the `paused' state
      * then all changes will already be in effect upon the next call to \Ref{resume}.
      * However, this also means that resuming the engine in this case might take
      * somewhat longer than it would normally take.
      *
      * The actual place of change is version-dependent and is described
      * separately in an accompanying document.
      *
      * Note that the size of the buffers of the output sites might cause
      * some delay, as well.
	  *
      * LIMITATIONS:
      * If the engine is about to finish speaking it does not work as expected.
      * Pause mode is not yet implemented.
      */
    void        setSpeaker   (const Speaker& speaker, bool now);
    //xx TODO: prepare accompanying document.

    /** Retrieves a reference to the current speaker.
      */
    const Speaker&  getSpeaker () const;

    /** Retrieves a reference to the engine's internal speaker map.
      */
    const SpeakerMap& getSpeakers() const;
    //@}

    /** @name Attribute handling methods.
      * Gives an IAttribute interface reference of the engine.
      * Through this interface user can set some engine-specific parameters using
      * the set/get functions \Ref{IAttribute}. These parameter-settings are independent
      * from the speaker and also from the embedded command settings
      * Currently the following parameters 
      * are available:
      *
      * - "speechRate":
      *   Specifies the speaking rate (speed) of the engine.
      *   The default value is 1.0. Larger values mean faster speech, smaller values 
      *   mean slower speech. Scale is roughly linear.
      *
      * - "volume"
      *   Specifies the volume of the engine.
      *   The default value is 1.0 that represents the speaker's default volume.
      *   If the value is smaller than 1.0 the speech will be softer, if it is greater
      *   the speech will be lauder.
      */
    //@{
    IAttribute&         attribute();
    /** The constant version of \Ref{attribute}
      */
    const IAttribute&   attribute() const;
    //@}

    /** @name Dictionary handler methods.
      */
    //@{
    
    /** Inserts a dictionary to the engine's list.
      * The dictionary defined by the handle is stored in the \Ref{EngineFactory} and
      * the engine refers to it. Any changes in the dictionary after inserting it into
      * the engine's list will take its effect on the speech.
      * The dictionary mustn't be removed from the factory while the engine uses it!
      * The priority of a dictionary is higher if it has greater index.
      * The dictionary will be inserted before the dictionary defined by "before" parameter
      * If the "before" parameter is zero the dictionary will be placed at the end of
      * the list which means the highest priority
      */
    void                    insertDictionary    ( const char *      name
                                                , DictionaryHandle& dicthandle
                                                , const char *      before = 0
                                                );
    /** Replaces the dictionary.
      * This method is equivalent with proper remove/insert call. It helps to 
      * deal with the dictionary order.
      */
    void                    replaceDictionary   ( const char *      name
                                                , DictionaryHandle& dicthandle
                                                );
    /** Removes a dictionary from the engine's list.
      * The dictionary will remain in the factory but the engine will not use it further.
      */
    void                    removeDictionary    ( const char *      name  );
    /** Enables/disables a dictionary
      * If the "enable" parameter is true the dictionary will be enabled otherwise it will
      * be disabled. The engine uses only the enabled dictionaries.
      */ 
    void                    enableDictionary    ( const char *      name
                                                , bool              enable=true
                                                );
    /** Gives if the dictonary is enabled or not
      */
    bool                    isDictionaryEnabled ( const char *      name  ) const;
    /** Gives the engine's dictionary-list size.
      * The indexes of the dictionaries are between 0 and this value.
      */
    int                     getDictCount        () const;
                            
    /** Gives the index of a dictionary defined by "name"
      * Return with -1 if the given name is not in the list.
      */
    int                     getDictionaryIndex  ( const char *      name  ) const;
    /** Gives the name of a dictionary defined by "name"
      * throws an \Ref{InvalidParamException} exception if the index is out of range
      */
    const char *            getDictionaryName   ( int               index ) const;

    /** Gives a handle of a dictionary defined by "name"
      * If the given name doesn't exists the returned handle will be invalid
      */ 
    DictionaryHandle        getDictionary       ( const char *      name  ) const;
    /** Gives a handle of a dictionary defined by "index"
      * throws an \Ref{InvalidParamException} exception if the index is out of range
      */ 
    DictionaryHandle        getDictionary       ( int               index ) const;
    //@}

    /** Sets the outer phoneme coder to code the phoneme notification.
      * If the client doesn't call this function, or calls it with NULL, the phoneme
      * notification will be coded by the standard inner coder of the FlexVoice, otherwise
      * the engine will code the phonemes with the given coder.
      * The engine takes the ownership of the coder and the client mustn't use the given coder
      * any more.
      * With this method the client can make his or her own phoneme coding so it is
      * unnecessary to use the FlexVoice's standard phoneme table. For more information please
      * read the documentation of \Ref{IPhonemeCoder}
      * Since the new and the delete for an object cannot be at different sides
      * of the dll boundary (to avoid asserts in debug mode), the coder must provide
      * a destroy function that is called by the engine (the implementation
      * must be "delete this").
      */
    void        setPhonemeCoder( IPhonemeCoder* coder );

    /** Gives the PhonemeCoder used by the Engine
      * If the PhonemeCoder was set by the \Ref{setPhonemeCoder} method this 
      * function will return with this value otherwise it will return with the 
      * default coder
      * @return Returns with a constant reference to the internally used PhonemeCoder
      *         This value cannot be used directly it should be cloned if needed.
      */
    const IPhonemeCoder&    getPhonemeCoder() const;

    /** Puts the engine in its initial phase and sets the current speaker to the default.
      * In particular, puts the engine in the `resumed' state, clears all internal buffers,
      * as well as the requests.
      */
    void        reset       ();

    /** Destructor
    */
    ~Engine();

    /** Deletes the engine.
      * Calls the factory's (protected) \Ref{destroy} function.
      * This ensures the proper management of engine instances.
      */
    static void operator delete( void *p );
    //xx size_t??

private:
                        Engine( class EngineFactoryImpl*  factoryImpl
                              , IOutputSite*              output
                              , const Speaker&            speaker
                              , Language                  lang );

                        
                        /// Engine shouldn't be copied
                        Engine      ( const Engine& from );
                        /// Engine shouldn't be copied
    Engine&             operator=   ( const Engine& from );

    class EngineImpl*   m_implementation;

    friend class    EngineImpl;
    friend class    EngineFactory;
    friend class    EngineFactoryImpl;
};
//@}

} // namespace MM_TTSAPI


//@}

#endif // MM_TTSAPI_ENGINE_H

