/*************************************************************************
*                                                                        *
*  (C) Copyright 2000-2001 Mindmaker Ltd.                                *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2000.11.21.                                                  *
*                                                                        *
*************************************************************************/

#ifndef MM_TTSAPI_ERRORS_H
#define MM_TTSAPI_ERRORS_H

#include "DLLSpec.h"

/** @name Errors.h */
//@{

namespace MM_TTSAPI
{

/** Base class for all exceptions.
  */
class FLEXVOICE_DECLSPEC GenericException
{
public:
            ///
            GenericException( const char * details_ = "" ): m_details( details_ ) { }
            ///
    virtual ~GenericException() { }

    ///
    virtual const char *    what   () const { return "GenericException"; }
    ///
    virtual const char *    details() const { return m_details; }

protected:
    const char * m_details;
};

#define MMDECL_EXCEPTION( C_, B_, D_ )  \
    class FLEXVOICE_DECLSPEC C_ : public B_ \
    { \
    public: \
        C_( const char * d_ = D_ ) : B_( d_ ) { } \
        virtual const char *what   () const { return #C_; } \
    }

/** @name SystemException 
  */
MMDECL_EXCEPTION( SystemException,                  GenericException,     "A system error occured." );
/** @name ResourceException
  */
MMDECL_EXCEPTION(     ResourceException,            SystemException,      "A resource error occured." );
/** @name FileNotFoundException
  */
MMDECL_EXCEPTION(         FileNotFoundException,    ResourceException,    "File not found." );
/** @name FileIOException
  */
MMDECL_EXCEPTION(         FileIOException,          ResourceException,    "Cannot read/write a specific file." );
/** @name AudioException
  */
MMDECL_EXCEPTION(     AudioException,               SystemException,      "Audio object function failed." );
/** @name OutOfMemoryException
  */
MMDECL_EXCEPTION(     OutOfMemoryException,         SystemException,      "Out of memory." );
/** @name InvalidParamException
  */
MMDECL_EXCEPTION( InvalidParamException,            GenericException,     "Invalid parameter." );
/** @name NotSupportedException
  */
MMDECL_EXCEPTION( NotSupportedException,            GenericException,     "A feature is not supported." );

} // namespace MM_TTSAPI

//@} // Errors.h

#endif // MM_TTSAPI_ERRORS_H

