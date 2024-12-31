/*************************************************************************
*                                                                        *
*  (C) Copyright 2000-2001 Mindmaker Ltd.                                *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2000.11.23.                                                  *
*                                                                        *
*************************************************************************/

#ifndef MM_TTSAPI_PHONEME_H
#define MM_TTSAPI_PHONEME_H

#include "DLLSpec.h"
#include <bitset>
#include <vector>

/**@name Phoneme.h */
//@{

namespace MM_TTSAPI
{

/** Describes phonemes by their features, essentially providing
  * a language-independent representation of phonemes.
    Features follow the naming system of the International Phonetic Association (IPA)
    as described in the Handbook of IPA (Cambridge Univ. Press 1999) 
    and on the IPA website at http://www2.arts.gla.ac.uk/IPA/ipa.html
  */
class IPAPhoneme
{
public:
    ///
    enum PhonemeType    { /** */ PTY_NOT_SPECIFIED
                        , /** */ PTY_SILENT
                        , /** */ PTY_CONSONANT
                        , /** */ PTY_VOWEL
                        };
    ///
    enum ConsonantType  { /** invalid consonant type  */ CTY_NOT_SPECIFIED     
                        , /** plosive ( p, t, k, )    */ CTY_PLOSIVE           
                        , /** plosive closure         */ CTY_PLOSIVE_CLOSURE
                        , /** plosive release         */ CTY_PLOSIVE_RELEASE
                        , /** plosive aspiration      */ CTY_PLOSIVE_ASPIRATION   
                        , /** implosive               */ CTY_IMPLOSIVE         
                        , /** affricate               */ CTY_AFFRICATE         
                        , /** affricate closure       */ CTY_AFFRICATE_CLOSURE
                        , /** affricate release       */ CTY_AFFRICATE_RELEASE 
                        , /** nasal   ( n, m )        */ CTY_NASAL             
                        , /** trill   ( hungarian r ) */ CTY_TRILL             
                        , /** trill-fricative ( czech r^ ) */ CTY_TRILL_FRICATIVE             
                        , /** taporflap               */ CTY_TAPORFLAP         
                        , /** fricative   ( f, s, )   */ CTY_FRICATIVE         
                        , /** lateral fricative       */ CTY_LATERAL_FRIC      
                        , /** approximant             */ CTY_APPROXIMANT       
                        , /** Catalan (j)             */ CTY_CENTRAL_APROXIMANT
                        , /** lateral approximant     */ CTY_LATERAL_APPROX    
                        , /** ejective                */ CTY_EJECTIVE          
                        , /** click                   */ CTY_CLICK             
                        , /**                         */ CTY_COUNT
                        };
    ///
    enum ConsonantPos   { /**                 */ CPS_NOT_SPECIFIED     
                        , /** bilabial        */ CPS_BILABIAL          
                        , /** labiodental     */ CPS_LABIODENTAL       
                        , /** dental          */ CPS_DENTAL            
                        , /** alveolar        */ CPS_ALVEOLAR          
                        , /** postalveolar    */ CPS_POSTALVEOLAR      
                        , /** retroflex       */ CPS_RETROFLEX         
                        , /** palatal         */ CPS_PALATAL           
                        , /** palato-alveolar */ CPS_PALATOALVEOLAR    
                        , /** velar           */ CPS_VELAR             
                        , /** uvular          */ CPS_UVULAR            
                        , /** pharyngeal      */ CPS_PHARYNGEAL
                        , /** glottal         */ CPS_GLOTTAL
                        , /** epiglottal      */ CPS_EPIGLOTTAL        
                        , /** labial velar    */ CPS_LABIAL_VELAR      
                        , /** labial-palatal  */ CPS_LABIAL_PALATAL    
                        , /** postalveolar-velar*/ CPS_POSTALVEOLAR_VELAR
                        , /**                 */ CPS_COUNT
                        };
    
