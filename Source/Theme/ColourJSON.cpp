#include "ColourJSON.h"
#include "ColourConfigKeys.h"

/* SharedResource object key */
const juce::Identifier ColourJSON::resourceKey = "ColourJSON";

/* Filename of the JSON configuration file */
static const constexpr char * configFilename = "colours.json";

ColourJSON::ColourJSON() : Config::FileResource(resourceKey, configFilename)
{
    loadJSONData();
}

ColourJSON::Listener::Listener() { }

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

/*
 * Calls configValueChanged for each tracked key, and calls colourChanged for 
 * each tracked colourId.
 */
void ColourJSON::Listener::loadAllConfigProperties()
{
    using namespace juce;
    using namespace ColourConfigKeys;
    const ScopedLock colourLock(trackedColourIds.getLock());
    for(const int& colourId : trackedColourIds)
    {
        const Identifier& idKey = getColourKey(colourId);
        if(idKey != invalidKey)
        {
            String colorStr = getConfigValue<String>(idKey);
            if(colorStr.isNotEmpty())
            {
                colourChanged(colourId, idKey,
                        Colour(colorStr.getHexValue32()));
                continue;
            }
        }
        // ID has no key, or specific color not defined, use category color:
        UICategory idCategory = getUICategory(colourId);
        if(idCategory != UICategory::none)
        {
            const Identifier& catKey = getCategoryKey(idCategory);
            colourChanged(colourId, catKey,
                    Colour(getConfigValue<String>(catKey).getHexValue32()));
        }
    }
    Config::Listener<ColourJSON>::loadAllConfigProperties();
}

/**
 * Checks if a single handler object is a Listener tracking updates of a single 
 * key value, and if so, notifies it that the tracked value has updated.
 */
void ColourJSON::notifyListener(Config::Listener<ColourJSON>* listener,
        const juce::Identifier& key)
{
    using namespace juce;
    using namespace ColourConfigKeys;
    Listener* colourListener = dynamic_cast<Listener*>(listener);
    if(colourListener != nullptr)
    {
        // Check for and notify listeners tracking colors by ColorId:
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

const std::vector<Config::DataKey>& ColourJSON::getConfigKeys() const
{
    using juce::Identifier;
    using ColourConfigKeys::getColourKeys;
    static std::vector<Config::DataKey> keys;
    if(keys.empty())
    {
        for(const Identifier& key : getColourKeys())
        {
            keys.push_back(Config::DataKey(key, Config::DataKey::stringType));
        }
    }
    return keys;
}
