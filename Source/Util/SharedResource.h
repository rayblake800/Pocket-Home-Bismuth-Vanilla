#pragma once
#include "JuceHeader.h"

/**
 * @file SharedResource.h
 *
 * @brief An abstract Singleton class structure to be managed through
 *        a ResourceManager.
 * 
 * @see ResourceManager.h
 */
 
 class SharedResource
 {
 public:
    friend class ResourceManager;
            
   /**
    * While debugging, prints an error if the resource is destroyed while the 
    * reference list is not empty.
    */
    virtual ~SharedResource();

private:
   /**
    * SharedResource objects may only be created by their ResourceManager.
    */ 
    SharedResource() { }

    //Holds a reference to every existing ResourceManager that uses this
    //resource.
    juce::Array<ResourceManager*> referenceList;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SharedResource)
 };

