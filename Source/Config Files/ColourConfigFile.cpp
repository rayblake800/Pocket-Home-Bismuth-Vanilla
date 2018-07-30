#include "ColourIds.h"
#include "ColourConfigFile.h"
#include "AssetFiles.h"
#include "Utils.h"

juce::ScopedPointer<ResourceManager::SharedResource>
        ColourConfigFile::sharedResource = nullptr;

juce::ReadWriteLock ColourConfigFile::configLock;

ColourConfigFile::ColourConfigFile() :
ResourceManager(sharedResource, configLock,
[this]()->ResourceManager::SharedResource*
{
    return new ConfigJson();
}) { }

/*
 * Look up the Colour value saved for a specific Juce ColourId.
 */
juce::Colour ColourConfigFile::getColour(int colourId) const
{
    using namespace juce;
    String colourKey = getColourKey(colourId);
    String colourStr = getColourString(colourKey);
    if(colourStr.isEmpty())
    {
        //No specific value set, look up the UICategory value.
        colourKey = getCategoryKey(getUICategory(colourId));
        colourStr = getColourString(colourKey);
    }
    return Colour(colourStr.getHexValue32());
}

/*
 * Gets the Colour value assigned as the default for all UI items in a
 * specific category.
 */
juce::Colour ColourConfigFile::getColour(UICategory category) const
{
    using namespace juce;
    if(category == none)
    {
        return Colour();
    }
    return getColour(getCategoryKey(category));
}
 
/**
 * Gets the colour value associated with a particular key string.
 */
juce::Colour ColourConfigFile::getColour(juce::String colourKey) const
{
    using namespace juce;
    return Colour(getColourString(colourKey).getHexValue32());   
}

/**
 * Sets the saved colour value for a single UI element.
 */
void ColourConfigFile::setColour(int colourId, juce::Colour newColour)
{
    using namespace juce;
    String colourKey = getColourKey(colourId);
    if(colourKey.isNotEmpty())
    {
        setColour(colourKey, newColour);
    }
}

/**
 * Sets the saved colour value for a category of UI elements.
 */
void ColourConfigFile::setColour(UICategory category, juce::Colour newColour)
{
    if(category != none)
    {
        setColour(getCategoryKey(category), newColour);
    }
}
 
/*
 * Sets the saved colour value for a specific key string.
 */
void ColourConfigFile::setColour(juce::String colourKey, juce::Colour newColour)
{
    using namespace juce;
    const ScopedWriteLock writeLock(configLock);
        ConfigJson* config = static_cast<ConfigJson*> (sharedResource.get());
	config->setConfigValue<String>(colourKey, newColour.toString());
}
   
/*
 * Gets all Juce ColourId values defined by the colour config file.
 */
juce::Array<int> ColourConfigFile::getColourIds()
{
    using namespace juce;
    Array<int> colourIds;
    for(const auto& iter : idCategories)
    {
        colourIds.add(iter.first);
    }
    for(const auto& iter : colourIdKeys)
    {
        colourIds.addIfNotAlreadyThere(iter.first);
    }
    return colourIds;
}
  
/*
 * Gets all key strings used by the ColourConfigFile.
 */
juce::StringArray ColourConfigFile::getColourKeys()
{
    using namespace juce;
    StringArray colourKeys = uiCategoryKeys;
    for(const auto& colourId : colourIdKeys)
    {
        colourKeys.add(colourId.second);
    }
    return colourKeys;
}

/*
 * Calls colourValueChanged for each Juce ColourId associated with
 * the updated value's key.
 */