    ///
    enum VoicingState   { /** */ VST_NOT_SPECIFIED
                        , /** */ VST_UNVOICED
                        , /** */ VST_VOICED
                        };
    ///
    enum VowelMouthPos  { VMP_NOT_SPECIFIED
                        , /** mouth is closed                                         */ VMP_CLOSE             
                        , /** mouth is nearly closed                                  */ VMP_CLOSE_NEAR        
                        , /** mouth is a little bit more closed than the middle level */ VMP_CLOSE_MID
                        , /** mouth is middle level opened                            */ VMP_MIDDLE            
                        , /** mouth is a little bit more opened than the middle level */ VMP_OPEN_MID          
                        , /** mouth is nearly opened                                  */ VMP_OPEN_NEAR
                        , /** mouth is opened                                         */ VMP_OPEN              
                        };
    ///
    enum VowelTonguePos { /** */ VTP_NOT_SPECIFIED
                        , /** */ VTP_FRONT
                        , /** */ VTP_FRONT_CENTRAL
                        , /** */ VTP_CENTRAL
                        , /** */ VTP_BACK_CENTRAL
                        , /** */ VTP_BACK
                        };
    ///                    
    enum VowelLipState  { /** */ LST_NOT_SPECIFIED
                        , /** */ LST_NOT_ROUNDED
                        , /** */ LST_ROUNDED
                        };
    ///
    enum Length         { /** */ LNG_NOT_SPECIFIED
                        , /** */ LNG_EXTRA_SHORT
                        , /** */ LNG_SHORT
                        , /** */ LNG_HALF_LONG
                        , /** */ LNG_LONG
                        };

    ///
    enum ToneStyle      { /** */ TST_NOT_SPECIFIED
                        , /** */ TST_EXTRA_HIGH
                        , /** */ TST_HIGH
                        , /** */ TST_MID
                        , /** */ TST_LOW
                        , /** */ TST_EXTRA_LOW
                        , /** */ TST_DOWNSTEP
                        , /** */ TST_UPSTEP
                        , /** */ TST_RISING
                        , /** */ TST_FALLING
                        , /** */ TST_HIGH_RISING
                        , /** */ TST_LOW_RISING
                        , /** */ TST_RISING_FALLING
                        , /** */ TST_GLOBAL_RISE
                        , /** */ TST_GLOBAL_FALL
                        };
    ///
    enum StressState    { /** */ SST_NOT_SPECIFIED
                        , /** */ SST_NO_STRESS
                        , /** */ SST_PRIMARY_STRESS
                        , /** */ SST_SECONDARY_STRESS
                        };

    ///
    enum BoundaryType   { /** */ BTY_PHONEME_INITIAL
                        , /** */ BTY_PHONEME_FINAL
                        , /** */ BTY_SYLLABLE_INITIAL
                        , /** */ BTY_SYLLABLE_FINAL
                        , /** */ BTY_WORD_INITIAL
                        , /** */ BTY_WORD_FINAL
                        , /** */ BTY_HYPHENATEDWORD_INITIAL
                        , /** */ BTY_HYPHENATEDWORD_FINAL
                        , /** */ BTY_MINORGROUP_INITIAL
                        , /** */ BTY_MINORGROUP_FINAL
                        , /** */ BTY_MAJORGROUP_INITIAL
                        , /** */ BTY_MAJORGROUP_FINAL
                        , /** */ BTY_COUNT
                        };

    ///
    enum PhonemeAttribute   { /** */ PAT_VOICELESS
                            , /** */ PAT_VOICED
                            , /** */ PAT_ASPIRATED
                            , /** */ PAT_MORE_ROUNDED
                            , /** */ PAT_LESS_ROUNDED
                            , /** */ PAT_ADVANCED
                            , /** */ PAT_RETRACTED
                            , /** */ PAT_CENTRALIZED
                            , /** */ PAT_MID_CENTRALIZED
                            , /** */ PAT_SYLLABIC
                            , /** */ PAT_NON_SYLLABIC
                            , /** */ PAT_RHOTICITY
                            , /** */ PAT_BREATHY_VOICED
                            , /** */ PAT_CREAKY_VOICED
                            , /** */ PAT_LINGUOLABIAL
                            , /** */ PAT_LABIALIZED
                            , /** */ PAT_PALATALIZED
                            , /** */ PAT_VELARIZED
                            , /** */ PAT_PHARYNGEALIZED
                            , /** */ PAT_RAISED
                            , /** */ PAT_LOWERED
                            , /** */ PAT_ADVANCED_TONGUE_ROOT
                            , /** */ PAT_RETRACTED_TONGUE_ROOT
                            , /** */ PAT_DENTAL
                            , /** */ PAT_APICAL
                            , /** */ PAT_LAMINAL
                            , /** */ PAT_NASALIZED
                            , /** */ PAT_NASAL_RELEASE
                            , /** */ PAT_PRENASALIZED
                            , /** */ PAT_LATERAL_RELEASE
                            , /** */ PAT_NO_AUDIBLE_RELEASE
                            , /** */ PAT_NON_PULMONIC
                            , /** */ PAT_LINKING
                            , /** */ PAT_COUNT_OF_ATTRIBUTES
                            };


public:
///
                        IPAPhoneme()                                { clear(); }
///
inline          bool    operator==  ( const IPAPhoneme& )   const;
///
inline          void    clear       ();
///
inline          bool    isVowel     ()                      const;
///
inline          bool    isSyllabic  ()                      const;
///
inline          bool    isConsonant ()                      const;
///
inline          bool    isSilent    ()                      const;
/// Returns voiced consonants as well as vowels.
inline          bool    isVoiced    ()                      const;
/// Returns sonorant consonants as well as vowels.
inline          bool    isSonorant  ()                      const;

public:
    ///
    PhonemeType         m_type;
    ///
    std::bitset<PAT_COUNT_OF_ATTRIBUTES>    m_attr;
    ///
    std::bitset<BTY_COUNT>                  m_boundary;

