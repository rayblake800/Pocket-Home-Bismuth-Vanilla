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

ScopedPointer<ResourceManager::SharedResource>
        ComponentConfigFile::sharedResource = nullptr;

CriticalSection ComponentConfigFile::configLock;

const StringArray ComponentConfigFile::defaultColourKeys = 
{
    "window background color",
    "widget background color",
    "widget color (off)",
    "widget color (on)",
    "menu background color",
    "outline color",
    "focused outline color",
    "text field color",
    "text color",
    "highlighted text field color",
    "highlighted text color"
};

const std::map<String, int> ComponentConfigFile::colourIds
{
    {
     "Page background color",
     PageComponent::backgroundColourId
    },

    {
     "Image color 0",
     DrawableImageComponent::imageColour0Id
    },
    {
     "Image color 1",
     DrawableImageComponent::imageColour1Id
    },
    {
     "Image color 2",
     DrawableImageComponent::imageColour2Id
    },
    {
     "Image color 3",
     DrawableImageComponent::imageColour3Id
    },
    {
     "Image color 4",
     DrawableImageComponent::imageColour4Id
    },

    {
     "Image color 4",
     DrawableImageComponent::imageColour4Id
    },

    {
     "List editor text color",
     ListEditor::textColourId
    },
    {
     "List editor  background color",
     ListEditor::backgroundColourId
    },
    {
     "List editor list item color",
     ListEditor::listItemColourId
    },
    {
     "List editor selected item color",
     ListEditor::selectedListItemColourId
    },

    {
     "AppMenu button text color",
     AppMenuButton::textColourId
    },
    {
     "AppMenu button background color",
     AppMenuButton::backgroundColourId
    },
    {
     "AppMenu button selection color",
     AppMenuButton::selectionColourId
    },
    {
     "AppMenu button border color",
     AppMenuButton::borderColourId
    },

    {
     "File picker window color",
     FileSelectTextEditor::fileWindowColourId
    },
    {
     "File picker text color",
     FileSelectTextEditor::textColourId
    },

    {
     "Drawable button text color",
     DrawableButton::textColourId
    },

    {
     "Overlay spinner background color",
     OverlaySpinner::backgroundColourId
    },
    {
     "Overlay spinner text color",
     OverlaySpinner::textColourId
    },

    {
     "Text button(off) color",
     TextButton::buttonColourId
    },
    {
     "Text button(off) text color",
     TextButton::textColourOffId
    },
    {
     "Text button(on) color",
     TextButton::buttonOnColourId
    },
    {
     "Text button(on) text color",
     TextButton::textColourOnId
    },

    {
     "Label background color",
     Label::backgroundColourId
    },
    {
     "Label text color",
     Label::textColourId
    },
    {
     "Label outline color",
     Label::outlineColourId
    },
    {
     "Label background color(editing)",
     Label::backgroundWhenEditingColourId
    },
    {
     "Label text color(editing)",
     Label::textColourId
    },
    {
     "Label outline color(editing)",
     Label::outlineWhenEditingColourId
    },

    {
     "Slider background color",
     Slider::backgroundColourId
    },
    {
     "Slider thumb color",
     Slider::thumbColourId
    },
    {
     "Slider track color",
     Slider::trackColourId
    },

    {
     "Text editor background color",
     TextEditor::backgroundColourId
    },
    {
     "Text editor text color",
     TextEditor::textColourId
    },
    {
     "Text editor highlight color",
     TextEditor::highlightColourId
    },
    {
     "Text editor highlighted text color",
     TextEditor::highlightedTextColourId
    },
    {
     "Text editor outline color",
     TextEditor::outlineColourId
    },
    {
     "Text editor focused outline color",
     TextEditor::focusedOutlineColourId
    },
    {
     "Text editor shadow color",
     TextEditor::shadowColourId
    },

    {
     "Switch background color",
     SwitchComponent::backgroundColourId
    },
    {
     "Switch handle color",
     SwitchComponent::handleColourId
    },
    {
     "Switch handle color(off)",
     SwitchComponent::handleOffColourId
    },

    {
     "Pop-up menu text color",
     PopupMenu::textColourId
    },
    {
     "Pop-up menu header text color",
     PopupMenu::headerTextColourId
    },
    {
     "Pop-up menu background color",
     PopupMenu::backgroundColourId
    },
    {
     "Pop-up menu highlighted text color",
     PopupMenu::highlightedTextColourId
    },


    {
     "Combo box text color",
     ComboBox::textColourId
    },
    {
     "Combo box background color",
     ComboBox::backgroundColourId
    },
    {
     "Combo box outline color",
     ComboBox::outlineColourId
    },
    {
     "Combo box button color",
     ComboBox::buttonColourId
    },
    {
     "Combo box arrow color",
     ComboBox::arrowColourId
    },


    {
     "Alert window background color",
     AlertWindow::backgroundColourId
    },
    {
     "Alert window text color",
     AlertWindow::textColourId
    },
    {
     "Alert window outline color",
     AlertWindow::outlineColourId
    }};

ComponentConfigFile::ComponentConfigFile() :
ResourceManager(sharedResource, configLock,
[this]()->ResourceManager::SharedResource*
{
    return new ConfigJson();
}) { }

ComponentConfigFile::DefaultColour ComponentConfigFile::getColourType
(String colorKey)
{
    int enumVal = defaultColourKeys.indexOf(colorkey);
    if(enumVal < 0)
    {
        return none;
    }
    return (DefaultColour) enumVal; 
}

String ComponentConfigFile::getColourKey(DefaultColour colour)
{
    if(colour == none)
    {
        return String();
    }
    return defaultColourKeys[(int) colour];
}

