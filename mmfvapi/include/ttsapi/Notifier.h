/*************************************************************************
*                                                                        *
*  (C) Copyright 2000-2001 Mindmaker Ltd.                                *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2000.10.13.                                                  *
*                                                                        *
*************************************************************************/

#ifndef MM_NOTIFIER_H
#define MM_NOTIFIER_H

#include "DLLSpec.h"

/** @name Notifier.h
  * Notification dispatching, receiving.
  */
//@{

namespace MM_TTSAPI
{

class Bookmark;
class BookmarkTypeList;

/// Defines notification methods to be implemented by the API client.
class FLEXVOICE_DECLSPEC INotify
{
public:
    virtual ~INotify() {}
    /** This method is called when a bookmark is bypassed.
	  * Note that by definition processing a bookmark (a notification)
	  * should be done in a very short amount of time (no blocking is allowed).
      */
    virtual void bookmark( const Bookmark& bookmark ) = 0;
    /** Should be used as a "pre-notification".
      * By intention, this method is called by an output site just
      * {\em before} the bookmark is passed to the actual output device;
	  * this should enable the listeners to prepare what they need to 
	  * prepare.
	  * Note that, by definition, processing a bookmark (a notification)
	  * should be done in a very short amount of time (no blocking is allowed).
      * @see IOutputSite
      */
    virtual void prepare( const Bookmark& bookmark ) = 0;
};
//xx: TODO: rename INotify to IListener.
//xx  Replace will work for the documentation as well.

/** Interface for managing notification dispatching.
  * An INotifyDispatcher interface stores a number of
  * \Ref{INotify} instances. When a notification arrives, it goes through
  * this internal list and calls the notification method of
  * all the INotify instances for which the given notification type is
  * permitted. An INotifyDispatcher typically implements an \Ref{INotify}
  * interface, as well.
  */
class FLEXVOICE_DECLSPEC INotifyDispatcher
{
public:
    virtual         ~INotifyDispatcher() {}

    /** Adds the given INotify instance to the internal list of INotify instances
      * managed by the dispatcher.
      * Ownership is not transferred. The client is responsible for managing the lifetime
      * of the INotify instance.
      * @param notify non-zero pointer to an INotify instance.
      * @param types  `list' of bookmark types, the given INotify instance will be
      *               notified of.
      */
    virtual void    registerNotify      ( INotify * notify, const BookmarkTypeList& types ) = 0;
    //xx exception specs?

    /** Deletes the given INotify instance from the internal list of INotify instances
      * managed by the dispatcher.
      */
    virtual void    unregisterNotify    ( INotify * notify                                ) = 0;
    //xx exception specs?

    /** Allows the manipulation of the bookmark types associated with a given INotify instance.
      */
    virtual void    addBookmarkTypes    ( INotify * notify, const BookmarkTypeList& types ) = 0;
    //xx exception specs?

    /** Allows the manipulation of the bookmark types associated with a given INotify instance.
      */
    virtual void    removeBookmarkTypes ( INotify * notify, const BookmarkTypeList& types ) = 0;
};
//xx INotifyDispatcher -> NotificationDispatcher??

/// The purpose of this class is to receive and dispatch the received notifications.
class FLEXVOICE_DECLSPEC INotifier : public virtual INotify
                                   , public virtual INotifyDispatcher
{
public:
    virtual ~INotifier() {}
};

/** The purpose of this class is to provide an implementation of
  * INotify such that the processing of the notifications happens
  * on a thread different from the notification thread.
  * Why anyone would like to do this? If you want to call methods
  * of the engine from notification calls then you must do this
  * to avoid certain dead-locks.
  *
  * Clients may derive from this class to specialize the way they
  * process normal and prepare bookmark notifications. The default
  * implementation does nothing.
  */
class FLEXVOICE_DECLSPEC NotifyTransfer : public virtual INotify
{
public:
	///
                    NotifyTransfer();
	///
    virtual         ~NotifyTransfer();

// INotify
    /** Clients deriving from this object can implement this function
      * to process the bookmark using the notification thread
	  * at the time the bookmark is receieved (see \Ref{INotify}).
	  * Note that, by definition, processing a bookmark (a notification)
	  * should be done in a very short amount of time (no blocking is allowed).
	  */
    virtual void    bookmark( const Bookmark& bookmark );
    /** Clients deriving from this object can implement this function
      * to process the bookmark using the notification thread
	  * at the time the bookmark-prepare notification
	  * is received (see \Ref{INotify}).
	  * Note that, by definition, processing a bookmark (a notification)
	  * should be done in a very short amount of time (no blocking is allowed).
	  */
    virtual void    prepare ( const Bookmark& bookmark );


// NotifThreadTransfer
    /** Clients should really implement this function -
	  * to actually process the notifications. Since this method is called
	  * asynchronously from a thread different to the one calling
	  * the bookmark method above, it should be safe to call
	  * e.g. functions of the engine or block here.
      */
    virtual void    bookmarkProcess( const Bookmark& bookmark );
    /** Same as \Ref{bookmarkProcess}, just corresponds to the prepare calls.
	  */
    virtual void    prepareProcess ( const Bookmark& bookmark );
    /** This function can be called to stop the notification loop or thread
	  * associated with this object. The parameter determines if the call is blocking.
	  */
    void            stopNotifyLoop( bool wait );
    /** This function can be overriden by the Clients - it will be called
	  * when the notification loop has actually stopped.
	  */
    virtual void    notifyLoopStopped();

private:
                    /// NotifyTransfer shouldn't be copied!
                    NotifyTransfer( const NotifyTransfer& );
                    /// NotifyTransfer shouldn't be copied!
    NotifyTransfer& operator=     ( const NotifyTransfer& );

    class NotifyTransferImpl* m_implementation;
};

/** An implementation of the \Ref{INotifier} interface.for dispatching notifications.
  */
class FLEXVOICE_DECLSPEC Notifier : public virtual INotifier
{
public:
// INotifier
    ///
    virtual void        registerNotify      ( INotify * notify, const BookmarkTypeList& types );
    ///
    virtual void        unregisterNotify    ( INotify * notify )                ;

    ///
    virtual void        addBookmarkTypes    ( INotify * notify, const BookmarkTypeList& types );
    ///
    virtual void        removeBookmarkTypes ( INotify * notify, const BookmarkTypeList& types );
// INotify

    ///
    virtual void        bookmark( const Bookmark& bookmark );
    ///
    virtual void        prepare ( const Bookmark& bookmark );

// Notifier
public:
                        Notifier ();
                        ~Notifier();
private:
                        /// Notifier shouldn't be copied!
                        Notifier ( const Notifier& );
                        /// Notifier shouldn't be copied!
    INotifier&          operator=( const Notifier& );

    class NotifierImpl* m_implementation;
};

} // namespace MM_TTSAPI

//@}

#endif // MM_NOTIFIER_H
