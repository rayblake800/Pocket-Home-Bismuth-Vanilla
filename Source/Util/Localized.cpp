#include "AssetFiles.h"
#include "Localized.h"

/**
 * Loads all localized text into memory.
 */
Localized::Localized(juce::String className, juce::String localeName)
{
    using namespace juce;
    StringArray filesToTry = {localeName, getLocaleName(), defaultLocale};
    var localeFile;
    for(const String& filename : filesToTry)
    {
        //don't bother checking empty strings or the default (unset) locale
        if(filename.isEmpty() || filename == "C")
        {
            continue;
        }
        localeFile = AssetFiles::loadJSONAsset
                (String("locale/")+filename+".json",false);
        if(!localeFile.isVoid())
        {
            break;
        }
    }
    jassert(!localeFile.isVoid());
    var classText = localeFile.getProperty(className, var());
    if (classText.isObject())
    {
        NamedValueSet& lineSet = classText.getDynamicObject()->getProperties();
        for (auto line = lineSet.begin(); line != lineSet.end(); line++)
        {
            localeStrings.set(line->name.toString(), line->value.toString());
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
juce::String Localized::getLocaleName()
{
    using namespace juce;
    std::locale l("");
    return String(l.name()).initialSectionNotContaining(".");
}

/**
 * Look up a piece of text for the current locale.
 */
juce::String Localized::localeText(juce::String key) const
{
    using juce::String;
    if(localeStrings.size() == 0)
    {
        DBG("Localized::" << __func__ << ": localized text not found!");
        return String();
    }
    return localeStrings[key];
}
