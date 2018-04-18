/**
 * @File SafeSignaler.h
 * 
 * @brief SafeSignaler can send signals to subscribed Listener objects. 
 *         Listeners can be added and removed safely at any time, and will
 *         remove themselves on destruction.
 * 
 *  I'd prefer for each signaler to have a distinct Listener type associated
 *  with it, but objects often need to have multiple Listener types, and 
 *  inheriting from multiple subclasses of a generic Listener class is not
 *  possible.  
 * 
 * For now, I think the best course of action is to store this generic
 * implementation of this design pattern here, and just copy and adapt it
 * whenever its needed.
 */
#pragma once

class SafeSignaler
{
public:

    class Listener
    {
        friend class SafeSignaler;
    public:

        Listener() { }

        /**
         * Removes itself from all signalers.
         */
        virtual ~Listener()
        {
            for (SafeSignaler* signaler : listening)
            {
                const ScopedLock signalerLock(signaler->listenerLock);
                signaler->listeners.removeAllInstancesOf(this);
                signaler->awaitingSignal.removeAllInstancesOf(this);
            }
        }

    private:
        //sends whatever notification the listener receives
        virtual void notify() = 0;

        Array<SafeSignaler*, CriticalSection> listening;
    };
    friend class Listener;

    SafeSignaler() { }

    //Removes itself from all listeners
    ~SafeSignaler()
    {
        const ScopedLock updateLock(listenerLock);
        awaitingSignal.clear();
        for (Listener * listener : listeners)
        {
            listener->listening(removeAllInstancesOf(this));
        }
    }

    void addListener(Listener* listener)
    {
        const ScopedLock addLock(listenerLock);
        listeners.addIfNotAlreadyThere(listener);
        if (!awaitingSignal.isEmpty())
        {
            awaitingSignal.addIfNotAlreadyThere(listener);
        }
        listener->listening.addIfNotAlreadyThere(this);
    }

    void removeListener(Listener* listener)
    {
        const ScopedLock removeLock(listenerLock);
        listeners.removeAllInstancesOf(listener);
        awaitingSignal.removeAllInstancesOf(listener);
        listener->listening.removeAllInstancesOf(this);
    }

    void signalListeners()
    {
        const ScopedLock signalLock(listenerLock);
        while (!awaitingSignal.isEmpty())
        {
            //All notification occurs on the message thread, so we must be
            //calling from another thread while the message thread handles a
            //different set of notifications.  Unlock and yield until
            //awaitingSignal is empty again.
            const ScopedUnlock signalUnlock(listenerLock);
            Thread::yield();
        }
        awaitingSignal = listeners;
        while (!awaitingSignal.isEmpty())
        {
            Listener* toNotify
                    = awaitingSignal.removeAndReturn(awaitingSignal.size() - 1);
            const ScopedUnlock signalUnlock(listenerLock);
            const MessageManagerLock mmLock;
            toNotify->notify();
        }
    }

private:
    CriticalSection listenerLock;

    Array<Listener*> listeners;
    Array<Listener*> awaitingSignal;

};
