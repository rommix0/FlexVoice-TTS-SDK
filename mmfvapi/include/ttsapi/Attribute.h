/*************************************************************************
*                                                                        *
*  (C) Copyright 2000 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2001.04.03.                                                  *
*                                                                        *
*************************************************************************/

#ifndef MM_ATTRIBUTE_H
#define MM_ATTRIBUTE_H

#include "DLLSpec.h"

/** @name Attribute.h 
*/
//@{

namespace MM_TTSAPI
{

/** Attribute interface
  * Lets object hide the member variables from the client by defining parameters
  * with their name. Without using this interface the class can't keep the binary
  * compatibility if it is extended with new member. This means that all client program
  * should be rebuilt. With this solution only those applications should be rebuilt
  * that wants to use the new member.
  * 
  * The usage of this interface is not too complicated. E.g. if the application wants to
  * set an int member value (m_intvalue) instead of writting 
  *     object.m_intvalue = 1;
  * it should write
  *
  *     object.set( "intvalue", 1 );
  *
  * To read this value instead of writting 
  *
  *     int value = object.m_intvalue;
  *     ... using value...
  *
  * it should write
  *
  *     int value;
  *     if( object.get( "intvalue", value ) )
  *     {
  *         ... using value...
  *     }
  *
  * If the object doesn't support "m_intvalue" further, the get and set calls returns false.
  * Setting and reading members with some other simple type (bool, char, int, double) 
  * works the same. This interface doesn't support short float and any unsigned types.
  *
  * Handling strings is a bit different. 
  * If the length of the string can be maximized then the object documents this fact 
  * and the set and get methods handles this documented value. 
  * E.g. setting a string value (named "strvalue") that was maximized in 10 
  * can be done with the following call:
  *
  *     object.set( "strvalue", "hello" );
  *
  * If the given text is longer then the maximum value it will be truncated.
  * Reading this value:
  * 
  *     char buffer[10+1];
  *     object.get( "strvalue", buffer, 10+1 );
  *
  * In this example the value is garantied to fit in the given buffer.
  *
  * If the length of string-parameter is not maximized, no truncation will be done in the
  * set call. In this case the value-readin is more complex:
  *
  *     int reqsize=0;
  *     if( object.get( "strvalue", NULL, 0, reqsize )
  *     {
  *         char * buffer = new char[requsize+1];
  *         object.get( "strvalue", bufer, requsize+1 );
  *         ... using buffer...
  *     }
  *     
  * If a member is an array of any type its size can be handled by setArraySize/getArraySize
  * Accessing an element of an array can be done by the "index" parameter of 
  * the set/get methods
  * E.g:
  *
  * object.setArraySize( "intarray", 3 );
  * for( int i=0; i < 3; ++i )
  * {
  *     object.set( "intarray", 0, i );
  * }
  * ...
  * int size = object.getArraySize( "intarray", 3 );
  * for( int i=0; i < size; ++i )
  * {
  *     int value;
  *     object.get( "intarray", value, i );
  *     ...using value...
  * }
  *
  * If a member is a structure its IAttribute interface can be get by the
  * getAttribute method. E.g.:
  * 
  * IAttribute* attr = object.getAttribute( "complex_value" );
  * if( attr != NULL )
  * {
  *     attr->set( "intvalue", 0 );
  * }
  *
  */
class FLEXVOICE_DECLSPEC IAttribute
{
public:

    virtual         ~IAttribute() {}

    /** @name set/get functions
      * The following set functions (with all types) set the value of a parameter 
      * identified by the 'name', the get functions give the current value.
      * In case of arrays, the 'index' identifies
      * the array-index. The size of the array can be set by \ref{setArraySize}
      * and get by \ref{getArraySize}
      * If name is not recognized, the return value is false otherwise true
      */
    //@{

    ///
    virtual bool    set( const char * name, bool  data, int index=0 ) = 0;
    ///
    virtual bool    get( const char * name, bool& data, int index=0 ) const = 0;

    ///
    virtual bool    set( const char * name, char  data, int index=0 ) = 0;
    ///
    virtual bool    get( const char * name, char& data, int index=0 ) const = 0;

    ///
    virtual bool    set( const char * name, int   data, int index=0 ) = 0;
    ///
    virtual bool    get( const char * name, int&  data, int index=0 ) const = 0;

    ///
    virtual bool    set( const char * name, double  data, int index=0 ) = 0;
    ///
    virtual bool    get( const char * name, double& data, int index=0 ) const = 0;
    //@}

    /** Sets the value of a string parameter identified by the 'name'
     *  In case of arrays, the 'index' identifies the array-index
     *  if name is not recognized, the return value is false otherwise true
     *  The length of some parameter's value is maximized that is described
     *  in the documentation. The given longer values are truncated. This
     *  behaviour is because the easy usage.
     */
    virtual bool    set( const char * name, const char * data, int index=0 ) = 0;

    /** Gets the value of a string parameter identified by the 'name'
     *  In case of arrays, the 'index' identifies the array-index
     *  If name is not recognized, the return value is false otherwise true
     *  The result will be copied to the buffer that should be minimum
     *  'buffsize' large. If the required value is longer than the given size,
     *  the value will be truncated. If the 'reqsize' is not NULL, the real length
     *  are written to this parameter
     */
    virtual bool    get( const char * name, char* buffer, int buffsize, int* reqsize=0, int index=0 ) const = 0;

    virtual bool    setArraySize( const char * name, int  size ) = 0;
    virtual bool    getArraySize( const char * name, int& size ) const = 0;

    virtual const IAttribute*   getAttribute( const char * name ) const = 0;
    virtual IAttribute*         getAttribute( const char * name ) = 0;
};

/** Base must be derived from IAttribute
 */
template< typename Base_=IAttribute >
class FakeAttribute : public virtual Base_
{
public:
    virtual bool    set( const char * /*name*/, bool        /*data*/, int /*index*/=0 )       { return false; }
    virtual bool    get( const char * /*name*/, bool&       /*data*/, int /*index*/=0 ) const { return false; }
    virtual bool    set( const char * /*name*/, char        /*data*/, int /*index*/=0 )       { return false; }
    virtual bool    get( const char * /*name*/, char&       /*data*/, int /*index*/=0 ) const { return false; }
    virtual bool    set( const char * /*name*/, int         /*data*/, int /*index*/=0 )       { return false; }
    virtual bool    get( const char * /*name*/, int&        /*data*/, int /*index*/=0 ) const { return false; }
    virtual bool    set( const char * /*name*/, double      /*data*/, int /*index*/=0 )       { return false; }
    virtual bool    get( const char * /*name*/, double&     /*data*/, int /*index*/=0 ) const { return false; }
    virtual bool    set( const char * /*name*/, const char* /*data*/, int /*index*/=0 )       { return false; }
    virtual bool    get( const char * /*name*/, char* /*buffer*/, int /*buffsize*/, int* /*reqsize*/=0, int /*index*/=0 ) const { return false; }

    virtual bool    setArraySize( const char * /*name*/, int  /*size*/ )        { return false; }
    virtual bool    getArraySize( const char * /*name*/, int& /*size*/ ) const  { return false; }

    virtual const IAttribute*   getAttribute( const char * /*name*/) const { return 0; }
    virtual IAttribute*         getAttribute( const char * /*name*/)       { return 0; }
};

template class FLEXVOICE_DECLSPEC FakeAttribute<IAttribute>;

} // namespace MM_TTSAPI

//@}

#endif // MM_ATTRIBUTE_H