    /** @name Consonant properties: values are valid only if the m_type is PTY_CONSONANT */
    //@{
    ///
    ConsonantType       m_consonantType;
    ///
    ConsonantPos        m_consonantPos;
    ///
    VoicingState        m_voicingState;
    //@}
    /** @name Vowel properties: values are valid only if the m_type is PTY_VOWEL */
    //@{
    ///
    VowelMouthPos       m_mouthPos;
    ///
    VowelTonguePos      m_tonguePos;
    ///
    VowelLipState       m_lipState;
    ///
    ToneStyle           m_toneStyle;
    ///
    StressState         m_stressState;
    //@}
    ///
    Length              m_length;
};

inline bool IPAPhoneme::operator==( const IPAPhoneme& right ) const
{
    return  (   m_type          == right.m_type          
            &&  m_consonantType == right.m_consonantType 
            &&  m_consonantPos  == right.m_consonantPos  
            &&  m_voicingState  == right.m_voicingState  
            &&  m_mouthPos      == right.m_mouthPos      
            &&  m_tonguePos     == right.m_tonguePos     
            &&  m_lipState      == right.m_lipState      
            &&  m_length        == right.m_length        
            &&  m_toneStyle     == right.m_toneStyle     
            &&  m_stressState   == right.m_stressState   
            &&  m_attr          == right.m_attr          
            );
}

inline void IPAPhoneme::clear()
{
    m_type          = PTY_NOT_SPECIFIED;
    m_consonantType = CTY_NOT_SPECIFIED;
    m_consonantPos  = CPS_NOT_SPECIFIED;
    m_voicingState  = VST_NOT_SPECIFIED;
    m_mouthPos      = VMP_NOT_SPECIFIED;
    m_tonguePos     = VTP_NOT_SPECIFIED;
    m_lipState      = LST_NOT_SPECIFIED;
    m_length        = LNG_NOT_SPECIFIED;
    m_toneStyle     = TST_NOT_SPECIFIED;
    m_stressState   = SST_NOT_SPECIFIED;

    m_attr.reset();
}

inline bool IPAPhoneme::isVowel()       const
{ return ( m_type==PTY_VOWEL ); }

inline bool IPAPhoneme::isSyllabic()    const
{ return ( m_type==PTY_VOWEL || m_attr[PAT_SYLLABIC] ); }

inline bool IPAPhoneme::isConsonant()   const
{ return ( m_type==PTY_CONSONANT ); }

inline bool IPAPhoneme::isSilent()      const
{ return ( m_type==PTY_SILENT ); }

inline bool IPAPhoneme::isVoiced()      const
{ return ( isVowel() || m_voicingState==VST_VOICED ); }

inline bool IPAPhoneme::isSonorant()    const
{ 
    return (    isVowel()
           ||   m_consonantType == CTY_NASAL 
           ||   m_consonantType == CTY_APPROXIMANT
           ||   m_consonantType == CTY_CENTRAL_APROXIMANT
           ||   m_consonantType == CTY_LATERAL_APPROX
           ||   m_consonantType == CTY_TRILL
           );
}


/** Interface to convert between phoneme symbol tables and the above IPA phoneme representations.
  * An API client may pass its converter to the engine - in this case the engine will assume the
  * phoneme symbol table defined by this phoneme coder.
  */
class FLEXVOICE_DECLSPEC IPhonemeCoder
{
public:
    ///
    virtual                 ~IPhonemeCoder() {}

    ///
    virtual IPhonemeCoder*  clone() const = 0;

    /** Destroys the object.
     *  The implementation of this function must call the delete function for
     *  this pointer.
     */
    virtual void            destroy() = 0;

    /// Returns the number of phoneme codes.
    virtual int             phonemeCount()              const   = 0;
    /** This method is to be used to enumerate the phonemes.
     *  Phoneme codes need not be continuous but index must.
     *  @param index is between 0 and phonemeCount()
     */
    virtual int             phonemeCode ( int index )   const   = 0;
    /// Returns a representative word for any phoneme.
    virtual const char *    sampleWord  ( int code )    const   = 0;

