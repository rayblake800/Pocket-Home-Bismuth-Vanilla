#include "ColourConfigKeys.h"
#include "ColourIds.h"
#include <map>


//=============================== Colour Keys: =================================
 /**
 * Keys for colour values assigned to UI element colour categories.  All
 * Juce ColourId values are grouped under one of these categories.  When
 * looking up colour values, if no value is explicitly assigned to a given
 * ColourId, the value assigned to the associated UICategory will be used.
 */
static const juce::Array<juce::Identifier> uiCategoryKeys= 
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

/**
 * For each Juce ColourId value tracked in colours.json, colourIds maps that
 * colour to its string key.
 */
static const std::map<int, juce::Identifier> colourIdKeys = 
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
    {ColourIds::label::outlineWhenEditing,       "Label outline(editing)"},
    {ColourIds::colourPicker::checkerboardLight,
            "Color picker checkerboard(light)"},
    {ColourIds::colourPicker::checkerboardDark,
            "Color picker checkerboard(dark)"},
    {ColourIds::colourPicker::outline,           
            "Color picker color outline"},
    {ColourIds::colourPicker::focusedOutline, 
            "Color picker focused color outline"}
};

/**
 * For each colour key string tracked in colours.json, colourIds maps that
 * key to its Juce ColourId value.
 */
static const std::map<juce::Identifier, int> colourIds
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
    {"Label outline(editing)",      ColourIds::label::outlineWhenEditing},   
    
    {"Color picker checkerboard(light)",
            ColourIds::colourPicker::checkerboardLight},
    { "Color picker checkerboard(dark)",
            ColourIds::colourPicker::checkerboardDark},
    {"Color picker color outline", ColourIds::colourPicker::outline},
    {"Color picker focused color outline",
            ColourIds::colourPicker::focusedOutline}
};

//======================= ColourId->UICategory Map =============================
// Auto-generated using ColourIdSort.pl.
/**
 * Maps each Juce ColourId value to the UI element category used to select
 * its default value.
 */
