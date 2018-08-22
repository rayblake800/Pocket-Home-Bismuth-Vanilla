/*
  ==============================================================================

    SignalHandler.cpp
    Created: 21 Aug 2018 4:00:32pm
    Author:  anthony

  ==============================================================================
*/

#include "SignalHandler.h"

/*
 * Adds the signal handler's address to the list of valid signal
 * handlers.
 */
GPPObject::SignalHandler::SignalHandler()
{
    //ADDR_LOG(this,"Created as SignalHandler");
    GPPObject::signalHandlers.addIfNotAlreadyThere(this);
}


        
/*
 * Copies an existing signal handler, subscribing to all of that
 * handler's tracked signals.
 */
GPPObject::SignalHandler::SignalHandler(const SignalHandler& rhs)
{   
    
    //ADDR_LOG(this,"Created as SignalHandler copying ", &rhs);
    //ADDR_LOG(&rhs,"Shared data with ", this);
    GPPObject::signalHandlers.addIfNotAlreadyThere(this);
    for(GPPObject* signalSource : sources)
    {
        signalSource->addSignalHandler(this);
    }
}
        
/*
 * Removes all of its signal handling callback functions from within the
 * signal context thread, and remove this signal handler's address from
 * the list of valid signal handlers.
 */
GPPObject::SignalHandler::~SignalHandler()
{
    while(!sources.isEmpty())
    {
        GPPObject* source = sources[0];
        if(source != nullptr)
        {
            if(source->isNull())
            {
                DBG("GPPObject::SignalHandler::" << __func__ 
                        << ": Error: Found null signal source");
            }
            else
            {
                source->removeSignalHandler(this);
            }
            sources.removeAllInstancesOf(source);
        }
    }
    GPPObject::signalHandlers.removeAllInstancesOf(this);
    //ADDR_LOG(this, "Destroyed as SignalHandler");
}

/*
 * Callback function for handling property change notification signals.
 * Subclasses should override this to handle all specific property 
 * change notifications that they support.
 */
void GPPObject::SignalHandler::propertyChanged
(GPPObject* source, juce::String property)
{
    DBG("GPPObject::SignalHandler::" << __func__ << ": Unexpected notification"
            << "for property " << property); 
}
