#include "../Basic Components/DrawableImageComponent.h"
#include "../Basic Components/ListEditor.h"
#include "../Basic Components/FileSelectTextEditor.h"
#include "../Basic Components/OverlaySpinner.h"
#include "../Basic Components/SwitchComponent.h"
#include "../Pages/AppMenu Page/AppMenuButton/AppMenuButton.h"
#include "../Utils.h"
#include "ComponentConfigFile.h"

const std::map<String,int> ComponentConfigFile::colourIds{
    {"Image color 0",
            DrawableImageComponent::imageColour0Id},
    {"Image color 1",
            DrawableImageComponent::imageColour1Id},
    {"Image color 2",
            DrawableImageComponent::imageColour2Id},
    {"Image color 3",
            DrawableImageComponent::imageColour3Id},
    {"Image color 4",
            DrawableImageComponent::imageColour4Id},
            
    {"List editor text color",
            ListEditor::textColourId},
    {"List editor  background color",
            ListEditor::backgroundColourId},
    {"List editor list item color",
            ListEditor::listItemColourId},
    {"List editor selected item color",
            ListEditor::selectedListItemColourId},
            
    {"AppMenu button text color",
            AppMenuButton::textColourId},
    {"AppMenu button background color",
            AppMenuButton::backgroundColourId},
    {"AppMenu button selection color", 
            AppMenuButton::selectionColourId},
    {"AppMenu button border color",
            AppMenuButton::borderColourId},
            
    {"File picker window color",
            FileSelectTextEditor::fileWindowColourId},
    {"File picker text color",
            FileSelectTextEditor::textColourId},
            
    {"Drawable button text color",
            DrawableButton::textColourId},
    
    {"Overlay spinner background color",
            OverlaySpinner::backgroundColourId},
    {"Overlay spinner text color",
            OverlaySpinner::textColourId},
                    
    {"Text button(off) color",
            TextButton::buttonColourId},
    {"Text button(off) text color",
            TextButton::textColourOffId},
    {"Text button(on) color",
            TextButton::buttonOnColourId},
    {"Text button(on) text color",
            TextButton::textColourOnId},
                    
    {"Label background color",
            Label::backgroundColourId},
    {"Label text color",
            Label::textColourId},
    {"Label outline color",
            Label::outlineColourId},
    {"Label background color(editing)",
            Label::backgroundWhenEditingColourId},
    {"Label text color(editing)",
            Label::textColourId},
    {"Label outline color(editing)",
            Label::outlineWhenEditingColourId},
                    
    {"Slider background color",
            Slider::backgroundColourId},
    {"Slider thumb color",
            Slider::thumbColourId},
    {"Slider track color",
            Slider::trackColourId},
                    
    {"Text editor background color",
            TextEditor::backgroundColourId},
    {"Text editor text color",
            TextEditor::textColourId},
    {"Text editor highlight color",
            TextEditor::highlightColourId},
    {"Text editor highlighted text color",
            TextEditor::highlightedTextColourId},
    {"Text editor outline color",
            TextEditor::outlineColourId},
    {"Text editor focused outline color",
            TextEditor::focusedOutlineColourId},
    {"Text editor shadow color",
            TextEditor::shadowColourId},
                    
    {"Switch background color",
            SwitchComponent::backgroundColourId},
    {"Switch handle color",
            SwitchComponent::handleColourId},
    {"Switch handle color(off)",
            SwitchComponent::handleOffColourId},
};

ComponentConfigFile::ComponentConfigFile() : ConfigFile(filenameConst)
{
    const ScopedLock readLock(lock);
    File configFile = File(getHomePath() + String(CONFIG_PATH) + filename);
    var jsonConfig = JSON::parse(configFile);
    var defaultConfig = var::null;
    readDataFromJson(jsonConfig, defaultConfig);
    writeChanges();
}

ComponentConfigFile::~ComponentConfigFile()
{
}

/**
* Find a Component ColourId value from its config key String
*/
int ComponentConfigFile::getColourId(String colourKey)
{
    std::map<String,int>::const_iterator search = colourIds.find(colourKey);
    if(search == colourIds.end())
    {
        return -1;
    }
    return search->second;
}

Array<String> ComponentConfigFile::getStringKeys() const
{
    Array<String> keys;
    for(std::map<String,int>::const_iterator it = colourIds.begin();
            it != colourIds.end(); it++)
    {
        keys.add(it->first);
    }
    return keys;
}
//######################### Boolean Data ###################################
//boolean value keys
const String ComponentConfigFile::showClockKey = "show clock";
const String ComponentConfigFile::use24HrModeKey = "use 24h mode";

Array<String> ComponentConfigFile::getBoolKeys() const
{
    return {showClockKey, use24HrModeKey};
}

