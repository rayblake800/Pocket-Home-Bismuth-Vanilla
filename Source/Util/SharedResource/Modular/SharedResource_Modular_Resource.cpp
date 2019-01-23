#include "SharedResource_Modular_Resource.h"

/*
 * Creates the single instance of a Resource subclass.
 */
SharedResource::Modular::Resource::Resource
(const juce::Identifier& resourceKey) : 
SharedResource::Resource(resourceKey) { }
