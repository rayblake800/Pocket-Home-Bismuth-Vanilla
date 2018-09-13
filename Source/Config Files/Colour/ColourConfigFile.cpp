#include "ColourIds.h"
#include "ColourConfigFile.h"
#include "ColourConfigKeys.h"
#include "AssetFiles.h"
#include "Utils.h"

/*
 * Look up the Colour value saved for a specific Juce ColourId.
 */
juce::Colour ColourConfigFile::getColour(const int colourId) const
{
    using namespace ColourConfigKeys;
    using namespace juce;
    String colourStr = getColourString(getColourKey(colourId));
    if(colourStr.isEmpty())
    {
        //No specific value set, look up the UICategory value.
        colourStr = getColourString(getCategoryKey(getUICategory(colourId)));
    }
    jassert(colourStr.isNotEmpty());
    return Colour(colourStr.getHexValue32());
}

/*
 * Gets the Colour value assigned as the default for all UI items in a
 * specific category.
 */
juce::Colour ColourConfigFile::getColour(UICategory category) const
{
    using namespace ColourConfigKeys;
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
juce::Colour ColourConfigFile::getColour
(const juce::Identifier& colourKey) const
{
    using namespace juce;
    return Colour(getColourString(colourKey).getHexValue32());   
}

/**
 * Sets the saved colour value for a single UI element.
 */
void ColourConfigFile::setColour
(const int colourId, const juce::Colour newColour)
{
    using namespace ColourConfigKeys;
    using namespace juce;
    const Identifier& colourKey = getColourKey(colourId);
    if(colourKey != invalidKey)
    {
        setColour(colourKey, newColour);
    }
}

/**
 * Sets the saved colour value for a category of UI elements.
 */
void ColourConfigFile::setColour
(const UICategory category, const juce::Colour newColour)
{
    using namespace ColourConfigKeys;
    if(category != none)
    {
        setColour(getCategoryKey(category), newColour);
    }
}
 
/*
 * Sets the saved colour value for a specific key string.
 */
void ColourConfigFile::setColour
(const juce::Identifier& colourKey, const juce::Colour newColour)
{
    using namespace juce;
    setConfigValue<String>(colourKey, newColour.toString());
}


/*
 * Gets a Colour string saved to the colour config file.
 */
juce::String ColourConfigFile::getColourString
(const juce::Identifier& colourKey) const
{
    using namespace juce;
    return getConfigValue<String>(colourKey);
}




