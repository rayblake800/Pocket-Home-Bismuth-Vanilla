#pragma once
/**
 * @file  Theme_Colour_ListenerInterface.h
 *
 * @brief  The interface used by the Colour::JSONResource to send updates to
 *         Colour::ConfigListener objects.
 */

namespace Theme { namespace Colour { class ListenerInterface; } }
namespace Theme { namespace Colour { class JSONResource; } }

#include "JuceHeader.h"
#include "Theme_Colour_UICategory.h"

class Theme::Colour::ListenerInterface
{
public:
    ListenerInterface() { }
    virtual ~ListenerInterface() { }

protected:
    /* Only the Listener and the JSONResource may access this interface. */
    friend class JSONResource;
    
    /**
     * @brief  Checks if a specific ID is tracked by this Listener.
     *
     * @param colourId  The ColourID value to search for in the Listener's 
     *                  tracked IDs.
     *
     * @return          Whether the Listener tracks the given ID.
     */
    virtual bool isTrackedId(const int colourId) const = 0;

    /**
     * @brief  Gets the list of ColourId values tracked by this Listener.
     *
     * @return  All tracked ColourIds.
     */
    virtual const juce::Array<int, juce::CriticalSection>& getTrackedIds() 
        const = 0;

    /**
     * @brief Notify the Listener when a colour value it tracks is updated. 
     *
     * When generic UICategory colors are updated, this method will run for 
     * each ColourId in that category that doesn't have an explicit colour 
     * definition.
     * 
     * @param colourId    The Juce ColourId value being updated.
     * 
     * @param updatedKey  The key string of the updated config value.
     *  
     * @param newColour   The updated Colour value.
     */
    virtual void colourChanged(
            const int colourId, 
            const juce::Identifier& updatedKey, 
            const juce::Colour newColour) = 0;
};
