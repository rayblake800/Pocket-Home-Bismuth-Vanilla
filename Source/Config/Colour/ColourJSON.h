#pragma once
#include "Config/FileResource.h"

class ColourJSON : public Config::FileResource
{
public:
    /* SharedResource object key */
    static const juce::Identifier resourceKey;

    ColourJSON();

    virtual ~ColourJSON() { }
    
    /**
     * Receives updates when color configurations change.
     */
    class Listener : protected Config::FileResource::Listener
    {
    friend class ColourJSON;
    protected:
        Listener() : Config::FileResource::Listener(ColourJSON::resourceKey,
                []()->SharedResource* { return new ColourJSON(); }) { }

        virtual ~Listener() { }

        /**
         * @brief  Sets this listener to receive updates when a specific Juce
         *         ColourId value changes.
         *
         * @param colourId  A color identifier for this listener to track.
         */
        void addTrackedColourId(const int colourId);

        /**
         * @brief  Stops this listener from receiving updates when a specific 
         *         Juce ColourId value changes.
         *
         * @param colourId  A color identifier this listener will no longer
         *                  track
         */
        void removeTrackedColourId(const int colourId);

        /**
         * @brief  Calls configValueChanged for each tracked key, and calls
         *         colourChanged for each tracked colourId.
         */
        virtual void loadAllConfigProperties() override;

    private:
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
       
        /* All tracked ColourId values */        
        juce::Array<int, juce::CriticalSection> trackedColourIds;
    };
     
private:
    /**
     * @brief   Gets the set of all basic (non-array, non-object) properties 
     *          tracked by this ConfigJSON.
     * 
     * @return  The keys to all variables tracked in this config file.
     */
    virtual const std::vector<Config::DataKey>& getConfigKeys()
        const final override;

    /**
     * @brief  Checks if a single handler object is a Listener tracking updates
     *         of a single key value, and if so, notifies it that the tracked
     *         value has updated.
     *
     * @param listener          A Listener object attached to the ConfigJSON
     *                          object.
     *
     * @param key               The key to an updated configuration value.
     */
    virtual void notifyListener(Config::FileResource::Listener* listener,
            const juce::Identifier& key) override; 
};