Colour ComponentConfigFile::getColour(DefaultColour colourType)
{
    if(colourType == none)
    {
        return Colour();
    }
    const ScopedLock readLock(configLock);
    ConfigJson* config = static_cast<ConfigJson*> (sharedResource.get());
    return config->getConfigValue<String>(getColourKey(colourType));
}

void ComponentConfigFile::setColour
(DefaultColour colourType, Colour newColour)
{
    if(colourType != none)
    {
        const ScopedLock readLock(configLock);
        ConfigJson* config = static_cast<ConfigJson*> (sharedResource.get());
	config->setConfigValue<String>
		(getColourKey(colourType),newColour.toString());
    }
}

void ComponentConfigFile::setColour(String key, Colour newColour)
{
    const ScopedLock readLock(configLock);
    ConfigJson* config = static_cast<ConfigJson*> (sharedResource.get());
    config->setConfigValue<String>(key,newColour.toString());
}

/**
 * @return the keys to all Component color settings stored in
 *          components.json
 */
StringArray ComponentConfigFile::getColourKeys() const
{
    StringArray keys;
    for (auto it = colourIds.begin(); it != colourIds.end(); it++)
    {
        keys.add(it->first);
    }
    DBG("Found "<<keys.size()<<" color keys");
    return keys;
}

/**
 * Find a Component ColourId value from its config key String
 */
int ComponentConfigFile::getColourId(String colourKey)
{
    auto searchIter = colourIds.find(colourKey);
    if (searchIter == colourIds.end())
    {
        return -1;
    }
    return searchIter->second;
}

/**
 * Get a color value from its String key.
 *  
 * @param colourKey the key for a color value in colourIds
 * @return the corresponding Colour value, or Colour() if colourKey wasn't
 * found.
 */
Colour ComponentConfigFile::getColour(String colourKey)
{
    const ScopedLock readLock(configLock);
    ConfigJson* config = static_cast<ConfigJson*> (sharedResource.get());
    return Colour(config->getConfigValue<String>(colourKey).getHexValue32());
}

/**
 * Gets configured component settings from shared .json file data.
 */
ComponentConfigFile::ComponentSettings
ComponentConfigFile::getComponentSettings(String componentKey)
{
    const ScopedLock readLock(configLock);
    ConfigJson* config = static_cast<ConfigJson*> (sharedResource.get());
    return config->getComponentSettings(componentKey);
}

/**
 * Add a listener to track component setting changes.
 */
void ComponentConfigFile::addListener(ConfigFile::Listener* listener,
        StringArray trackedKeys)
{
    const ScopedLock readLock(configLock);
    ConfigJson* config = static_cast<ConfigJson*> (sharedResource.get());
    config->addListener(listener, trackedKeys);
}

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
const String ComponentConfigFile::spinnerKey = "loading spinner";
const String ComponentConfigFile::smallTextKey = "small text";
const String ComponentConfigFile::mediumTextKey = "medium text";
const String ComponentConfigFile::largeTextKey = "large text";

/**
 * Return the most appropriate font size for drawing text
 */
int ComponentConfigFile::getFontHeight(Rectangle <int> textBounds, String text)
{
    const ScopedLock readLock(configLock);
    ConfigJson* config = static_cast<ConfigJson*> (sharedResource.get());
    int heightLarge = config->getComponentSettings(largeTextKey).getBounds()
            .getHeight();
    int heightMedium = config->getComponentSettings(mediumTextKey).getBounds()
            .getHeight();
    int heightSmall = config->getComponentSettings(smallTextKey).getBounds()
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
            spinnerKey,
            smallTextKey,
            mediumTextKey,
            largeTextKey};
}

ComponentConfigFile::ConfigJson::ConfigJson() : ConfigFile(filenameConst)
{
    var jsonConfig = AssetFiles::loadJSONAsset(String(configPath)
            + filenameConst, true);
    var defaultConfig = var();
    readDataFromJson(jsonConfig, defaultConfig);
    writeChanges();
}

/**
 * Gets the configured settings for a particular component.
 */
ComponentConfigFile::ComponentSettings
ComponentConfigFile::ConfigJson::getComponentSettings(String componentKey)
{
    return components[componentKey];
}

/**
 * @return the list of key Strings for each integer value tracked in 
 * components.json
 */
std::vector<ConfigFile::DataKey> ComponentConfigFile::ConfigJson
::getDataKeys() const
{
    std::vector<DataKey> keys = {
    };
    for(const String& key : defaultColourKeys)
    {
        keys.push_back({key,StringType});
    }
    for (auto it = colourIds.begin(); it != colourIds.end(); it++)
    {
        keys.push_back({it->first, stringType});
    }
    return keys;
}

/**
 * Read in this object's data from a json config object
 */
void ComponentConfigFile::ConfigJson::readDataFromJson
(var& config, var& defaultConfig)
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
void ComponentConfigFile::ConfigJson::copyDataToJson
(DynamicObject::Ptr jsonObj)
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

/**
 * Initializes from json data.
 */
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

/**
 * Packages the object into a DynamicObject that can be written to a
 * json file.
 */
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

/**
 * @return the bounds of the component relative to the window,
 * measured in pixels.
 */
Rectangle<int> ComponentConfigFile::ComponentSettings::getBounds()
{
    Rectangle<int> window = Display::getWindowSize();
    return Rectangle<int>(
                          x * window.getWidth(),
                          y * window.getHeight(),
                          width * window.getWidth(),
                          height * window.getHeight());
}

/**
 * @return the list of configurable colors.
 */
Array<Colour> ComponentConfigFile::ComponentSettings::getColours()
{
    return colours;
}

/**
 * @return the list of component asset files.
 */
StringArray ComponentConfigFile::ComponentSettings::getAssetFiles()
{
    return assetFiles;
}

/**
 * Use these settings to position and size a component.
 */
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

