#include "ComponentJSON.h"

// SharedResource object key
const ComponentJSON juce::Identifier resourceKey("ComponentJSON");
// JSON configutation file name
static const constexpr char * configFilename = "component.json";

ComponentJSON::ComponentJSON() : ConfigFile(filenameConst)
{
    using namespace juce;
    StringArray keys = getComponentKeys();
    for (const String& key : keys)
    {
	DynamicObject::Ptr componentData = initProperty<DynamicObject*>(key);
        components[key] = ComponentSettings(componentData);
    }
    loadJSONData();
}

/*
 * Gets the configured settings for a particular component.
 */
ComponentConfigFile::ComponentSettings
ComponentJSON::getComponentSettings(juce::String componentKey)
{
    return components[componentKey];
}

/*
 * @return the list of key Strings for each integer value tracked in 
 * components.json
 */
const std::vector<ConfigKey>& 
ComponentJSON::getDataKeys() const
{
    static const std::vector<ConfigKey> keys = 
    {
        {smallTextKey,  ConfigFile::doubleType},
        {mediumTextKey, ConfigFile::doubleType},
        {largeTextKey,  ConfigFile::doubleType}
    };
    return keys;
}

/*
 * Copy all ComponentSettings data back to the JSON file.
 */
void ComponentJSON::writeDataToJSON()
{
    using namespace juce;
    const StringArray& keys = getComponentKeys();
    for (const String& key : keys)
    {
        if(components.count(key) != 0)
        {
            updateProperty<DynamicObject*>(key, 
                    components[key].getDynamicObject());
        }
    }
}