void ColourConfigFile::Listener::configValueChanged(juce::String propertyKey)
{
    using namespace juce;
    ColourConfigFile config;
    String colourString;
    try
    {
        colourString = config.getColourString(propertyKey);
    }
    catch(std::out_of_range e)
    {
        nonColorValueChanged(propertyKey);
        return;
    }
    if(colourString.isEmpty())
    {
        return;
    }
    Colour newColour = Colour(colourString.getHexValue32());
    
    int colourId = getColourId(propertyKey);
    if(colourId >= 0)
    {
        colourValueChanged(colourId, propertyKey, newColour);
    }
    else
    {
        UICategory category = getCategoryType(propertyKey);
        if(category == UICategory::none)
        {
            return;
        }
        
        for(const int& trackedId : trackedColourIds)
        {
            if(getUICategory(trackedId) == category)
            {
                String idKey = getColourKey(trackedId);
                if(idKey.isEmpty() || config.getColourString(idKey).isEmpty())
                {
                    colourValueChanged(trackedId, propertyKey, newColour);
                }
            }
        }      
    }
}

/*
 * Adds a listener to track color setting changes.
 */
void ColourConfigFile::addListener(ColourConfigFile::Listener * listener,
        juce::Array<int> trackedIds)
{
    using namespace juce;
    listener->trackedColourIds = trackedIds;
    StringArray colourKeys;
    for(const int& colourId : trackedIds)
    {
        String colourKey = getColourKey(colourId);
        if(colourKey.isNotEmpty())
        {
            colourKeys.addIfNotAlreadyThere(colourKey);
        }
        UICategory idCategory = getUICategory(colourId);
        colourKeys.addIfNotAlreadyThere(getCategoryKey(idCategory));
    }
    const ScopedWriteLock writeLock(configLock);
    ConfigJson* config = static_cast<ConfigJson*> (sharedResource.get());
    config->addListener(listener, colourKeys);
}

/*
 * Gets a Colour string saved to the colour config file.
 */
juce::String ColourConfigFile::getColourString(juce::String colourKey) const
{
    using namespace juce;
    const ScopedReadLock readLock(configLock);
    ConfigJson* config = static_cast<ConfigJson*> (sharedResource.get());
    return config->getConfigValue<String>(colourKey);
}
 
/*
 * Gets the UICategory assigned to a Juce ColourId value. 
 */
ColourConfigFile::UICategory 
ColourConfigFile::getUICategory(int colourId)
{
    auto searchIter = idCategories.find(colourId);
    if (searchIter == idCategories.end())
    {
        return UICategory::none;
    }
    return searchIter->second;
}



/*
 * Finds the Juce ColourId value of a UI element from its key string.
 */
int ColourConfigFile::getColourId(juce::String colourKey)
{
    auto searchIter = colourIds.find(colourKey);
    if (searchIter == colourIds.end())
    {
        return -1;
    }
    return searchIter->second;
}

/*
 * Finds the UICategory type represented by a specific key string.
 */
ColourConfigFile::UICategory ColourConfigFile::getCategoryType
(juce::String categoryKey)
{
    int enumVal = uiCategoryKeys.indexOf(categoryKey);
    if(enumVal < 0)
    {
        return none;
    }
    return (UICategory) enumVal; 
}

/*
 * Gets the key string used to store a specific Juce ColourId in the
 * colour config file.
 */
juce::String ColourConfigFile::getColourKey(int colourId)
{
    using namespace juce;
    auto keySearch = colourIdKeys.find(colourId);
    if(keySearch == colourIdKeys.end())
    {
        return String();
    }
    return keySearch->second;
}

/*
 * Finds the key string representing a UICategory value.
 */
juce::String ColourConfigFile::getCategoryKey(UICategory category)
{
    using namespace juce;
    if(category == none)
    {
        return String();
    }
    return uiCategoryKeys[(int) category];
}

ColourConfigFile::ConfigJson::ConfigJson() : ConfigFile(filenameConst)
{
    using namespace juce;
    var jsonConfig = AssetFiles::loadJSONAsset(String(configPath)
            + filenameConst, true);
    var defaultConfig;
    readDataFromJson(jsonConfig, defaultConfig);
    writeChanges();
}

std::vector<ConfigFile::DataKey> ColourConfigFile::ConfigJson
::getDataKeys() const
{
    using namespace juce;
    std::vector<DataKey> keys = { };
    for(const String& key : uiCategoryKeys)
    {
        keys.push_back({key,stringType});
    }
    for (auto it = colourIds.begin(); it != colourIds.end(); it++)
    {
        keys.push_back({it->first, stringType});
    }
    return keys;
}

