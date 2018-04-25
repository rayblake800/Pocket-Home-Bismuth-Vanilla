#pragma once
#include <map>
#include "JuceHeader.h"

/**
 * @file Localized.h
 *
 * @brief Manages a set of LocalizedTextConsumer objects, which load all text
 *        displayed to the user.
 * 
 *  Displayed text is loaded from a locale/<localeName.json> file in the asset 
 *  folder.
 */

class Localized
{
public:
    /**
     * Loads all localized text into memory.
     * 
     * @param className   Identifies the text values for a particular class
     *                     in the localization file.  Only text values under
     *                     this name will be loaded.
     * 
     * @param localeName  Sets the name of the locale to load.  If this is
     *                     the empty string or the file is not found, the system
     *                     locale will be used.  If this locale and the system
     *                     locale are both missing localization files, 
     *                     defaultLocale will be used.
     * 
     */
    Localized(String className, String localeName = String());

    virtual ~Localized() { }

    /**
     * Get the name of the system locale.
     * 
     * @return The short locale name, used for naming locale text files and
     * identifying locale in .desktop files. 
     */
    static String getLocaleName();

    /**
     * Look up a piece of text for the current locale.
     * 
     * @param key  One of this object's text keys.
     * 
     * @return     The localized text string, or the empty string if text
     *              wasn't found.
     */
    String localeText(String key) const;

private:
    //All of the localized text for this class.
    std::map<String, String> localeStrings;
    //default locale to use if others aren't found.
    static const constexpr char * defaultLocale = "en_US";
};
