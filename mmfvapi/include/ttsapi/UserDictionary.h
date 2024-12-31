/*************************************************************************
*                                                                        *
*  (C) Copyright 2000-2001 Mindmaker Ltd.                                *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*                                                                        *
*************************************************************************/

#ifndef MM_TTSAPI_USER_DICTIONARY_H
#define MM_TTSAPI_USER_DICTIONARY_H

#include "DLLSpec.h"
#include "Attribute.h"

/** @name UserDictionary.h
  * A \Ref{DictionaryHandle} is a list of word-pronunciation associations.
  * A \Ref{DictionaryHandle} instance can be given to the \Ref{Engine} by calling
  * \Ref{Engine}::\Ref{setDictionary}(). The engine will always look up
  * word-pronunciations first in the \Ref{DictionaryHandle} instance, providing
  * an easy way to define the pronunciation of irregular worlds not yet
  * known by the engine.
  *
  * The \Ref{DictionaryHandle} is implemented with an std::map-like interface.
  * It holds \Ref{IDictionaryItem}s that can be retrieved, replaced, etc.
  */
//@{

namespace MM_TTSAPI
{

/** A dictionary item accesses the word parameters.
 *  This interface inherits the \Ref{IAttribute} interface that makes the parameter
 *  access available. Currently the following parameters are supported:
 *  
 *  - "soundsLike"
 *      The engine tries to interpret the input string as a word of the given language,
 *      transforms it into a phoneme representation, and associates the word
 *      with the given phoneme representation. This parameter provides an easy
 *      and intuitive way to define pronunciations without any knowledge
 *      of the phoneme codes.
 *
 *  - "phonemeString"
 *      A string parameter associated with the given item that is the 
 *      symbol-string representation of a phoneme-string. 
 *      FlexVoice2 standard symbol representation is assumed.
 */
class FLEXVOICE_DECLSPEC IDictionaryItem : public virtual IAttribute
{
};

// Unfortunately this class cannot be an inner class because classes that contain
//  inner classes cannot be exported from the DLL.
/** Non-const iteration of a \Ref{DictionaryHandle} instance. */
class FLEXVOICE_DECLSPEC DictIterator
{
public:
                ///
                DictIterator();
                ///
                DictIterator( const DictIterator& from );
                
                ~DictIterator();

    /// Gives the word that the iterator belongs to.
    const char *            getKey    () const;

    ///
    const IDictionaryItem&  operator* () const;
    ///
    const IDictionaryItem*  operator->() const;
    ///
    IDictionaryItem&        operator* ();
    ///
    IDictionaryItem*        operator->();
    ///
    DictIterator&           operator= ( const DictIterator& from );
    ///
    DictIterator            operator++();
    ///
    DictIterator            operator++(int);
    ///
    bool                    operator==( const DictIterator& comp );
    ///
    bool                    operator!=( const DictIterator& comp );

private:
    class DictIteratorImpl*   m_implementation;

    friend class DictionaryHandle;
    friend class ConstDictIterator;
};

/** Const iteration of a \Ref{DictionaryHandle} instance. */
class FLEXVOICE_DECLSPEC ConstDictIterator
{
public:
                ///
                ConstDictIterator();
                ///
                ConstDictIterator( const ConstDictIterator& from );
                ///
                ConstDictIterator( const DictIterator& from );
                
                ~ConstDictIterator();

    /// Gives the word that the iterator belongs to.
    const char *            getKey    () const;

    ///
    const IDictionaryItem&  operator* () const;
    ///
    const IDictionaryItem*  operator->() const;
    ///
    ConstDictIterator&      operator= ( const ConstDictIterator& from );
    ///
    ConstDictIterator       operator++();
    ///
    ConstDictIterator       operator++(int);
    ///
    bool                    operator==( const ConstDictIterator& comp );
    ///
    bool                    operator!=( const ConstDictIterator& comp );

private:
    class DictIteratorImpl*   m_implementation;

    friend class DictionaryHandle;
};

/** This dictionary implementation is compatible with FlexVoice 1.0 implementations.
  *
  * The file format (used in \Ref{read} and \Ref{write}) is as follows:
  *
  * #
  * ({<word> [<sounds-like>] <SAMPA-phoneme-string>|";"<anytext>|} <NL>])*
  * #
  */
class FLEXVOICE_DECLSPEC DictionaryHandle
{
public:
    ///
    typedef DictIterator        iterator;
    ///
    typedef ConstDictIterator   const_iterator;

                        ///
                        DictionaryHandle ();
                        ///
                        DictionaryHandle ( const DictionaryHandle& from );
						///                 
                        ~DictionaryHandle();

    DictionaryHandle&   operator= ( const DictionaryHandle& from );

    /** True if the dictionary-handle is valid.
      * The methods can only be called if the handle is valid.
      */
    bool                isValid() const;
    /** Copies the given dictionary to this one.
      * NOT IMPLEMENTED YET
      */
	void                copy ( const DictionaryHandle& from );
    /** Reads the dictionary from a file.
      * The "compiled" status will be set depending on the read file.
      * @see compile
      */
    bool                read ( const char* fileName );
    /** Writes the dictionary to a file
      * If the dictionary was compiled the output file will be a binary file otherwise
      * it will be a readable/editable text file.
      * @see compile
      */
    bool                write( const char* fileName );
    /** Rereads the dictionary from a file
      * The "compiled" status will be set depending on the read file.
      * @see compile
      */
    bool                reRead();

    /** Compiles the dictionary.
      * A large dictionaries are suggested to compile while this process reduces the 
      * memory usage. The compilation may takes a rather long time so it shouldn't be
      * called in "on-line" applications. After the compilation the dictionary will be
      * read-only.
      * THIS METHOD IS NOT IMPLEMENTED YET!
      */
    bool                compile();
    /** Decompiles the dictinary.
      */
    bool                decompile();
    /** Returns true if the dictionary is compiled.
      * It is important to query the "compiled" status in order to know if the dictionary
      * is read-only or not.
      */
    bool                isCompiled();

    /** @name Container handling methods
      */
    //@{
    ///
    const_iterator      begin () const;
    ///
    const_iterator      end   () const;

    ///
    iterator            begin ();
    ///
    iterator            end   ();

    ///
    void                clear ();
    ///
    void                erase ( iterator it );
    ///
    void                erase ( const char * word );

    /** Finds the given word in the dictionary.
      * If the "caseMatch" parameter is true then the found word is strictly
      * equals the given string (case-sensitive match). If the "exactMatch" parameter
      * is false then the method tries to find the word that best matches with the given 
      * string
      */
    const_iterator      find  ( const char * word, bool matchCase ) const;
    /** Finds the given word in the dictionary.
      * The same as the const version of this method but returns with a non-const 
      * iterator
      */
    iterator            find  ( const char * word, bool matchCase );
    /** Creates a new word in the dictionary.
      * The method returns with an iterator of the newly created word. The values should
      * be set using this iterator.
      */
    iterator            create( const char * word );

    ///
    int                 size  ();
    ///
    bool                empty ();
    //@}

private:
    DictionaryHandle( class DictionaryImpl* implementation );

    void assign( class DictionaryImpl* implementation );

private:
    class DictionaryImpl* m_implementation;

    friend class EngineImpl;
    friend class EngineFactoryImpl;
};

} // namespace MM_TTSAPI

//@}

#endif // MM_TTSAPI_USER_DICTIONARY_H
