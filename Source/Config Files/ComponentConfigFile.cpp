#include "ColourIds.h"
#include "ComponentConfigFile.h"
#include "AssetFiles.h"
#include "Utils.h"

ScopedPointer<ResourceManager::SharedResource>
        ComponentConfigFile::sharedResource = nullptr;

ReadWriteLock ComponentConfigFile::configLock;

const StringArray ComponentConfigFile::defaultColourKeys = 
{
    "window background",
    "widget background",
    "widget(off)",
    "widget(on)",
    "menu background",
    "outline",
    "focused outline",
    "text field",
    "text",
    "highlighted text field",
    "highlighted text"
};

const std::map<String, int> ComponentConfigFile::colourIds
{
    {
     "Page background",
     ColourIds::pageComponent::background
    },
    {
     "Image0",
     ColourIds::drawableImageComponent::image0
    },
    {
     "Image1",
     ColourIds::drawableImageComponent::image1
    },
    {
     "Image2",
     ColourIds::drawableImageComponent::image2
    },
    {
     "Image3",
     ColourIds::drawableImageComponent::image3
    },
    {
     "Image4",
     ColourIds::drawableImageComponent::image4
    },
    {
     "AppMenu button text",
     ColourIds::appMenuButton::text
    },
    {
     "AppMenu button background",
     ColourIds::appMenuButton::background
    },
    {
     "AppMenu button selection",
     ColourIds::appMenuButton::selection
    },
    {
     "AppMenu button border",
     ColourIds::appMenuButton::border
    },
    {
     "File picker window",
     ColourIds::fileSelectTextEditor::fileWindow
    },
    {
     "File picker text",
     ColourIds::fileSelectTextEditor::text
    },
    {
     "Overlay spinner background",
     ColourIds::overlaySpinner::background
    },
    {
     "Overlay spinner text",
     ColourIds::overlaySpinner::text
    },
    {
     "Text button(off)",
     ColourIds::textButton::button
    },
    {
     "Text button(off) text",
     ColourIds::textButton::textOff
    },
    {
     "Text button(on)",
     ColourIds::textButton::buttonOn
    },
    {
     "Text button(on) text",
     ColourIds::textButton::textOn
    },
    {
     "Slider background",
     ColourIds::slider::background
    },
    {
     "Slider thumb",
     ColourIds::slider::thumb
    },
    {
     "Slider track",
     ColourIds::slider::track
    },
    {
     "Switch background",
     ColourIds::switchComponent::background
    },
    {
     "Switch handle",
     ColourIds::switchComponent::handle
    },
    {
     "Switch handle(off)",
     ColourIds::switchComponent::handleOff
    },
    {
     "Combo box text",
     ColourIds::comboBox::text
    },
    {
     "Combo box background",
     ColourIds::comboBox::background
    },
    {
     "Combo box outline",
     ColourIds::comboBox::outline
    },
    {
     "Combo box button",
     ColourIds::comboBox::button
    },
    {
     "Combo box arrow",
     ColourIds::comboBox::arrow
    },
    {
     "Alert window background",
     ColourIds::alertWindow::background
    },
    {
     "Alert window text",
     ColourIds::alertWindow::text
    },
    {
     "Alert window outline",
     ColourIds::alertWindow::outline
    }};

ComponentConfigFile::ComponentConfigFile() :
ResourceManager(sharedResource, configLock,
[this]()->ResourceManager::SharedResource*
{
    return new ConfigJson();
}) { }

ComponentConfigFile::DefaultColour ComponentConfigFile::getColourType
(String colourKey)
{
    int enumVal = defaultColourKeys.indexOf(colourKey);
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
    const ScopedReadLock readLock(configLock);
    ConfigJson* config = static_cast<ConfigJson*> (sharedResource.get());
    return Colour(config->getConfigValue<String>(getColourKey(colourType))
		    .getHexValue32());
}