static const std::map<int, ColourConfigKeys::UICategory> idCategories =
{
    {
        ColourIds::alertWindow::background,
        ColourConfigKeys::UICategory::windowBackground
    },
    {
        ColourIds::alertWindow::outline,
        ColourConfigKeys::UICategory::outline
    },
    {
        ColourIds::alertWindow::text,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::appMenuButton::background,
        ColourConfigKeys::UICategory::widgetBackground
    },
    {
        ColourIds::appMenuButton::border,
        ColourConfigKeys::UICategory::outline
    },
    {
        ColourIds::appMenuButton::selection,
        ColourConfigKeys::UICategory::highlightedTextField
    },
    {
        ColourIds::appMenuButton::text,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::booleanPropertyComponent::background,
        ColourConfigKeys::UICategory::textField
    },
    {
        ColourIds::booleanPropertyComponent::outline,
        ColourConfigKeys::UICategory::outline
    },
    {
        ColourIds::bubbleComponent::background,
        ColourConfigKeys::UICategory::menuBackground
    },
    {
        ColourIds::bubbleComponent::outline,
        ColourConfigKeys::UICategory::outline
    },
    {
        ColourIds::caretComponent::caret,
        ColourConfigKeys::UICategory::highlightedText
    },
    {
        ColourIds::codeEditorComponent::background,
        ColourConfigKeys::UICategory::textField
    },
    {
        ColourIds::codeEditorComponent::defaultText,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::codeEditorComponent::highlight,
        ColourConfigKeys::UICategory::highlightedTextField
    },
    {
        ColourIds::codeEditorComponent::lineNumberBackground,
        ColourConfigKeys::UICategory::highlightedTextField
    },
    {
        ColourIds::colourSelector::background,
        ColourConfigKeys::UICategory::windowBackground
    },
    {
        ColourIds::colourSelector::labelText,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::comboBox::arrow,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::comboBox::background,
        ColourConfigKeys::UICategory::menuBackground
    },
    {
        ColourIds::comboBox::button,
        ColourConfigKeys::UICategory::widgetOff
    },
    {
        ColourIds::comboBox::focusedOutline,
        ColourConfigKeys::UICategory::focusedOutline
    },
    {
        ColourIds::comboBox::outline,
        ColourConfigKeys::UICategory::outline
    },
    {
        ColourIds::comboBox::text,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::directoryContentsDisplayComponent::highlight,
        ColourConfigKeys::UICategory::highlightedTextField
    },
    {
        ColourIds::directoryContentsDisplayComponent::highlightedText,
        ColourConfigKeys::UICategory::highlightedText
    },
    {
        ColourIds::directoryContentsDisplayComponent::text,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::documentWindow::text,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::drawableButton::background,
        ColourConfigKeys::UICategory::widgetBackground
    },
    {
        ColourIds::drawableButton::backgroundOn,
        ColourConfigKeys::UICategory::widgetBackground
    },
    {
        ColourIds::drawableButton::text,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::drawableButton::textOn,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::fileBrowserComponent::currentPathBoxArrow,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::fileBrowserComponent::currentPathBoxBackground,
        ColourConfigKeys::UICategory::widgetBackground
    },
    {
        ColourIds::fileBrowserComponent::currentPathBoxText,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::fileBrowserComponent::filenameBoxBackground,
        ColourConfigKeys::UICategory::textField
    },
    {
        ColourIds::fileBrowserComponent::filenameBoxText,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::fileChooserDialogBox::titleText,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::fileSearchPathListComponent::background,
        ColourConfigKeys::UICategory::menuBackground
    },
    {
        ColourIds::fileSelectTextEditor::fileWindow,
        ColourConfigKeys::UICategory::windowBackground
    },
    {
        ColourIds::fileSelectTextEditor::text,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::groupComponent::outline,
        ColourConfigKeys::UICategory::outline
    },
    {
        ColourIds::groupComponent::text,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::hyperlinkButton::text,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::jucer_CommonHeaders::activeTabIcon,
        ColourConfigKeys::UICategory::widgetOn
    },
    {
        ColourIds::jucer_CommonHeaders::background,
        ColourConfigKeys::UICategory::windowBackground
    },
    {
        ColourIds::jucer_CommonHeaders::codeEditorLineNumber,
        ColourConfigKeys::UICategory::highlightedText
    },
    {
        ColourIds::jucer_CommonHeaders::contentHeaderBackground,
        ColourConfigKeys::UICategory::windowBackground
    },
    {
        ColourIds::jucer_CommonHeaders::defaultButtonBackground,
        ColourConfigKeys::UICategory::widgetBackground
    },
    {
        ColourIds::jucer_CommonHeaders::defaultHighlight,
        ColourConfigKeys::UICategory::highlightedTextField
    },
    {
        ColourIds::jucer_CommonHeaders::defaultHighlightedText,
        ColourConfigKeys::UICategory::highlightedText
    },
    {
        ColourIds::jucer_CommonHeaders::defaultIcon,
        ColourConfigKeys::UICategory::widgetOff
    },
    {
        ColourIds::jucer_CommonHeaders::defaultText,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::jucer_CommonHeaders::inactiveTabIcon,
        ColourConfigKeys::UICategory::widgetOff
    },
    {
        ColourIds::jucer_CommonHeaders::secondaryBackground,
        ColourConfigKeys::UICategory::widgetBackground
    },
    {
        ColourIds::jucer_CommonHeaders::secondaryWidgetBackground,
        ColourConfigKeys::UICategory::textField
    },
    {
        ColourIds::jucer_CommonHeaders::userButtonBackground,
        ColourConfigKeys::UICategory::widgetBackground
    },
    {
        ColourIds::keyMappingEditorComponent::background,
        ColourConfigKeys::UICategory::windowBackground
    },
    {
        ColourIds::keyMappingEditorComponent::text,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::label::backgroundWhenEditing,
        ColourConfigKeys::UICategory::textField
    },
    {
        ColourIds::label::outlineWhenEditing,
        ColourConfigKeys::UICategory::focusedOutline
    },
    {
        ColourIds::label::text,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::label::textWhenEditing,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::lassoComponent::lassoFill,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::lassoComponent::lassoOutline,
        ColourConfigKeys::UICategory::outline
    },
    {
        ColourIds::listBox::background,
        ColourConfigKeys::UICategory::menuBackground
    },
    {
        ColourIds::listBox::outline,
        ColourConfigKeys::UICategory::outline
    },
    {
        ColourIds::listBox::text,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::listEditor::background,
        ColourConfigKeys::UICategory::menuBackground
    },
    {
        ColourIds::listEditor::listItem,
        ColourConfigKeys::UICategory::textField
    },
    {
        ColourIds::listEditor::selectedListItem,
        ColourConfigKeys::UICategory::highlightedTextField
    },
    {
        ColourIds::listEditor::text,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::overlaySpinner::background,
        ColourConfigKeys::UICategory::widgetBackground
    },
    {
        ColourIds::overlaySpinner::text,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::pageComponent::background,
        ColourConfigKeys::UICategory::windowBackground
    },
    {
        ColourIds::popupMenu::background,
        ColourConfigKeys::UICategory::menuBackground
    },
    {
        ColourIds::popupMenu::headerText,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::popupMenu::highlightedBackground,
        ColourConfigKeys::UICategory::highlightedTextField
    },
    {
        ColourIds::popupMenu::highlightedText,
        ColourConfigKeys::UICategory::highlightedText
    },
    {
        ColourIds::popupMenu::text,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::progressBar::background,
        ColourConfigKeys::UICategory::widgetBackground
    },
    {
        ColourIds::progressBar::foreground,
        ColourConfigKeys::UICategory::widgetOn
    },
    {
        ColourIds::propertyComponent::background,
        ColourConfigKeys::UICategory::menuBackground
    },
    {
        ColourIds::propertyComponent::labelText,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::resizableWindow::background,
        ColourConfigKeys::UICategory::windowBackground
    },
    {
        ColourIds::scrollBar::background,
        ColourConfigKeys::UICategory::widgetBackground
    },
    {
        ColourIds::scrollBar::thumb,
        ColourConfigKeys::UICategory::widgetOn
    },
    {
        ColourIds::scrollBar::track,
        ColourConfigKeys::UICategory::widgetOff
    },
    {
        ColourIds::sidePanel::background,
        ColourConfigKeys::UICategory::windowBackground
    },
    {
        ColourIds::sidePanel::dismissButtonDown,
        ColourConfigKeys::UICategory::widgetOff
    },
    {
        ColourIds::sidePanel::dismissButtonNormal,
        ColourConfigKeys::UICategory::widgetOn
    },
    {
        ColourIds::sidePanel::dismissButtonOver,
        ColourConfigKeys::UICategory::widgetOn
    },
    {
        ColourIds::sidePanel::titleText,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::slider::background,
        ColourConfigKeys::UICategory::widgetBackground
    },
    {
        ColourIds::slider::rotarySliderFill,
        ColourConfigKeys::UICategory::widgetOn
    },
    {
        ColourIds::slider::rotarySliderOutline,
        ColourConfigKeys::UICategory::outline
    },
    {
        ColourIds::slider::textBoxText,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::slider::thumb,
        ColourConfigKeys::UICategory::widgetOn
    },
    {
        ColourIds::switchComponent::background,
        ColourConfigKeys::UICategory::widgetBackground
    },
    {
        ColourIds::switchComponent::handle,
        ColourConfigKeys::UICategory::widgetOn
    },
    {
        ColourIds::switchComponent::handleOff,
        ColourConfigKeys::UICategory::widgetOff
    },
    {
        ColourIds::tabbedButtonBar::frontOutline,
        ColourConfigKeys::UICategory::outline
    },
    {
        ColourIds::tabbedButtonBar::frontText,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::tabbedButtonBar::tabOutline,
        ColourConfigKeys::UICategory::outline
    },
    {
        ColourIds::tabbedButtonBar::tabText,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::tabbedComponent::background,
        ColourConfigKeys::UICategory::windowBackground
    },
    {
        ColourIds::tabbedComponent::outline,
        ColourConfigKeys::UICategory::outline
    },
    {
        ColourIds::tableHeaderComponent::background,
        ColourConfigKeys::UICategory::menuBackground
    },
    {
        ColourIds::tableHeaderComponent::highlight,
        ColourConfigKeys::UICategory::highlightedTextField
    },
    {
        ColourIds::tableHeaderComponent::outline,
        ColourConfigKeys::UICategory::outline
    },
    {
        ColourIds::tableHeaderComponent::text,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::textButton::button,
        ColourConfigKeys::UICategory::widgetBackground
    },
    {
        ColourIds::textButton::buttonOn,
        ColourConfigKeys::UICategory::highlightedTextField
    },
    {
        ColourIds::textButton::textOff,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::textButton::textOn,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::textEditor::background,
        ColourConfigKeys::UICategory::textField
    },
    {
        ColourIds::textEditor::focusedOutline,
        ColourConfigKeys::UICategory::focusedOutline
    },
    {
        ColourIds::textEditor::highlight,
        ColourConfigKeys::UICategory::highlightedTextField
    },
    {
        ColourIds::textEditor::highlightedText,
        ColourConfigKeys::UICategory::highlightedText
    },
    {
        ColourIds::textEditor::outline,
        ColourConfigKeys::UICategory::outline
    },
    {
        ColourIds::textEditor::text,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::textPropertyComponent::background,
        ColourConfigKeys::UICategory::textField
    },
    {
        ColourIds::textPropertyComponent::outline,
        ColourConfigKeys::UICategory::outline
    },
    {
        ColourIds::textPropertyComponent::text,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::toggleButton::text,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::toggleButton::tick,
        ColourConfigKeys::UICategory::widgetOn
    },
    {
        ColourIds::toggleButton::tickDisabled,
        ColourConfigKeys::UICategory::widgetOff
    },
    {
        ColourIds::toolbar::background,
        ColourConfigKeys::UICategory::menuBackground
    },
    {
        ColourIds::toolbar::buttonMouseDownBackground,
        ColourConfigKeys::UICategory::widgetOn
    },
    {
        ColourIds::toolbar::buttonMouseOverBackground,
        ColourConfigKeys::UICategory::widgetBackground
    },
    {
        ColourIds::toolbar::editingModeOutline,
        ColourConfigKeys::UICategory::outline
    },
    {
        ColourIds::toolbar::labelText,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::tooltipWindow::background,
        ColourConfigKeys::UICategory::windowBackground
    },
    {
        ColourIds::tooltipWindow::outline,
        ColourConfigKeys::UICategory::outline
    },
    {
        ColourIds::tooltipWindow::text,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::treeView::background,
        ColourConfigKeys::UICategory::windowBackground
    },
    {
        ColourIds::treeView::dragAndDropIndicator,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::treeView::evenItems,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::treeView::lines,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::treeView::oddItems,
        ColourConfigKeys::UICategory::text
    },
    {
        ColourIds::treeView::selectedItemBackground,
        ColourConfigKeys::UICategory::highlightedTextField
    },
    {
        ColourIds::colourPicker::outline,
        ColourConfigKeys::UICategory::outline
    },
    {
        ColourIds::colourPicker::focusedOutline,
        ColourConfigKeys::UICategory::focusedOutline        
    }
};

