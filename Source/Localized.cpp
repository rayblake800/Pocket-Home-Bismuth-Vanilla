#include "AssetFiles.h"
#include "Localized.h"

/**
 * Loads all localized text into memory.
 */
Localized::Localized(String className, String localeName)
{
    var localeFile;
    if (localeName.isNotEmpty())
    {
        localeFile = AssetFiles::loadJSONAsset
                (String("locale/") + localeName + ".json", false);
    }
    if (localeFile.isVoid())
    {
        localeFile = AssetFiles::loadJSONAsset
                (String("locale/") + getLocaleName() + ".json", false);
    }
    if (localeFile.isVoid())
    {
        localeFile = AssetFiles::loadJSONAsset
                (String("locale/") + defaultLocale + ".json", false);
    }
    jassert(!localeFile.isVoid());
    var classText = localeFile.getProperty(className, var());
    if (classText.isObject())
    {
        NamedValueSet& lineSet = classText.getDynamicObject()->getProperties();
        for (auto line = lineSet.begin(); line != lineSet.end(); line++)
        {
            localeStrings[line->name.toString()] = line->value.toString();
        }
    }
    else
    {
        DBG("Localized::" << __func__
                << ": Failed to load localized text for class " << className);
    }
}

/**
 * Get the name of the system locale.
 */
String Localized::getLocaleName()
{
    std::locale l("");
    return String(l.name()).initialSectionNotContaining(".");
}

/**
 * Look up a piece of text for the current locale.
 */
String Localized::localeText(String key) const
{
    try
    {
        return localeStrings.at(key);
    }
    catch (std::out_of_range e)
    {
        DBG("Localized::" << __func__ << ": Failed to find text with key "
                << key);
        return String();
    }
}