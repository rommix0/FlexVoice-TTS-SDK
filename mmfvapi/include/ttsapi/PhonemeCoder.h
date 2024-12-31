/*************************************************************************
*                                                                        *
*  (C) Copyright 2000-2001 Mindmaker Ltd.                                *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2001.02.02.                                                  *
*                                                                        *
*************************************************************************/

#ifndef MM_TTSAPI_PHONEME_CODER_H
#define MM_TTSAPI_PHONEME_CODER_H

#include "DLLSpec.h"
#include "Phoneme.h"

/**@name PhonemeCoder.h */
//@{

namespace MM_TTSAPI
{

/** \Ref{IPhonemeCoder} implementation for converting phoneme codes using phonemes described in a file
*/
class FLEXVOICE_DECLSPEC FilePhonemeCoder : public virtual IPhonemeCoder
{
public:
	///
                    FilePhonemeCoder( const char * filename = NULL );
                    FilePhonemeCoder( const FilePhonemeCoder& from );
	///
                    ~FilePhonemeCoder();

    bool            read( const char * filename );

// IPhonemeCoder
    ///
    virtual IPhonemeCoder*  clone       () const;
    ///
    virtual void            destroy     ();
	///
    virtual int             phonemeCount()              const;
	///
    virtual int             phonemeCode ( int index )   const;
	///
    virtual const char *    sampleWord  ( int code )    const;

	///
    virtual const char*     getSymbol   ( int code );
	///
    virtual int             parseString ( const char* symbolstr, int& code );

	///
    virtual void            reset       ();

	///
    virtual int             putIPA      ( const IPAPhoneme& ipa );
	///
    virtual int             flushIPA    ();
	///
    virtual int             getCode     ( int& code );

	///
    virtual int             putCode     ( int code );
	///
    virtual int             flushCode   ();
	///
    virtual int             getIPA      ( IPAPhoneme& ipa );

    ///
    virtual long            getCoderID    ()            const;
    ///
    virtual int             getInvalidCode()            const;
    ///
    virtual bool            isSpecialCode ( int code  ) const;
    ///
    virtual int             getStressCode ( IPAPhoneme::StressState stressState ) const;
    ///
    virtual int             getBoundaryCode ( IPAPhoneme::BoundaryType boundaryType ) const;
    ///
    virtual void            setGeneratedIPAForm( const GeneratedIPAForm& generatedForm );

private:
    
    FilePhonemeCoder& operator=( const FilePhonemeCoder& from );

    class FilePhonemeCoderImpl*   m_implementation;
};

} // namespace MM_TTSAPI

//@} // PhonemeCoder.h

#endif // MM_TTSAPI_PHONEME_CODER_H