/*
 * Gets all Juce ColourId values defined by the colour config file.
 */
const juce::Array<int>& ColourConfigKeys::getColourIds()
{
    static juce::Array<int> idList;
    if(idList.isEmpty())
    {
        for(const auto& iter : idCategories)
        {
            idList.add(iter.first);
        }
        for(const auto& iter : colourIdKeys)
        {
            idList.addIfNotAlreadyThere(iter.first);
        }
    }
    return idList;
}
    
/*
 * Gets all UI category color keys.
 */
const juce::Array<juce::Identifier>& ColourConfigKeys::getCategoryKeys()
{
    return uiCategoryKeys;
}

/*
 * Gets all keys used by the ColourConfigFile.
 */
const juce::Array<juce::Identifier>& ColourConfigKeys::getColourKeys()
{
    using namespace juce;
    static Array<Identifier> colourKeys;
    if(colourKeys.isEmpty())
    {
        colourKeys = uiCategoryKeys;
        for(const auto& colourId : colourIdKeys)
        {
            colourKeys.add(colourId.second);
        }
    }
    return colourKeys;
}

/*
 * Gets the UICategory assigned to a Juce ColourId value.
 */
ColourConfigKeys::UICategory ColourConfigKeys::getUICategory
(const int colourId)
{
    auto searchIter = idCategories.find(colourId);
    if (searchIter == idCategories.end())
    {
        return UICategory::none;
    }
    return searchIter->second;
}

