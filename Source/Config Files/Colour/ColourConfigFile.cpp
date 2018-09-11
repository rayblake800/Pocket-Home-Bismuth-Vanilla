#include "ColourIds.h"
#include "ColourConfigFile.h"
#include "AssetFiles.h"
#include "Utils.h"

/*
 * Look up the Colour value saved for a specific Juce ColourId.
 */
juce::Colour ColourConfigFile::getColour(int colourId) const
{
    using namespace juce;
    String colourKey = getColourKey(colourId);
    String colourStr = getColourString(colourKey);
    if(colourStr.isEmpty())
    {
        //No specific value set, look up the UICategory value.
        colourKey = getCategoryKey(getUICategory(colourId));
        colourStr = getColourString(colourKey);
    }
    return Colour(colourStr.getHexValue32());
}

/*
 * Gets the Colour value assigned as the default for all UI items in a
 * specific category.
 */
juce::Colour ColourConfigFile::getColour(UICategory category) const
{
    using namespace juce;
    if(category == none)
    {
        return Colour();
    }
    return getColour(getCategoryKey(category));
}
 
/**
 * Gets the colour value associated with a particular key string.
 */
juce::Colour ColourConfigFile::getColour(juce::String colourKey) const
{
    using namespace juce;
    return Colour(getColourString(colourKey).getHexValue32());   
}

/**
 * Sets the saved colour value for a single UI element.
 */
void ColourConfigFile::setColour(int colourId, juce::Colour newColour)
{
    using namespace juce;
    String colourKey = getColourKey(colourId);
    if(colourKey.isNotEmpty())
    {
        setColour(colourKey, newColour);
    }
}

/**
 * Sets the saved colour value for a category of UI elements.
 */
void ColourConfigFile::setColour(UICategory category, juce::Colour newColour)
{
    if(category != none)
    {
        setColour(getCategoryKey(category), newColour);
    }
}
 
/*
 * Sets the saved colour value for a specific key string.
 */
void ColourConfigFile::setColour(juce::String colourKey, juce::Colour newColour)
{
    using namespace juce;
    setConfigValue<String>(colourKey, newColour.toString());
}
   
/*
 * Calls colourValueChanged for each Juce ColourId associated with
 * the updated value's key.
 */
void ColourConfigFile::Listener::configValueChanged(juce::String propertyKey)
{
    using namespace juce;
    ColourConfigFile config;
    String colourString;
    try
    {
        colourString = config.getColourString(propertyKey);
    }
    catch(ConfigFile::BadKeyException e)
    {
        nonColorValueChanged(propertyKey);
        return;
    }
    if(colourString.isEmpty())
    {
        return;
    }
    Colour newColour = Colour(colourString.getHexValue32());
    
    int colourId = getColourId(propertyKey);
    if(colourId >= 0)
    {
        colourValueChanged(colourId, propertyKey, newColour);
    }
    else
    {
        UICategory category = getCategoryType(propertyKey);
        if(category == UICategory::none)
        {
            return;
        }
        
        for(const int& trackedId : trackedColourIds)
        {
            if(getUICategory(trackedId) == category)
            {
                String idKey = getColourKey(trackedId);
                if(idKey.isEmpty() || config.getColourString(idKey).isEmpty())
                {
                    colourValueChanged(trackedId, propertyKey, newColour);
                }
            }
        }      
    }
}

/*
 * Gets a Colour string saved to the colour config file.
 */
juce::String ColourConfigFile::getColourString(juce::String colourKey) const
{
    using namespace juce;
    return getConfigValue<String>(colourKey);
}


/*
 * Gets the key string used to store a specific Juce ColourId in the
 * colour config file.
 */
juce::String ColourConfigFile::getColourKey(int colourId)
{
    using namespace juce;
    auto keySearch = colourIdKeys.find(colourId);
    if(keySearch == colourIdKeys.end())
    {
        return String();
    }
    return keySearch->second;
}

ColourConfigFile::ConfigJson::ConfigJson() : ConfigFile(filenameConst)
{
    loadJSONData();
}

const std::vector<ConfigFile::DataKey>&  
ColourConfigFile::ConfigJson::getDataKeys() const
{
    using namespace juce;
    static std::vector<DataKey> keys;
    if(keys.empty())
    {
        for(const String& key : uiCategoryKeys)
        {
            keys.push_back({key,stringType});
        }
        for (auto it = colourIds.begin(); it != colourIds.end(); it++)
        {
            keys.push_back({it->first, stringType});
        }
    }
    return keys;
}