    /// Returns the symbolic representation of a phoneme.
    virtual const char*     getSymbol   ( int code )                         = 0;
    /** This method sequentially converts a symbol string into phoneme codes.
     *  @return the number of characters parsed.
     */
    virtual int             parseString ( const char* symbolstr, int& code ) = 0;

    /// Clears the internal state of the coder.
    virtual void            reset       ()                          = 0;

    /** Starts converting an IPAPhoneme object into phoneme code(s) and puts codes into an output queue.
     *  @return the number of codes in the queue.
     */
    virtual int             putIPA      ( const IPAPhoneme& ipa )   = 0;
    /** Finishes the conversion of IPAPhoneme objects into phoneme code(s) and puts codes into an output queue.
     *  @return the number of codes in the queue.
     */
    virtual int             flushIPA    ()                          = 0;
    /** Gets the first code in the output queue.
     *  @return the number of IPAPhoneme objects contributing to this code, or -1 if queue is empty.
     */
    virtual int             getCode     ( int& code )               = 0;

    /** Starts converting a phoneme code into IPAPhoneme object(s) and puts them into an output queue.
     *  @return the number of IPAPhonemes in the queue.
     */
    virtual int             putCode     ( int code )                = 0;
    /** Finishes the conversion of phoneme codes into IPAPhoneme object(s) and puts them into an output queue.
     *  @return the number of IPAPhonemes in the queue.
     */
    virtual int             flushCode   ()                          = 0;
    /** Gets the first IPAPhoneme object in the output queue.
     *  @return the number of phoneme codes contributing to this object, or -1 if queue is empty.
     */
    virtual int             getIPA      ( IPAPhoneme& ipa )         = 0;
    /**
     *
     */
    virtual long            getCoderID  () const = 0;
    /**
     *
     */
    virtual int             getInvalidCode() const = 0;
    /**
     *
     */
    virtual bool            isSpecialCode ( int code ) const = 0;
    /**
     *
     */
    virtual int             getStressCode ( IPAPhoneme::StressState stressState ) const = 0;

    /**
     *
     */
    virtual int             getBoundaryCode ( IPAPhoneme::BoundaryType boundaryType ) const = 0;

    /** If several IPA forms are recognized as the same code, the user may select which 
     *  one is generated.
     */
    enum GeneratedIPAForm
    {   /**                                             */ GIF_DEFAULT
    ,   /** selects the shortest IPA representation 
        *** (e.g. flagged monophthong)                  */ GIF_SHORTEST
    ,   /** selects the longest IPA representation 
        *** (e.g. diphthong)                            */ GIF_LONGEST
    };

    virtual void            setGeneratedIPAForm( const GeneratedIPAForm& generatedForm ) = 0;
};

/** A vector of timepoint-pitch pairs; 
    time is relative to the phoneme duration 
    and pitch is relative to the speaker's default pitch.
*/
typedef std::vector<std::pair<float,float> > PitchContour;

/** A vector of timepoint-loudness pairs; 
    time is relative to the phoneme duration 
    and loudness is relative to the speaker's default loudness.
*/
typedef std::vector<std::pair<float,float> > LoudnessContour;

///This struct contains a phoneme code and prosodic information.
struct AnnPhoneme
{
    /// Phoneme code.
    int             code;
    /// Duration in msec.
    int             duration;
    /** This vector contains pairs of relative locations in the phoneme [0,1] 
        and corresponding pitch values, relative to the speaker's default pitch.
    */
    PitchContour    pitchContour;
    /** This vector contains pairs of relative locations in the phoneme [0,1] 
        and corresponding loudness values, relative to the speaker's default loudness.
    */
    LoudnessContour loudnessContour;
};

///This struct contains an IPAPhoneme object and prosodic information.
struct AnnIPAPhoneme
{
    /// IPAPhoneme object
    IPAPhoneme      ipa;
    /// Duration in msec.
    int             duration;
    /** This vector contains pairs of relative locations in the phoneme [0,1] 
        and corresponding pitch values, relative to the speaker's default pitch.
    */
    PitchContour    pitchContour;
    /** This vector contains pairs of relative locations in the phoneme [0,1] 
        and corresponding loudness values, relative to the speaker's default loudness.
    */
    LoudnessContour loudnessContour;
};

} // namespace MM_TTSAPI

//@}

#endif // MM_TTSAPI_PHONEME_H
