/*************************************************************************
*                                                                        *
*  (C) Copyright 2000-2001 Mindmaker Ltd.                                *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2000.10.13.                                                  *
*                                                                        *
*************************************************************************/

#ifndef MM_BOOKMARK_H
#define MM_BOOKMARK_H
#include <bitset>
#include "Phoneme.h"
#include "Speaker.h"

/**@name Bookmark.h */
//@{

namespace MM_TTSAPI
{

/**@name Bookmark types, typelists */
//@{	

/// List of valid bookmark types.
enum BookmarkType   { /** */ BM_INVALID
                    , /** */ BM_TEXT_BEGIN
                    , /** */ BM_TEXT_END
                    , /** */ BM_SECTION_BEGIN
                    , /** */ BM_SECTION_END
                    , /** */ BM_PARAGRAPH_BEGIN
                    , /** */ BM_PARAGRAPH_END
                    , /** */ BM_SENTENCE_BEGIN
                    , /** */ BM_SENTENCE_END
                    , /** */ BM_ITEM_BEGIN
                    , /** */ BM_ITEM_END
                    , /** */ BM_WORD_BEGIN
                    , /** */ BM_WORD_END
                    , /** */ BM_PHONEME_BEGIN
                    , /** */ BM_PHONEME_END
                    , /** */ BM_IPAPHONEME_BEGIN
                    , /** */ BM_IPAPHONEME_END
                    , /** */ BM_SPEAKERCHANGED
                    , /** */ BM_PARAMCHANGED
                    , /** */ BM_USER
                    , /** */ BM_EMBEDDED
                    , /** */ BM_PAUSE
                    , /** */ BM_RESUME
                    , /** */ BM_START
                    , /** */ BM_STOP
                    , /** */ BM_REPEAT
                    // and so on
                    , /** */ BM_NO_OF_BOOKMARKS 
                    };

/** A BookmarkTypeList provides a comfortable interface to be used with 
  * \Ref{INotifyDispatcher} methods,
  * namely with \Ref{registerNotify}, \Ref{addBookmarkTypes} and \Ref{removeBookmarkTypes}.
  * Note that you can call e.g. \Ref{registerNotify} with the syntax #registerNotify( aNotifyObject, BM_STOP | BM_PAUSE )#.
  * 
  * See also \Ref{operator|}.
  */
class BookmarkTypeList : public std::bitset<BM_NO_OF_BOOKMARKS>
{
public:
///
    BookmarkTypeList() {}
///
    BookmarkTypeList( const BookmarkType& type ) { set( type ); }
};

/// 
inline BookmarkTypeList  operator|( const BookmarkType& first, const BookmarkType& second )
{
    BookmarkTypeList typelist;
    typelist.set( first );
    typelist.set( second );
    return typelist;
}

///
inline BookmarkTypeList operator|( const BookmarkTypeList& first, const BookmarkType& second )
{
    BookmarkTypeList typelist(first);
    typelist.set( second );
    return typelist;
}

///
inline BookmarkTypeList operator|( const BookmarkTypeList& first, const BookmarkTypeList& second )
{
    BookmarkTypeList typelist(first);
    typelist |= second;
    return typelist;
}
//@}

/** @name Bookmarks.  */
//@{

/** Bookmarks are passed along to the \Ref{IOutputSite} by the \Ref{Engine} 
  * which forwards them to \Ref{INotify} instances.
  * Bookmarks can be used to be notified about certain events - see \Ref{BookmakrType}.
  */
class Bookmark
{
public:
    ///
    Bookmark() : type       ( BM_INVALID )
               , pos        ( 0 )
               , time_sec   ( 0 )
               , time_ms    ( 0 )
               , len        ( 0 )
               , dur        ( 0 )
               , id         ( 0 )
    {}

    ///
    virtual ~Bookmark() {}

    ///
    virtual Bookmark* clone() const
    {
        return new Bookmark( *this );
    }

    ///
    BookmarkType    type;
    ///
    int             pos;
    /** Time when the bookmark was processed by the last output device.
	 *  This is the absolute time in seconds since a specified "zero" time.
	 *  The "zero" time is OS dependent.
     */
    unsigned long   time_sec; 
    /** Time when the bookmark was processed by the last output device.
	  * Fraction of the second in milliseconds.
	  */ 
    unsigned short  time_ms;
    /// Character position from the beginning of the unnormalized input text.
    int             len;    // length in character position; refine xx
    /// Duration of the corresponding wave chunk in milliseconds - if any.
    int             dur;    // duration in milliseconds
    /// Bookmark id, used internally.
    unsigned long   id;
};

/// Base class of bookmarks that are embedded into the output flow of the engine.
class BookmarkEmbedded : public Bookmark
{
public:
///
    BookmarkEmbedded() : userData( 0 ) {}

    virtual Bookmark* clone() const
    {
        return new BookmarkEmbedded( *this );
    }

///
    int userData;
};

/// NOT YET USED
class BookmarkDocumentStructure : public Bookmark
{
public:
    BookmarkDocumentStructure() : level( 0 ) {}

    virtual Bookmark* clone() const
    {
        return new BookmarkDocumentStructure( *this );
    }
    ///
    int level;
};

/// Arrives when phoneme playing starts (but see also \Ref{prepare}).
class BookmarkPhoneme : public Bookmark
{
public:
    BookmarkPhoneme() : phonemeCode     ( -1 )
                      , nextPhonemeCode ( -1 )
    {}

    virtual Bookmark* clone() const
    {
        return new BookmarkPhoneme( *this );
    }
    ///
    int phonemeCode;
    int nextPhonemeCode;
};

/// 
class BookmarkIPA : public Bookmark
{
public:
    virtual Bookmark* clone() const
    {
        return new BookmarkIPA( *this );
    }
    ///
    IPAPhoneme      ipa;
    IPAPhoneme      nextipa;
};

/// 
class BookmarkSpeakerChanged : public Bookmark
{
public:

    BookmarkSpeakerChanged() : Bookmark()
    {}

    virtual Bookmark* clone() const
    {
        return new BookmarkSpeakerChanged( *this );
    }
    ///
    Speaker     speaker;
};

/// 
class BookmarkParamChanged : public Bookmark
{
public:
    enum    ChangeType { UNKNOWN
                       , EXTERNAL
                       , EMBEDDED
                       };

    enum    ValueType  { NO_VALUE
                       , BOOL_VALUE
                       , CHAR_VALUE
                       , INT_VALUE
                       , DBL_VALUE
                       , STR_VALUE
                       };

    union Value
    {
        bool                valueBool;
        char                valueChar;
        int                 valueInt;
        double              valueDbl;
        const char*         valueStr;
    };

    virtual Bookmark*   clone() const
    {
        return new BookmarkParamChanged( *this );
    }

    ChangeType          changeType;
    ValueType           valueType;
    
    const char*         name;
    Value               value;
};

//@}

//@} // Bookmark.h

}
 

#endif // MM_BOOKMARK_H