//=============================== Colour Keys: =================================
const juce::StringArray ColourConfigFile::uiCategoryKeys = 
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

const std::map<int, juce::String> ColourConfigFile::colourIdKeys = 
{
    {ColourIds::pageComponent::background,       "Page background"},
    {ColourIds::drawableImageComponent::image0,  "Image0"},
    {ColourIds::drawableImageComponent::image1,  "Image1"},
    {ColourIds::drawableImageComponent::image2,  "Image2"},
    {ColourIds::drawableImageComponent::image3,  "Image3"},
    {ColourIds::drawableImageComponent::image4,  "Image4"},
    {ColourIds::appMenuButton::text,             "AppMenuButton text"},
    {ColourIds::appMenuButton::background,       "AppMenuButton background"},
    {ColourIds::appMenuButton::selection,        "AppMenuButton selection"},
    {ColourIds::appMenuButton::border,           "AppMenuButton border"},
    {ColourIds::fileSelectTextEditor::fileWindow,"FilePicker window"},
    {ColourIds::fileSelectTextEditor::text,      "FilePicker text"},
    {ColourIds::overlaySpinner::background,      "OverlaySpinner background"},
    {ColourIds::overlaySpinner::text,            "OverlaySpinner text"},
    {ColourIds::textButton::button,              "TextButton(off)"},
    {ColourIds::textButton::textOff,             "TextButton(off) text"},
    {ColourIds::textButton::buttonOn,            "TextButton(on)"},
    {ColourIds::textButton::textOn,              "TextButton(on) text"},
    {ColourIds::slider::background,              "Slider background"},
    {ColourIds::slider::thumb,                   "Slider thumb"},
    {ColourIds::slider::track,                   "Slider track"},
    {ColourIds::switchComponent::background,     "Switch background"},
    {ColourIds::switchComponent::handle,         "Switch handle"},
    {ColourIds::switchComponent::handleOff,      "Switch handle(off)"},
    {ColourIds::comboBox::text,                  "ComboBox text"},
    {ColourIds::comboBox::background,            "ComboBox background"},
    {ColourIds::comboBox::outline,               "ComboBox outline"},
    {ColourIds::comboBox::button,                "ComboBox button"},
    {ColourIds::comboBox::arrow,                 "ComboBox arrow"},
    {ColourIds::comboBox::focusedOutline,        "ComboBox focused outline"},
    {ColourIds::alertWindow::background,         "AlertWindow background"},
    {ColourIds::alertWindow::text,               "AlertWindow text"},
    {ColourIds::alertWindow::outline,            "AlertWindow outline"},
    {ColourIds::textEditor::background,          "TextEditor background"},
    {ColourIds::textEditor::text,                "TextEditor text"},
    {ColourIds::textEditor::highlight,           "TextEditor highlight"},
    {ColourIds::textEditor::highlightedText,     "TextEditor highlighted text"},
    {ColourIds::textEditor::outline,             "TextEditor outline"},
    {ColourIds::textEditor::focusedOutline,      "TextEditor focused outline"},
    {ColourIds::textEditor::shadow,              "TextEditor shadow"},
    {ColourIds::label::background,               "Label background"},
    {ColourIds::label::text,                     "Label text"},
    {ColourIds::label::outline,                  "Label outline"},
    {ColourIds::label::backgroundWhenEditing,    "Label background(editing)"},
    {ColourIds::label::textWhenEditing,          "Label text(editing)"},
    {ColourIds::label::outlineWhenEditing,       "Label outline(editing)"}
};