Array<int> ComponentConfigFile::getColourIds
(ComponentConfigFile::DefaultColour colourType)
{
    using namespace ColourIds;
    switch(colourType)
    {
    case windowBackground:
        return
        {
            tabbedComponent::background,
            sidePanel::background,
            treeView::background,
            tooltipWindow::background,
            alertWindow::background,
            resizableWindow::background,
            colourSelector::background,
            keyMappingEditorComponent::background,
            jucer_CommonHeaders::background,
            pageComponent::background,
            jucer_CommonHeaders::contentHeaderBackground,
            fileSelectTextEditor::fileWindow,
            jucer_CommonHeaders::inactiveTabBackground
        };

    case widgetBackground:
        return
        {
            scrollBar::background,
            progressBar::background,
            slider::background,
            drawableButton::background,
            overlaySpinner::background,
            switchComponent::background,
            appMenuButton::background,
            drawableButton::backgroundOn,
            textButton::button,
            toolbar::buttonMouseOverBackground,
            fileBrowserComponent::currentPathBoxBackground,
            jucer_CommonHeaders::defaultButtonBackground,
            jucer_CommonHeaders::secondaryBackground,
            jucer_CommonHeaders::userButtonBackground,
            jucer_CommonHeaders::widgetBackground
        };

    case widgetOff:
        return
        {
            comboBox::button,
            jucer_CommonHeaders::defaultIcon,
            sidePanel::dismissButtonDown,
            switchComponent::handleOff,
            jucer_CommonHeaders::inactiveTabIcon,
            toggleButton::tickDisabled,
            scrollBar::track,
            slider::track
        };

    case widgetOn:
        return
        {
            jucer_CommonHeaders::activeTabIcon,
            toolbar::buttonMouseDownBackground,
            sidePanel::dismissButtonNormal,
            sidePanel::dismissButtonOver,
            progressBar::foreground,
            switchComponent::handle,
            slider::rotarySliderFill,
            scrollBar::thumb,
            slider::thumb,
            toggleButton::tick,
            jucer_CommonHeaders::treeIcon
        };

    case menuBackground:
        return
        {
            bubbleComponent::background,
            fileSearchPathListComponent::background,
            propertyComponent::background,
            comboBox::background,
            tableHeaderComponent::background,
            listBox::background,
            toolbar::background,
            popupMenu::background,
            listEditor::background,
            jucer_CommonHeaders::secondaryButtonBackground
        };

    case outline:
        return
        {
            appMenuButton::border,
            toolbar::editingModeOutline,
            tabbedButtonBar::frontOutline,
            lassoComponent::lassoOutline,
            bubbleComponent::outline,
            textPropertyComponent::outline,
            booleanPropertyComponent::outline,
            groupComponent::outline,
            tabbedComponent::outline,
            comboBox::outline,
            textEditor::outline,
            tableHeaderComponent::outline,
            listBox::outline,
            tooltipWindow::outline,
            alertWindow::outline,
            slider::rotarySliderOutline,
            tabbedButtonBar::tabOutline,
            slider::textBoxOutline
        };

    case focusedOutline:
        return
        {
            comboBox::focusedOutline,
            textEditor::focusedOutline,
            label::outlineWhenEditing,
            toolbar::separator
        };

    case textField:
        return
        {
            textPropertyComponent::background,
            booleanPropertyComponent::background,
            textEditor::background,
            codeEditorComponent::background,
            label::backgroundWhenEditing,
            fileBrowserComponent::filenameBoxBackground,
            listEditor::listItem,
            jucer_CommonHeaders::secondaryWidgetBackground,
            slider::textBoxBackground
        };

    case text:
        return
        {
            comboBox::arrow,
            fileBrowserComponent::currentPathBoxArrow,
            fileBrowserComponent::currentPathBoxText,
            codeEditorComponent::defaultText,
            jucer_CommonHeaders::defaultText,
            treeView::dragAndDropIndicator,
            treeView::evenItems,
            fileBrowserComponent::filenameBoxText,
            tabbedButtonBar::frontText,
            popupMenu::headerText,
            propertyComponent::labelText,
            toolbar::labelText,
            colourSelector::labelText,
            lassoComponent::lassoFill,
            treeView::lines,
            treeView::oddItems,
            tabbedButtonBar::tabText,
            directoryContentsDisplayComponent::text,
            textPropertyComponent::text,
            groupComponent::text,
            comboBox::text,
            textEditor::text,
            label::text,
            tableHeaderComponent::text,
            listBox::text,
            tooltipWindow::text,
            alertWindow::text,
            documentWindow::text,
            popupMenu::text,
            toggleButton::text,
            hyperlinkButton::text,
            drawableButton::text,
            keyMappingEditorComponent::text,
            overlaySpinner::text,
            fileSelectTextEditor::text,
            listEditor::text,
            appMenuButton::text,
            slider::textBoxText,
            textButton::textOff,
            textButton::textOn,
            drawableButton::textOn,
            label::textWhenEditing,
            fileChooserDialogBox::titleText,
            sidePanel::titleText,
            jucer_CommonHeaders::widgetText
        };

    case highlightedTextField:
        return
        {
            textButton::buttonOn,
            jucer_CommonHeaders::defaultHighlight,
            directoryContentsDisplayComponent::highlight,
            textEditor::highlight,
            tableHeaderComponent::highlight,
            codeEditorComponent::highlight,
            popupMenu::highlightedBackground,
            codeEditorComponent::lineNumberBackground,
            treeView::selectedItemBackground,
            listEditor::selectedListItem,
            appMenuButton::selection,
            slider::textBoxHighlight
        };

    case highlightedText:
        return
        {
            caretComponent::caret,
            jucer_CommonHeaders::codeEditorLineNumber,
            jucer_CommonHeaders::defaultHighlightedText,
            directoryContentsDisplayComponent::highlightedText,
            textEditor::highlightedText,
            popupMenu::highlightedText,
            codeEditorComponent::lineNumberText
        };
    default:
        return {};
    }

}