//######################### UI Component Data ##############################
//Defines all component types managed in the config file
const String ComponentConfigFile::appMenuButtonKey = "app menu buttons";
const String ComponentConfigFile::scrollingAppMenuKey = "scrolling app menu";
const String ComponentConfigFile::pagedAppMenuKey = "paged app menu";
const String ComponentConfigFile::menuFrameKey = "menu frame";
const String ComponentConfigFile::batteryIconKey = "battery";
const String ComponentConfigFile::batteryPercentKey = "battery percent text";
const String ComponentConfigFile::clockLabelKey = "time";
const String ComponentConfigFile::wifiIconKey = "wifi";
const String ComponentConfigFile::powerButtonKey = "power button";
const String ComponentConfigFile::settingsButtonKey = "settings button";
const String ComponentConfigFile::popupMenuKey = "popup menu";
const String ComponentConfigFile::pageLeftKey = "left arrow button";
const String ComponentConfigFile::pageRightKey = "right arrow button";

ComponentConfigFile::ComponentSettings ComponentConfigFile::getComponentSettings
(String componentKey)
{
    const ScopedLock readLock(lock);
    return components[componentKey];
}

/**
 * Read in this object's data from a json config object
 */
void ComponentConfigFile::readDataFromJson(var& config, var& defaultConfig)
{
    ConfigFile::readDataFromJson(config, defaultConfig);
    Array<String> keys = getComponentKeys();
    for (const String& key : keys)
    {
        var componentData = getProperty(config, defaultConfig, key);
        components[key] = ComponentSettings(componentData);
    }
}

/**
 * Copy all config data to a json object
 */
void ComponentConfigFile::copyDataToJson(DynamicObject::Ptr jsonObj)
{
    ConfigFile::copyDataToJson(jsonObj);
    Array<String> keys = getComponentKeys();
    for (const String& key : keys)
    {
        jsonObj->setProperty(key, components[key].getDynamicObject());
    }
}

/**
 * @return the list of all component keys.
 */
Array<String> ComponentConfigFile::getComponentKeys()
{
    return {appMenuButtonKey,
        scrollingAppMenuKey,
        pagedAppMenuKey,
        menuFrameKey,
        batteryIconKey,
        batteryPercentKey,
        clockLabelKey,
        wifiIconKey,
        powerButtonKey,
        settingsButtonKey,
        popupMenuKey,
        pageLeftKey,
        pageRightKey};
}

ComponentConfigFile::ComponentSettings::ComponentSettings() :
x(0), y(0), width(0), height(0)
{
}

//copy constructor

ComponentConfigFile::ComponentSettings::ComponentSettings
(const ComponentConfigFile::ComponentSettings& copy) :
x(copy.x), y(copy.y),
width(copy.width), height(copy.height),
assetFiles(copy.assetFiles), colours(copy.colours)
{
}

ComponentConfigFile::ComponentSettings::ComponentSettings(var jsonObj)
{
    x = jsonObj.getProperty("x", 0);
    y = jsonObj.getProperty("y", 0);
    width = jsonObj.getProperty("width", 0);
    height = jsonObj.getProperty("height", 0);

    var colourList = jsonObj["colours"];
    if (colourList.isArray())
    {
        for (var colour : *colourList.getArray())
        {
            if (colour.isString())
            {
                colours.add(Colour::fromString(colour.toString()));
            }
        }
    }
    var assetList = jsonObj["asset files"];
    if (assetList.isArray())
    {
        for (var asset : *assetList.getArray())
        {
            assetFiles.add(asset);
        }
    }
}

DynamicObject * ComponentConfigFile::ComponentSettings::getDynamicObject()
{
    DynamicObject * componentObject = new DynamicObject();
    componentObject->setProperty("x", x);
    componentObject->setProperty("y", y);
    componentObject->setProperty("width", width);
    componentObject->setProperty("height", height);
    Array<var> coloursListed;
    for (int i = 0; i < colours.size(); i++)
    {
        coloursListed.add(var(colours[i].toString()));
    }
    componentObject->setProperty("colours", coloursListed);

    Array<var> assetFilesListed;
    for (int i = 0; i < assetFiles.size(); i++)
    {
        assetFilesListed.add(var(assetFiles[i]));
    }
    componentObject->setProperty("asset files", assetFilesListed);
    return componentObject;
}

bool ComponentConfigFile::ComponentSettings::operator==
(const ComponentSettings& rhs) const
{
    return x == rhs.x &&
            y == rhs.y &&
            width == rhs.width &&
            height == rhs.height &&
            colours == rhs.colours &&
            assetFiles == rhs.assetFiles;
}

Rectangle<int> ComponentConfigFile::ComponentSettings::getBounds()
{
    Rectangle<int>window = getWindowSize();
    return Rectangle<int>(x * window.getWidth(), y * window.getHeight(),
            width * window.getWidth(), height * window.getHeight());
}

Array<Colour> ComponentConfigFile::ComponentSettings::getColours()
{
    return colours;
}

Array<String> ComponentConfigFile::ComponentSettings::getAssetFiles()
{
    return assetFiles;
}

void ComponentConfigFile::ComponentSettings::applyBounds(Component * component)
{
    component->setBounds(getBounds());
}

void ComponentConfigFile::ComponentSettings::applySize(Component * component)
{
    Rectangle<int>screen = getWindowSize();
    component->setSize(width * screen.getWidth(), height * screen.getHeight());
}