const std::map<juce::String, int> ColourConfigFile::colourIds
{
    {"Page background",            ColourIds::pageComponent::background},
            
    {"Image0",                     ColourIds::drawableImageComponent::image0},
    {"Image1",                     ColourIds::drawableImageComponent::image1},
    {"Image2",                     ColourIds::drawableImageComponent::image2},
    {"Image3",                     ColourIds::drawableImageComponent::image3},
    {"Image4",                     ColourIds::drawableImageComponent::image4},
            
    {"AppMenuButton text",         ColourIds::appMenuButton::text},
    {"AppMenuButton background",   ColourIds::appMenuButton::background},
    {"AppMenuButton selection",    ColourIds::appMenuButton::selection},
    {"AppMenuButton border",       ColourIds::appMenuButton::border},
            
    {"FilePicker window",          ColourIds::fileSelectTextEditor::fileWindow},
    {"FilePicker text",            ColourIds::fileSelectTextEditor::text},
            
    {"OverlaySpinner background",  ColourIds::overlaySpinner::background},
    {"OverlaySpinner text",        ColourIds::overlaySpinner::text},
            
    {"TextButton(off)",            ColourIds::textButton::button},
    {"TextButton(off) text",       ColourIds::textButton::textOff},
    {"TextButton(on)",             ColourIds::textButton::buttonOn},
    {"TextButton(on) text",        ColourIds::textButton::textOn},
            
    {"Slider background",          ColourIds::slider::background},
    {"Slider thumb",               ColourIds::slider::thumb},
    {"Slider track",               ColourIds::slider::track},
            
    {"Switch background",          ColourIds::switchComponent::background},
    {"Switch handle",              ColourIds::switchComponent::handle},
    {"Switch handle(off)",         ColourIds::switchComponent::handleOff},
            
    {"ComboBox text",              ColourIds::comboBox::text},
    {"ComboBox background",        ColourIds::comboBox::background},
    {"ComboBox outline",           ColourIds::comboBox::outline},
    {"ComboBox button",            ColourIds::comboBox::button},
    {"ComboBox arrow",             ColourIds::comboBox::arrow},
    {"ComboBox focused outline",   ColourIds::comboBox::focusedOutline},
            
    {"AlertWindow background",     ColourIds::alertWindow::background},
    {"AlertWindow text",           ColourIds::alertWindow::text},
    {"AlertWindow outline",        ColourIds::alertWindow::outline},
            
    {"TextEditor background",       ColourIds::textEditor::background},
    {"TextEditor text",             ColourIds::textEditor::text},
    {"TextEditor highlight",        ColourIds::textEditor::highlight},
    {"TextEditor highlighted text", ColourIds::textEditor::highlightedText},
    {"TextEditor outline",          ColourIds::textEditor::outline},
    {"TextEditor focused outline",  ColourIds::textEditor::focusedOutline},
    {"TextEditor shadow",           ColourIds::textEditor::shadow},
            
    {"Label background",            ColourIds::label::background},
    {"Label text",                  ColourIds::label::text},
    {"Label outline",               ColourIds::label::outline},
    {"Label background(editing)",   ColourIds::label::backgroundWhenEditing},
    {"Label text(editing)",         ColourIds::label::textWhenEditing},
    {"Label outline(editing)",      ColourIds::label::outlineWhenEditing}          
};

