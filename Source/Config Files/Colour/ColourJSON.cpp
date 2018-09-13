#include "ColourJSON.h"
#include "ColourConfigKeys.h"

/* SharedResource object key */
const juce::Identifier ColourJSON::resourceKey = "ColourJSON";

/* Filename of the JSON configuration file */
static const constexpr char * configFilename = "colours.json";

ColourJSON::ColourJSON() : ConfigJSON(resourceKey, configFilename)
{
    loadJSONData();
}

/**
 * Sets this listener to receive updates when a specific Juce ColourId value 
 * changes.
 */
void ColourJSON::Listener::addTrackedColourId(const int colourId)
{
    const juce::ScopedLock colourLock(trackedColourIds.getLock());
    trackedColourIds.addIfNotAlreadyThere(colourId);
}

/**
 * Stops this listener from receiving updates when a specific Juce ColourId 
 * value changes.
 */
void ColourJSON::Listener::removeTrackedColourId(const int colourId)
{
    const juce::ScopedLock colourLock(trackedColourIds.getLock());
    trackedColourIds.removeAllInstancesOf(colourId);
}

/**
 * Checks if a single handler object is a Listener tracking updates of a single 
 * key value, and if so, notifies it that the tracked value has updated.
 */
void ColourJSON::notifyListener(ConfigJSON::Listener* listener,
        const juce::Identifier& key)
{
    using namespace juce;
    using namespace ColourConfigKeys;
    Listener* colourListener = dynamic_cast<Listener*>(listener);
    if(colourListener != nullptr)
    {
        String colourStr = getConfigValue<String>(key);
        int colourId = getColourId(key);
        if(colourStr.isEmpty())
        {
            if(colourId == -1)
            {
                //Category color removed, this shouldn't happen.
                DBG("ColourJSON::" << __func__ << ": Color category "
                        << key << " value was removed!");
                jassertfalse;
            }
            //Color removed, revert to category color
            colourStr = getConfigValue<String>
                    (getCategoryKey(getUICategory(colourId)));
            jassert(colourStr.isNotEmpty());
        }
        const Colour newColor(colourStr.getHexValue32());
        const juce::ScopedLock colourLock
            (colourListener->trackedColourIds.getLock());
        if(colourId > -1)
        {
            if(colourListener->trackedColourIds.contains(colourId))
            {
                colourListener->colourChanged(colourId, key, newColor);
            }
        }
        else //Color category changed
        {
            UICategory category = getCategoryType(key);
            for(const int& trackedId : colourListener->trackedColourIds)
            {
                if(getUICategory(trackedId) == category)
                {
                    const Identifier& idKey = getColourKey(trackedId);
                    if(idKey == invalidKey 
                            || getConfigValue<String>(idKey).isEmpty())
                    {
                        colourListener->colourChanged
                            (trackedId, key, newColor);
                    }
                }
            }
        }
    }
}

const std::vector<ConfigKey>& ColourJSON::getConfigKeys() const
{
    using namespace juce;
    using namespace ColourConfigKeys;
    static std::vector<ConfigKey> keys;
    if(keys.empty())
    {
        for(const Identifier& key : getColourKeys())
        {
            keys.push_back(ConfigKey(key, ConfigKey::stringType));
        }
    }
    return keys;
}
