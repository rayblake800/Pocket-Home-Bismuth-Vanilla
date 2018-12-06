#pragma once
/**
 * @file ComponentConfigFile.h
 * 
 * @brief Gets and sets all component settings defined in 
 *        ~/.pocket-home/components.json.
 */

#include "Config/FileHandler.h"
#include "Config/Listener.h"

class ComponentJSON;
class ComponentSettings;

/** 
 * This reads and writes relative component size and position, image asset 
 * filenames, and other miscellaneous data relevant to UI components.
 */
class ComponentConfigFile : public Config::FileHandler<ComponentJSON>
{
public:
    ComponentConfigFile();

    virtual ~ComponentConfigFile() { }

    /**
     * @brief   Gets configured component settings from shared .json file data.
     * 
     * @param componentKey  A configurable UI component's key string.
     * 
     * @return  The properties defined for that component.
     */
    ComponentSettings getComponentSettings
    (const juce::Identifier& componentKey);

    /**
     * @brief  Represents the three main text size options.  
     *
     *  The actual size of each of these options is set in components.json, 
     * either as a fraction of the window height (if textSize <= 1) or as a 
     * fixed height in pixels (if textSize > 1).
     */
    enum TextSize
    {
        smallText,
        mediumText,
        largeText
    };

    /**
     * @brief  Returns the most appropriate font size for drawing text.
     * 
     * @param textBounds  The area in which the text will be drawn.
     * 
     * @param text        The actual text being drawn.
     * 
     * @return            Whichever font height (small, medium, or large) 
     *                    defined in components.json would best fit this text 
     *                    within its bounds, or the largest height that will fit
     *                    if even the small font is too large to fit. 
     */
    int getFontHeight(juce::Rectangle <int> textBounds, juce::String text);

    /**
     * @brief   Gets the height in pixels of one of the three configured text 
     *          sizes.
     *
     * @param sizeType  A text size type defined in the Component config file.
     *
     * @return          The height in pixels of that text size type.
     */
    int getFontHeight(TextSize sizeType);

    class Listener : protected Config::Listener<ComponentJSON>
    {
    public:
        Listener();

        virtual ~Listener() { }
    };

private:
    /**
     * @brief   Gets the list of all configurable component keys.
     * 
     * @return  The list of component keys.
     */
    static const juce::StringArray& getComponentKeys();
};