//======================= ColourId->UICategory Map =============================
// Auto-generated using ColourIdSort.pl.
const std::map<int, ColourConfigFile::UICategory>
ColourConfigFile::idCategories =
{
    {
        ColourIds::alertWindow::background,
        ColourConfigFile::UICategory::windowBackground
    },
    {
        ColourIds::alertWindow::outline,
        ColourConfigFile::UICategory::outline
    },
    {
        ColourIds::alertWindow::text,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::appMenuButton::background,
        ColourConfigFile::UICategory::widgetBackground
    },
    {
        ColourIds::appMenuButton::border,
        ColourConfigFile::UICategory::outline
    },
    {
        ColourIds::appMenuButton::selection,
        ColourConfigFile::UICategory::highlightedTextField
    },
    {
        ColourIds::appMenuButton::text,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::booleanPropertyComponent::background,
        ColourConfigFile::UICategory::textField
    },
    {
        ColourIds::booleanPropertyComponent::outline,
        ColourConfigFile::UICategory::outline
    },
    {
        ColourIds::bubbleComponent::background,
        ColourConfigFile::UICategory::menuBackground
    },
    {
        ColourIds::bubbleComponent::outline,
        ColourConfigFile::UICategory::outline
    },
    {
        ColourIds::caretComponent::caret,
        ColourConfigFile::UICategory::highlightedText
    },
    {
        ColourIds::codeEditorComponent::background,
        ColourConfigFile::UICategory::textField
    },
    {
        ColourIds::codeEditorComponent::defaultText,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::codeEditorComponent::highlight,
        ColourConfigFile::UICategory::highlightedTextField
    },
    {
        ColourIds::codeEditorComponent::lineNumberBackground,
        ColourConfigFile::UICategory::highlightedTextField
    },
    {
        ColourIds::colourSelector::background,
        ColourConfigFile::UICategory::windowBackground
    },
    {
        ColourIds::colourSelector::labelText,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::comboBox::arrow,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::comboBox::background,
        ColourConfigFile::UICategory::menuBackground
    },
    {
        ColourIds::comboBox::button,
        ColourConfigFile::UICategory::widgetOff
    },
    {
        ColourIds::comboBox::focusedOutline,
        ColourConfigFile::UICategory::focusedOutline
    },
    {
        ColourIds::comboBox::outline,
        ColourConfigFile::UICategory::outline
    },
    {
        ColourIds::comboBox::text,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::directoryContentsDisplayComponent::highlight,
        ColourConfigFile::UICategory::highlightedTextField
    },
    {
        ColourIds::directoryContentsDisplayComponent::highlightedText,
        ColourConfigFile::UICategory::highlightedText
    },
    {
        ColourIds::directoryContentsDisplayComponent::text,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::documentWindow::text,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::drawableButton::background,
        ColourConfigFile::UICategory::widgetBackground
    },
    {
        ColourIds::drawableButton::backgroundOn,
        ColourConfigFile::UICategory::widgetBackground
    },
    {
        ColourIds::drawableButton::text,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::drawableButton::textOn,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::fileBrowserComponent::currentPathBoxArrow,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::fileBrowserComponent::currentPathBoxBackground,
        ColourConfigFile::UICategory::widgetBackground
    },
    {
        ColourIds::fileBrowserComponent::currentPathBoxText,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::fileBrowserComponent::filenameBoxBackground,
        ColourConfigFile::UICategory::textField
    },
    {
        ColourIds::fileBrowserComponent::filenameBoxText,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::fileChooserDialogBox::titleText,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::fileSearchPathListComponent::background,
        ColourConfigFile::UICategory::menuBackground
    },
    {
        ColourIds::fileSelectTextEditor::fileWindow,
        ColourConfigFile::UICategory::windowBackground
    },
    {
        ColourIds::fileSelectTextEditor::text,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::groupComponent::outline,
        ColourConfigFile::UICategory::outline
    },
    {
        ColourIds::groupComponent::text,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::hyperlinkButton::text,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::jucer_CommonHeaders::activeTabIcon,
        ColourConfigFile::UICategory::widgetOn
    },
    {
        ColourIds::jucer_CommonHeaders::background,
        ColourConfigFile::UICategory::windowBackground
    },
    {
        ColourIds::jucer_CommonHeaders::codeEditorLineNumber,
        ColourConfigFile::UICategory::highlightedText
    },
    {
        ColourIds::jucer_CommonHeaders::contentHeaderBackground,
        ColourConfigFile::UICategory::windowBackground
    },
    {
        ColourIds::jucer_CommonHeaders::defaultButtonBackground,
        ColourConfigFile::UICategory::widgetBackground
    },
    {
        ColourIds::jucer_CommonHeaders::defaultHighlight,
        ColourConfigFile::UICategory::highlightedTextField
    },
    {
        ColourIds::jucer_CommonHeaders::defaultHighlightedText,
        ColourConfigFile::UICategory::highlightedText
    },
    {
        ColourIds::jucer_CommonHeaders::defaultIcon,
        ColourConfigFile::UICategory::widgetOff
    },
    {
        ColourIds::jucer_CommonHeaders::defaultText,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::jucer_CommonHeaders::inactiveTabIcon,
        ColourConfigFile::UICategory::widgetOff
    },
    {
        ColourIds::jucer_CommonHeaders::secondaryBackground,
        ColourConfigFile::UICategory::widgetBackground
    },
    {
        ColourIds::jucer_CommonHeaders::secondaryWidgetBackground,
        ColourConfigFile::UICategory::textField
    },
    {
        ColourIds::jucer_CommonHeaders::userButtonBackground,
        ColourConfigFile::UICategory::widgetBackground
    },
    {
        ColourIds::keyMappingEditorComponent::background,
        ColourConfigFile::UICategory::windowBackground
    },
    {
        ColourIds::keyMappingEditorComponent::text,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::label::backgroundWhenEditing,
        ColourConfigFile::UICategory::textField
    },
    {
        ColourIds::label::outlineWhenEditing,
        ColourConfigFile::UICategory::focusedOutline
    },
    {
        ColourIds::label::text,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::label::textWhenEditing,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::lassoComponent::lassoFill,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::lassoComponent::lassoOutline,
        ColourConfigFile::UICategory::outline
    },
    {
        ColourIds::listBox::background,
        ColourConfigFile::UICategory::menuBackground
    },
    {
        ColourIds::listBox::outline,
        ColourConfigFile::UICategory::outline
    },
    {
        ColourIds::listBox::text,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::listEditor::background,
        ColourConfigFile::UICategory::menuBackground
    },
    {
        ColourIds::listEditor::listItem,
        ColourConfigFile::UICategory::textField
    },
    {
        ColourIds::listEditor::selectedListItem,
        ColourConfigFile::UICategory::highlightedTextField
    },
    {
        ColourIds::listEditor::text,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::overlaySpinner::background,
        ColourConfigFile::UICategory::widgetBackground
    },
    {
        ColourIds::overlaySpinner::text,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::pageComponent::background,
        ColourConfigFile::UICategory::windowBackground
    },
    {
        ColourIds::popupMenu::background,
        ColourConfigFile::UICategory::menuBackground
    },
    {
        ColourIds::popupMenu::headerText,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::popupMenu::highlightedBackground,
        ColourConfigFile::UICategory::highlightedTextField
    },
    {
        ColourIds::popupMenu::highlightedText,
        ColourConfigFile::UICategory::highlightedText
    },
    {
        ColourIds::popupMenu::text,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::progressBar::background,
        ColourConfigFile::UICategory::widgetBackground
    },
    {
        ColourIds::progressBar::foreground,
        ColourConfigFile::UICategory::widgetOn
    },
    {
        ColourIds::propertyComponent::background,
        ColourConfigFile::UICategory::menuBackground
    },
    {
        ColourIds::propertyComponent::labelText,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::resizableWindow::background,
        ColourConfigFile::UICategory::windowBackground
    },
    {
        ColourIds::scrollBar::background,
        ColourConfigFile::UICategory::widgetBackground
    },
    {
        ColourIds::scrollBar::thumb,
        ColourConfigFile::UICategory::widgetOn
    },
    {
        ColourIds::scrollBar::track,
        ColourConfigFile::UICategory::widgetOff
    },
    {
        ColourIds::sidePanel::background,
        ColourConfigFile::UICategory::windowBackground
    },
    {
        ColourIds::sidePanel::dismissButtonDown,
        ColourConfigFile::UICategory::widgetOff
    },
    {
        ColourIds::sidePanel::dismissButtonNormal,
        ColourConfigFile::UICategory::widgetOn
    },
    {
        ColourIds::sidePanel::dismissButtonOver,
        ColourConfigFile::UICategory::widgetOn
    },
    {
        ColourIds::sidePanel::titleText,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::slider::background,
        ColourConfigFile::UICategory::widgetBackground
    },
    {
        ColourIds::slider::rotarySliderFill,
        ColourConfigFile::UICategory::widgetOn
    },
    {
        ColourIds::slider::rotarySliderOutline,
        ColourConfigFile::UICategory::outline
    },
    {
        ColourIds::slider::textBoxText,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::slider::thumb,
        ColourConfigFile::UICategory::widgetOn
    },
    {
        ColourIds::switchComponent::background,
        ColourConfigFile::UICategory::widgetBackground
    },
    {
        ColourIds::switchComponent::handle,
        ColourConfigFile::UICategory::widgetOn
    },
    {
        ColourIds::switchComponent::handleOff,
        ColourConfigFile::UICategory::widgetOff
    },
    {
        ColourIds::tabbedButtonBar::frontOutline,
        ColourConfigFile::UICategory::outline
    },
    {
        ColourIds::tabbedButtonBar::frontText,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::tabbedButtonBar::tabOutline,
        ColourConfigFile::UICategory::outline
    },
    {
        ColourIds::tabbedButtonBar::tabText,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::tabbedComponent::background,
        ColourConfigFile::UICategory::windowBackground
    },
    {
        ColourIds::tabbedComponent::outline,
        ColourConfigFile::UICategory::outline
    },
    {
        ColourIds::tableHeaderComponent::background,
        ColourConfigFile::UICategory::menuBackground
    },
    {
        ColourIds::tableHeaderComponent::highlight,
        ColourConfigFile::UICategory::highlightedTextField
    },
    {
        ColourIds::tableHeaderComponent::outline,
        ColourConfigFile::UICategory::outline
    },
    {
        ColourIds::tableHeaderComponent::text,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::textButton::button,
        ColourConfigFile::UICategory::widgetBackground
    },
    {
        ColourIds::textButton::buttonOn,
        ColourConfigFile::UICategory::highlightedTextField
    },
    {
        ColourIds::textButton::textOff,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::textButton::textOn,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::textEditor::background,
        ColourConfigFile::UICategory::textField
    },
    {
        ColourIds::textEditor::focusedOutline,
        ColourConfigFile::UICategory::focusedOutline
    },
    {
        ColourIds::textEditor::highlight,
        ColourConfigFile::UICategory::highlightedTextField
    },
    {
        ColourIds::textEditor::highlightedText,
        ColourConfigFile::UICategory::highlightedText
    },
    {
        ColourIds::textEditor::outline,
        ColourConfigFile::UICategory::outline
    },
    {
        ColourIds::textEditor::text,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::textPropertyComponent::background,
        ColourConfigFile::UICategory::textField
    },
    {
        ColourIds::textPropertyComponent::outline,
        ColourConfigFile::UICategory::outline
    },
    {
        ColourIds::textPropertyComponent::text,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::toggleButton::text,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::toggleButton::tick,
        ColourConfigFile::UICategory::widgetOn
    },
    {
        ColourIds::toggleButton::tickDisabled,
        ColourConfigFile::UICategory::widgetOff
    },
    {
        ColourIds::toolbar::background,
        ColourConfigFile::UICategory::menuBackground
    },
    {
        ColourIds::toolbar::buttonMouseDownBackground,
        ColourConfigFile::UICategory::widgetOn
    },
    {
        ColourIds::toolbar::buttonMouseOverBackground,
        ColourConfigFile::UICategory::widgetBackground
    },
    {
        ColourIds::toolbar::editingModeOutline,
        ColourConfigFile::UICategory::outline
    },
    {
        ColourIds::toolbar::labelText,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::tooltipWindow::background,
        ColourConfigFile::UICategory::windowBackground
    },
    {
        ColourIds::tooltipWindow::outline,
        ColourConfigFile::UICategory::outline
    },
    {
        ColourIds::tooltipWindow::text,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::treeView::background,
        ColourConfigFile::UICategory::windowBackground
    },
    {
        ColourIds::treeView::dragAndDropIndicator,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::treeView::evenItems,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::treeView::lines,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::treeView::oddItems,
        ColourConfigFile::UICategory::text
    },
    {
        ColourIds::treeView::selectedItemBackground,
        ColourConfigFile::UICategory::highlightedTextField
    }
};


