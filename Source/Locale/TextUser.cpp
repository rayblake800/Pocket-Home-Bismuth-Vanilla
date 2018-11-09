#include "AssetFiles.h"
#include "Config/MainFile.h"
#include "Config/MainKeys.h"
#include "Locale/TextUser.h"

/* Localized text data loaded from a JSON locale file: */
std::map<juce::Identifier, std::map<juce::Identifier, juce::String>> 
Locale::TextUser::localeData;

/* Default locale to use if no other is found: */
static const juce::String defaultLocale = "en_US"; 

/* The default POSIX locale, returned by the system when no locale is set. */
static const juce::String unsetLocale = "C";

/*
 * Initializes all localized text data.
 */
Locale::TextUser::TextUser(const juce::Identifier& className) : 
    className(className) 
{
    if(!localeData.empty()) // Already loaded data, skip initialization.
    {
        return;
    }
    using juce::String;
    using juce::NamedValueSet;
    Config::MainFile config;
    const juce::StringArray filesToTry = 
    {
        config.getConfigValue<String>(Config::MainKeys::localeKey),
        getLocaleName(),
        defaultLocale
    };

    juce::var jsonData;
    for(const String& filename : filesToTry)
    {
        //don't bother checking empty strings or the default (unset) locale
        if(filename.isEmpty() || filename == "C")
        {
            continue;
        }
        jsonData = AssetFiles::loadJSONAsset
                (String("locale/")+filename+".json",false);
        if(!jsonData.isVoid())
        {
            break;
        }
    }
    if(!jsonData.isObject())
    {
        // There should always be some locale file loaded, even if it's just the
        // default en_US file.
        jassertfalse;
        return;
    }
    NamedValueSet& textGroups = jsonData.getDynamicObject()->getProperties();
    for(auto group = textGroups.begin(); group != textGroups.end(); group++)
    {
        if(!group->value.isObject())
        {
            DBG("Locale::TextUser::TextUser: no data found in group "
                    << group->name);
            continue;
        }
        NamedValueSet& groupStrings 
            = group->value.getDynamicObject()->getProperties();
        auto& groupMap = localeData[className];
        for(auto textValue = groupStrings.begin();
                textValue != groupStrings.end(); textValue++)
        {
            localeData[group->name][textValue->name] 
                = textValue->value.toString();
        }
    }
}


/**
 * Look up a piece of text for the current locale.
 */
juce::String Locale::TextUser::localeText
(const juce::Identifier& textKey) const
{
    if(!localeData.count(className))
    {
        DBG("Locale::TextUser::" << __func__ << ": Couldn't find text values "
                << "for TextUser with key \"" << className.toString());
        return juce::String();
    }
    std::map<juce::Identifier, juce::String>& userStrings 
        = localeData[className];
    if(!userStrings.count(textKey))
    {
        DBG("Locale::TextFile::" << __func__ << ": Couldn't find text value \""
                << textKey.toString() << "\" for TextUser with key \""
                << className.toString() << "\"");
        return juce::String();
    }
    return userStrings[textKey];
}

