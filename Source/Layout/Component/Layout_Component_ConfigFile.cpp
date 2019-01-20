#include "Layout_Component_ConfigFile.h"
#include "Layout_Component_JSONKeys.h"
#include "Layout_Component_JSONResource.h"
#include "Layout_Component_ConfigLayout.h"
#include "AssetFiles.h"
#include "Utils.h"

namespace ComponentLayout = Layout::Component;

ComponentLayout::ConfigFile::ConfigFile() { }

/*
 * Gets configured component layouts from shared JSON file data.
 */
ComponentLayout::ConfigLayout 
ComponentLayout::ConfigFile::getLayout(const juce::Identifier& componentKey)
{
    SharedResource::LockedPtr<const JSONResource> config 
            = getReadLockedResource();
    return config->getLayout(componentKey);
}

/*
 * Return the most appropriate font size for drawing text
 */
int ComponentLayout::ConfigFile::getFontHeight
(juce::Rectangle <int> textBounds, juce::String text)
{
    SharedResource::LockedPtr<const JSONResource> config 
            = getReadLockedResource();

    int numLines = 1;
    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] == '\n')
        {
            numLines++;
        }
    }

    int height = textBounds.getHeight() / numLines;
    juce::Font defaultFont(height);
    int width = defaultFont.getStringWidth(text);
    if (width > textBounds.getWidth())
    {
        height = textBounds.getWidth() * height / width;
    }

    int testHeight = getFontHeight(TextSize::largeText);
    if (height > testHeight)
    {
        return testHeight;
    }
    testHeight = getFontHeight(TextSize::mediumText);
    if (height > testHeight)
    {
        return testHeight;
    }
    testHeight = getFontHeight(TextSize::smallText);
    if (height > testHeight)
    {
        return testHeight;
    }
    return height;
}

/**
 * Gets the height in pixels of one of the three configured text sizes.
 */
int ComponentLayout::ConfigFile::getFontHeight(TextSize sizeType)
{
    namespace Keys = Layout::Component::JSONKeys;
    const juce::Identifier& key = (sizeType == TextSize::smallText ? 
            Keys::smallText :
            (sizeType == TextSize::mediumText ?
                Keys::mediumText : Keys::largeText));
    double size = getReadLockedResource()->getConfigValue<double>(key);
    if(size > 1)
    {
        return (int) size;
    }
    return (int) (size * getWindowBounds().getHeight());
}
