#include "ComponentJSON.h"
#include "ComponentConfigKeys.h"

/* SharedResource object key */
const juce::Identifier ComponentJSON::resourceKey("ComponentJSON");

/* Filename of the JSON configuration file */
static const constexpr char * configFilename = "components.json";

ComponentJSON::ComponentJSON() : ConfigJSON(resourceKey, configFilename)
{
    using namespace juce;
    const Array<Identifier> keys = ComponentConfigKeys::componentKeys();
    for (const Identifier& key : keys)
    {
	DynamicObject::Ptr componentData = initProperty<DynamicObject*>(key);
        components[key] = ComponentSettings(componentData);
    }
    loadJSONData();
}

/*
 * Gets the configured settings for a particular component.
 */
ComponentSettings ComponentJSON::getComponentSettings
(const juce::Identifier& componentKey) const
{
    return components.at(componentKey);
}

/*
 * Gets the key string and data type for each basic value stored in 
 * components.json.
 */
const std::vector<ConfigKey>& 
ComponentJSON::getConfigKeys() const
{
    return ComponentConfigKeys::basicKeys();
}

/*
 * Copy all ComponentSettings data back to the JSON file.
 */
void ComponentJSON::writeDataToJSON()
{
    using namespace juce;
    const Array<Identifier>& keys = ComponentConfigKeys::componentKeys();
    for (const Identifier& key : keys)
    {
        if(components.count(key) != 0)
        {
            updateProperty<DynamicObject*>(key, 
                    components[key].getDynamicObject());
        }
    }
}