void ComponentConfigFile::setColour
(DefaultColour colourType, Colour newColour)
{
    if(colourType != none)
    {
        const ScopedWriteLock writeLock(configLock);
        ConfigJson* config = static_cast<ConfigJson*> (sharedResource.get());
	config->setConfigValue<String>
		(getColourKey(colourType),newColour.toString());
    }
}

void ComponentConfigFile::setColour(String key, Colour newColour)
{
    const ScopedWriteLock writeLock(configLock);
    ConfigJson* config = static_cast<ConfigJson*> (sharedResource.get());
    config->setConfigValue<String>(key,newColour.toString());
}

/*
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
    //DBG("Found "<<keys.size()<<" color keys");
    return keys;
}

/*
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

/*
 * Get a color value from its String key.
 */
Colour ComponentConfigFile::getColour(String colourKey)
{
    const ScopedReadLock readLock(configLock);
    ConfigJson* config = static_cast<ConfigJson*> (sharedResource.get());
    return Colour(config->getConfigValue<String>(colourKey).getHexValue32());
}

/*
 * Gets configured component settings from shared .json file data.
 */
ComponentConfigFile::ComponentSettings
ComponentConfigFile::getComponentSettings(String componentKey)
{
    const ScopedReadLock readLock(configLock);
    ConfigJson* config = static_cast<ConfigJson*> (sharedResource.get());
    return config->getComponentSettings(componentKey);
}

/*
 * Add a listener to track component setting changes.
 */
void ComponentConfigFile::addListener(ConfigFile::Listener* listener,
        StringArray trackedKeys)
{
    const ScopedWriteLock writeLock(configLock);
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

/*
 * Return the most appropriate font size for drawing text
 */
int ComponentConfigFile::getFontHeight(Rectangle <int> textBounds, String text)
{
    const ScopedReadLock readLock(configLock);
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

/*
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

/*
 * Gets the configured settings for a particular component.
 */
ComponentConfigFile::ComponentSettings
ComponentConfigFile::ConfigJson::getComponentSettings(String componentKey)
{
    return components[componentKey];
}

/*
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
        keys.push_back({key,stringType});
    }
    for (auto it = colourIds.begin(); it != colourIds.end(); it++)
    {
        keys.push_back({it->first, stringType});
    }
    return keys;
}

/*
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

/*
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

/*
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

/*
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

/*
 * @return the bounds of the component relative to the window,
 * measured in pixels.
 */
Rectangle<int> ComponentConfigFile::ComponentSettings::getBounds()
{
    Rectangle<int> window = getWindowBounds();
    return Rectangle<int>(
                          x * window.getWidth(),
                          y * window.getHeight(),
                          width * window.getWidth(),
                          height * window.getHeight());
}

/*
 * @return the list of configurable colors.
 */
Array<Colour> ComponentConfigFile::ComponentSettings::getColours()
{
    return colours;
}

/*
 * @return the list of component asset files.
 */
StringArray ComponentConfigFile::ComponentSettings::getAssetFiles()
{
    return assetFiles;
}

/*
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

