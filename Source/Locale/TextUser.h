#pragma once
#include <map>
#include "JuceHeader.h"
#include "SharedResource/Handler.h"
#include "Locale/Locale.h"

/**
 * @file  Locale/TextUser.h
 *
 * @brief  Provides access to all localized text strings by class.
 */

class Locale::TextUser
{
public:
    /**
     * @brief  Creates a TextUser, loading locale data if necessary.
     * 
     * @param className    Identifies the text values for a particular class
     *                     in the localization file.  Only text values under
     *                     this name will be accessed.
     */
    TextUser(const juce::Identifier& className);

    virtual ~TextUser() { }

protected:
    /**
     * @brief  Looks up a piece of text for the current locale.
     * 
     * @param key  One of this object's text keys.
     * 
     * @return     The localized text string, or the empty string if text
     *             wasn't found.
     */
    juce::String localeText(const juce::Identifier& key) const;

private:
    /* The key to all localized strings that belong to this class: */
    const juce::Identifier& className;

    /* Localized text data loaded from a JSON locale file: */
    static std::map<juce::Identifier, 
        std::map<juce::Identifier, juce::String>> localeData;
};