//=================== Lookup values using keys: ============================
/**
 * Finds the Juce ColourId value of a UI element from its key.
 */
int ColourConfigKeys::getColourId(const juce::Identifier& colourKey)
{
    auto searchIter = colourIds.find(colourKey);
    if (searchIter == colourIds.end())
    {
        return -1;
    }
    return searchIter->second;
}

/**
 * Finds the UICategory type represented by a specific key.
 */
ColourConfigKeys::UICategory ColourConfigKeys::getCategoryType
(const juce::Identifier& categoryKey)
{
    int enumVal = uiCategoryKeys.indexOf(categoryKey);
    if(enumVal < 0)
    {
        return none;
    }
    return (UICategory) enumVal; 
}

 //====================== Lookup keys by value: ============================
/**
 * Gets the key used to store a specific Juce ColourId in the colour config 
 * file.
 */
const juce::Identifier& ColourConfigKeys::getColourKey(const int colourId)
{
    using namespace juce;
    auto keySearch = colourIdKeys.find(colourId);
    if(keySearch == colourIdKeys.end())
    {
        return invalidKey;
    }
    return keySearch->second;
}

/*
 * Finds the key representing a UICategory value.
 */
const juce::Identifier& ColourConfigKeys::getCategoryKey
(const UICategory category)
{
    if(category == none)
    {
        return invalidKey;
    }
    return uiCategoryKeys[(int) category];
}
