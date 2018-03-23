#include "DrawableImageComponent.h"
#include "ListEditor.h"
#include "FileSelectTextEditor.h"
#include "OverlaySpinner.h"
#include "SwitchComponent.h"
#include "AppMenuButton.h"
#include "PageComponent.h"
#include "Display.h"
#include "ComponentConfigFile.h"
#include "AssetFiles.h"
#include "Utils.h"

CriticalSection ComponentConfigFile::componentLock;

std::map<String, ComponentConfigFile::ComponentSettings>
ComponentConfigFile::components;

const std::map<String, int> ComponentConfigFile::colourIds{
    {"Page background color",
     PageComponent::backgroundColourId},

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
     SwitchComponent::handleOffColourId},};

ComponentConfigFile::ComponentConfigFile() : ConfigFile(filenameConst)
{
    ASSERT_SINGULAR;
    const ScopedLock readLock(componentLock);
    var jsonConfig = AssetFiles::loadJSONAsset
            (String(configPath) + filenameConst, true);
    var defaultConfig = var();
    readDataFromJson(jsonConfig, defaultConfig);
    writeChanges();
}

ComponentConfigFile::~ComponentConfigFile() { }

/**
 * Find a Component ColourId value from its config key String
 */
int ComponentConfigFile::getColourId(String colourKey)
{
    std::map<String, int>::const_iterator search = colourIds.find(colourKey);
    if (search == colourIds.end())
    {
        return -1;
    }
    return search->second;
}

/**
 * @return the keys to all Component color settings stored in
 * components.json
 */
StringArray ComponentConfigFile::getColourKeys() const
{
    StringArray keys;
    for (std::map<String, int>::const_iterator it = colourIds.begin();
         it != colourIds.end(); it++)
    {
        keys.add(it->first);
    }
    return keys;
}


//##### boolean value keys: ######
const String ComponentConfigFile::showClockKey = "show clock";
const String ComponentConfigFile::use24HrModeKey = "use 24h mode";


//######################### UI Component Data ##############################
//Defines all component types managed in the config file
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
const String ComponentConfigFile::pageUpKey = "up arrow button";
const String ComponentConfigFile::pageDownKey = "down arrow button";
const String ComponentConfigFile::smallTextKey = "small text";
const String ComponentConfigFile::mediumTextKey = "medium text";
const String ComponentConfigFile::largeTextKey = "large text";

/**
 * Return the most appropriate font size for drawing text
 */
int ComponentConfigFile::getFontHeight(Rectangle <int> textBounds, String text)
{
    int heightLarge = getComponentSettings(largeTextKey).getBounds()
            .getHeight();
    int heightMedium = getComponentSettings(mediumTextKey).getBounds()
            .getHeight();
    int heightSmall = getComponentSettings(smallTextKey).getBounds()
            .getHeight();
    int numLines = 1;
    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] == '\n')
        {
            numLines++;
        }
    }

    int height = textBounds.getHeight() / numLines;
    Font defaultFont(height);
    int width = defaultFont.getStringWidth(text);
    if (width > textBounds.getWidth())
    {
        height = textBounds.getWidth() * height / width;
    }
    if (height > heightLarge)
    {
        return heightLarge;
    }
    else if (height > heightMedium)
    {
        return heightMedium;
    }
    else if (height > heightSmall)
    {
        return heightSmall;
    }
    return height;
}

ComponentConfigFile::ComponentSettings ComponentConfigFile::getComponentSettings
(String componentKey)
{
    const ScopedLock readLock(componentLock);
    return components[componentKey];
}

/**
 * @return the list of all component keys.
 */
StringArray ComponentConfigFile::getComponentKeys()
{
    return {
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
            pageRightKey,
            pageUpKey,
            pageDownKey,
            smallTextKey,
            mediumTextKey,
            largeTextKey};
}

/**
 * @return the list of key Strings for each integer value tracked in 
 * components.json
 */
std::vector<ConfigFile::DataKey> ComponentConfigFile::getDataKeys() const
{
    std::vector<DataKey> keys = {
        {showClockKey, boolType},
        {use24HrModeKey, boolType}
    };
    StringArray colourKeys = getColourKeys();
    for (const String& colourKey : colourKeys)
    {
        keys.push_back({colourKey, stringType});
    }
    return keys;
}

/**
 * Read in this object's data from a json config object
 */
void ComponentConfigFile::readDataFromJson(var& config, var& defaultConfig)
{
    ConfigFile::readDataFromJson(config, defaultConfig);
    StringArray keys = getComponentKeys();
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
    StringArray keys = getComponentKeys();
    for (const String& key : keys)
    {
        jsonObj->setProperty(key, components[key].getDynamicObject());
    }
}

ComponentConfigFile::ComponentSettings::ComponentSettings() :
x(0), y(0), width(0), height(0) { }

ComponentConfigFile::ComponentSettings::ComponentSettings(var jsonObj)
{
    x = jsonObj.getProperty("x", -1);
    y = jsonObj.getProperty("y", -1);
    width = jsonObj.getProperty("width", -1);
    height = jsonObj.getProperty("height", -1);

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
    if (x != -1)
    {
        componentObject->setProperty("x", x);
    }
    if (y != -1)
    {
        componentObject->setProperty("y", y);
    }
    if (width != -1)
    {
        componentObject->setProperty("width", width);
    }
    if (height != -1)
    {
        componentObject->setProperty("height", height);
    }
    if (colours.size() > 0)
    {
        Array<var> coloursListed;
        for (int i = 0; i < colours.size(); i++)
        {
            coloursListed.add(var(colours[i].toString()));
        }
        componentObject->setProperty("colours", coloursListed);
    }

    if (assetFiles.size() > 0)
    {
        Array<var> assetFilesListed;
        for (int i = 0; i < assetFiles.size(); i++)
        {
            assetFilesListed.add(var(assetFiles[i]));
        }
        componentObject->setProperty("asset files", assetFilesListed);
    }
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
    Rectangle<int> window = Display::getWindowSize();
    return Rectangle<int>(x * window.getWidth(), y * window.getHeight(),
                          width * window.getWidth(), height * window.getHeight());
}

Array<Colour> ComponentConfigFile::ComponentSettings::getColours()
{
    return colours;
}

StringArray ComponentConfigFile::ComponentSettings::getAssetFiles()
{
    return assetFiles;
}

void ComponentConfigFile::ComponentSettings::applyBounds(Component * component)
{
    Rectangle<int> bounds = getBounds();
    if (x == -1)
    {
        bounds.setX(component->getX());
    }
    if (y == -1)
    {
        bounds.setY(component->getY());
    }
    if (width == -1)
    {
        bounds.setWidth(component->getWidth());
    }
    if (height == -1)
    {
        bounds.setHeight(component->getHeight());
    }

    component->setBounds(bounds);
}

