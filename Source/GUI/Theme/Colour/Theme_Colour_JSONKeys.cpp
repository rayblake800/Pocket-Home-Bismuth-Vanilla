#include "Theme_Colour_JSONKeys.h"
#include "Theme_Colour_ColourIds.h"
#include <map>

namespace ColourKeys = Theme::Colour::JSONKeys;

//=============================== Colour Keys: =================================
/**
 * @brief  Keys for colour values assigned to UI element colour categories.  
 *
 * All Juce ColourId values are grouped under one of these categories. When
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

namespace Theme { namespace Colour { namespace ColourIds {

/**
 * For each Juce ColourId value tracked in colours.json, colourIdKeys maps that
 * colour to its string key.
 */
static const std::map<int, juce::Identifier> colourIdKeys = 
{
    {pageComponent::background,       "Page background"},
    {drawableImageComponent::image0,  "Image0"},
    {drawableImageComponent::image1,  "Image1"},
    {drawableImageComponent::image2,  "Image2"},
    {drawableImageComponent::image3,  "Image3"},
    {drawableImageComponent::image4,  "Image4"},
    {appMenuButton::text,             "AppMenuButton text"},
    {appMenuButton::selectedText,     "AppMenuButton selected text"},
    {appMenuButton::textBackground,   "AppMenuButton text background"},
    {appMenuButton::selectedTextBG,   "AppMenuButton selected text background"},
    {appMenuButton::background,       "AppMenuButton background"},
    {appMenuButton::selectedBG,       "AppMenuButton selected background"},
    {appMenuButton::border,           "AppMenuButton border"},
    {appMenuButton::selectedBorder,   "AppMenuButton selected border"},
    {fileSelectTextEditor::fileWindow,"FilePicker window"},
    {fileSelectTextEditor::text,      "FilePicker text"},
    {overlaySpinner::background,      "OverlaySpinner background"},
    {overlaySpinner::text,            "OverlaySpinner text"},
    {textButton::button,              "TextButton(off)"},
    {textButton::textOff,             "TextButton(off) text"},
    {textButton::buttonOn,            "TextButton(on)"},
    {textButton::textOn,              "TextButton(on) text"},
    {slider::background,              "Slider background"},
    {slider::thumb,                   "Slider thumb"},
    {slider::track,                   "Slider track"},
    {switchComponent::background,     "Switch background"},
    {switchComponent::handle,         "Switch handle"},
    {switchComponent::handleOff,      "Switch handle(off)"},
    {comboBox::text,                  "ComboBox text"},
    {comboBox::background,            "ComboBox background"},
    {comboBox::outline,               "ComboBox outline"},
    {comboBox::button,                "ComboBox button"},
    {comboBox::arrow,                 "ComboBox arrow"},
    {comboBox::focusedOutline,        "ComboBox focused outline"},
    {alertWindow::background,         "AlertWindow background"},
    {alertWindow::text,               "AlertWindow text"},
    {alertWindow::outline,            "AlertWindow outline"},
    {textEditor::background,          "TextEditor background"},
    {textEditor::text,                "TextEditor text"},
    {textEditor::highlight,           "TextEditor highlight"},
    {textEditor::highlightedText,     "TextEditor highlighted text"},
    {textEditor::outline,             "TextEditor outline"},
    {textEditor::focusedOutline,      "TextEditor focused outline"},
    {textEditor::shadow,              "TextEditor shadow"},
    {label::background,               "Label background"},
    {label::text,                     "Label text"},
    {label::outline,                  "Label outline"},
    {label::backgroundWhenEditing,    "Label background(editing)"},
    {label::textWhenEditing,          "Label text(editing)"},
    {label::outlineWhenEditing,       "Label outline(editing)"},
    {colourPicker::checkerboardLight,
            "Color picker checkerboard(light)"},
    {colourPicker::checkerboardDark,
            "Color picker checkerboard(dark)"},
    {colourPicker::outline,           
            "Color picker color outline"},
    {colourPicker::focusedOutline, 
            "Color picker focused color outline"},
    {clockLabel::text,                "ClockLabel text"},
    {batteryIcon::text,               "BatteryIcon text"}
};

/*
 * For each colour key string tracked in colours.json, colourIds maps that
 * key to its Juce ColourId value.
 */
static const std::map<juce::Identifier, int> colourIds
{
    {"Page background",            pageComponent::background},
            
    {"Image0",                     drawableImageComponent::image0},
    {"Image1",                     drawableImageComponent::image1},
    {"Image2",                     drawableImageComponent::image2},
    {"Image3",                     drawableImageComponent::image3},
    {"Image4",                     drawableImageComponent::image4},
            
    {"AppMenuButton text",                     appMenuButton::text},
    {"AppMenuButton selected text",            appMenuButton::selectedText},
    {"AppMenuButton text background",          appMenuButton::textBackground},
    {"AppMenuButton selected text background", appMenuButton::selectedTextBG},
    {"AppMenuButton background",               appMenuButton::background},
    {"AppMenuButton selected background",      appMenuButton::selectedBG},
    {"AppMenuButton border",                   appMenuButton::border},
    {"AppMenuButton selected border",          appMenuButton::selectedBorder},
            
    {"FilePicker window",          fileSelectTextEditor::fileWindow},
    {"FilePicker text",            fileSelectTextEditor::text},
            
    {"OverlaySpinner background",  overlaySpinner::background},
    {"OverlaySpinner text",        overlaySpinner::text},
            
    {"TextButton(off)",            textButton::button},
    {"TextButton(off) text",       textButton::textOff},
    {"TextButton(on)",             textButton::buttonOn},
    {"TextButton(on) text",        textButton::textOn},
            
    {"Slider background",          slider::background},
    {"Slider thumb",               slider::thumb},
    {"Slider track",               slider::track},
            
    {"Switch background",          switchComponent::background},
    {"Switch handle",              switchComponent::handle},
    {"Switch handle(off)",         switchComponent::handleOff},
            
    {"ComboBox text",              comboBox::text},
    {"ComboBox background",        comboBox::background},
    {"ComboBox outline",           comboBox::outline},
    {"ComboBox button",            comboBox::button},
    {"ComboBox arrow",             comboBox::arrow},
    {"ComboBox focused outline",   comboBox::focusedOutline},
            
    {"AlertWindow background",     alertWindow::background},
    {"AlertWindow text",           alertWindow::text},
    {"AlertWindow outline",        alertWindow::outline},
            
    {"TextEditor background",       textEditor::background},
    {"TextEditor text",             textEditor::text},
    {"TextEditor highlight",        textEditor::highlight},
    {"TextEditor highlighted text", textEditor::highlightedText},
    {"TextEditor outline",          textEditor::outline},
    {"TextEditor focused outline",  textEditor::focusedOutline},
    {"TextEditor shadow",           textEditor::shadow},
            
    {"Label background",            label::background},
    {"Label text",                  label::text},
    {"Label outline",               label::outline},
    {"Label background(editing)",   label::backgroundWhenEditing},
    {"Label text(editing)",         label::textWhenEditing},
    {"Label outline(editing)",      label::outlineWhenEditing},   
    
    {"Color picker checkerboard(light)",
            colourPicker::checkerboardLight},
    { "Color picker checkerboard(dark)",
            colourPicker::checkerboardDark},
    {"Color picker color outline", colourPicker::outline},
    {"Color picker focused color outline",
            colourPicker::focusedOutline},
    {"ClockLabel text",             clockLabel::text},
    {"BatteryIcon text",            batteryIcon::text}
};


//======================= ColourId->UICategory Map =============================

// Auto-generated using ColourIdSort.pl.

/**
 * @brief  Maps each Juce ColourId value to the UI element category used to 
 *         select its default value.
 */
static const std::map<int, Theme::Colour::UICategory> idCategories =
{
    {
        alertWindow::background,
        UICategory::windowBackground
    },
    {
        alertWindow::outline,
        UICategory::outline
    },
    {
        alertWindow::text,
        UICategory::text
    },
    {
        appMenuButton::background,
        UICategory::widgetBackground
    },
    {
        appMenuButton::selectedBG,
        UICategory::widgetBackground
    },
    {
        appMenuButton::textBackground,
        UICategory::widgetBackground
    },
    {
        appMenuButton::selectedTextBG,
        UICategory::widgetBackground
    },
    {
        appMenuButton::border,
        UICategory::outline
    },
    {
        appMenuButton::selectedBorder,
        UICategory::outline
    },
    {
        appMenuButton::text,
        UICategory::text
    },
    {
        appMenuButton::selectedText,
        UICategory::text
    },
    {
        booleanPropertyComponent::background,
        UICategory::textField
    },
    {
        booleanPropertyComponent::outline,
        UICategory::outline
    },
    {
        bubbleComponent::background,
        UICategory::menuBackground
    },
    {
        bubbleComponent::outline,
        UICategory::outline
    },
    {
        caretComponent::caret,
        UICategory::highlightedText
    },
    {
        codeEditorComponent::background,
        UICategory::textField
    },
    {
        codeEditorComponent::defaultText,
        UICategory::text
    },
    {
        codeEditorComponent::highlight,
        UICategory::highlightedTextField
    },
    {
        codeEditorComponent::lineNumberBackground,
        UICategory::highlightedTextField
    },
    {
        colourSelector::background,
        UICategory::windowBackground
    },
    {
        colourSelector::labelText,
        UICategory::text
    },
    {
        comboBox::arrow,
        UICategory::text
    },
    {
        comboBox::background,
        UICategory::menuBackground
    },
    {
        comboBox::button,
        UICategory::widgetOff
    },
    {
        comboBox::focusedOutline,
        UICategory::focusedOutline
    },
    {
        comboBox::outline,
        UICategory::outline
    },
    {
        comboBox::text,
        UICategory::text
    },
    {
        directoryContentsDisplayComponent::highlight,
        UICategory::highlightedTextField
    },
    {
        directoryContentsDisplayComponent::highlightedText,
        UICategory::highlightedText
    },
    {
        directoryContentsDisplayComponent::text,
        UICategory::text
    },
    {
        documentWindow::text,
        UICategory::text
    },
    {
        drawableButton::background,
        UICategory::widgetBackground
    },
    {
        drawableButton::backgroundOn,
        UICategory::widgetBackground
    },
    {
        drawableButton::text,
        UICategory::text
    },
    {
        drawableButton::textOn,
        UICategory::text
    },
    {
        fileBrowserComponent::currentPathBoxArrow,
        UICategory::text
    },
    {
        fileBrowserComponent::currentPathBoxBackground,
        UICategory::widgetBackground
    },
    {
        fileBrowserComponent::currentPathBoxText,
        UICategory::text
    },
    {
        fileBrowserComponent::filenameBoxBackground,
        UICategory::textField
    },
    {
        fileBrowserComponent::filenameBoxText,
        UICategory::text
    },
    {
        fileChooserDialogBox::titleText,
        UICategory::text
    },
    {
        fileSearchPathListComponent::background,
        UICategory::menuBackground
    },
    {
        fileSelectTextEditor::fileWindow,
        UICategory::windowBackground
    },
    {
        fileSelectTextEditor::text,
        UICategory::text
    },
    {
        groupComponent::outline,
        UICategory::outline
    },
    {
        groupComponent::text,
        UICategory::text
    },
    {
        hyperlinkButton::text,
        UICategory::text
    },
    {
        jucer_CommonHeaders::activeTabIcon,
        UICategory::widgetOn
    },
    {
        jucer_CommonHeaders::background,
        UICategory::windowBackground
    },
    {
        jucer_CommonHeaders::codeEditorLineNumber,
        UICategory::highlightedText
    },
    {
        jucer_CommonHeaders::contentHeaderBackground,
        UICategory::windowBackground
    },
    {
        jucer_CommonHeaders::defaultButtonBackground,
        UICategory::widgetBackground
    },
    {
        jucer_CommonHeaders::defaultHighlight,
        UICategory::highlightedTextField
    },
    {
        jucer_CommonHeaders::defaultHighlightedText,
        UICategory::highlightedText
    },
    {
        jucer_CommonHeaders::defaultIcon,
        UICategory::widgetOff
    },
    {
        jucer_CommonHeaders::defaultText,
        UICategory::text
    },
    {
        jucer_CommonHeaders::inactiveTabIcon,
        UICategory::widgetOff
    },
    {
        jucer_CommonHeaders::secondaryBackground,
        UICategory::widgetBackground
    },
    {
        jucer_CommonHeaders::secondaryWidgetBackground,
        UICategory::textField
    },
    {
        jucer_CommonHeaders::userButtonBackground,
        UICategory::widgetBackground
    },
    {
        keyMappingEditorComponent::background,
        UICategory::windowBackground
    },
    {
        keyMappingEditorComponent::text,
        UICategory::text
    },
    {
        label::backgroundWhenEditing,
        UICategory::textField
    },
    {
        label::outlineWhenEditing,
        UICategory::focusedOutline
    },
    {
        label::text,
        UICategory::text
    },
    {
        label::textWhenEditing,
        UICategory::text
    },
    {
        lassoComponent::lassoFill,
        UICategory::text
    },
    {
        lassoComponent::lassoOutline,
        UICategory::outline
    },
    {
        listBox::background,
        UICategory::menuBackground
    },
    {
        listBox::outline,
        UICategory::outline
    },
    {
        listBox::text,
        UICategory::text
    },
    {
        listEditor::background,
        UICategory::menuBackground
    },
    {
        listEditor::listItem,
        UICategory::textField
    },
    {
        listEditor::selectedListItem,
        UICategory::highlightedTextField
    },
    {
        listEditor::text,
        UICategory::text
    },
    {
        layoutContainer::background,
        UICategory::menuBackground
    },
    {
        overlaySpinner::background,
        UICategory::widgetBackground
    },
    {
        overlaySpinner::text,
        UICategory::text
    },
    {
        pageComponent::background,
        UICategory::windowBackground
    },
    {
        popupMenu::background,
        UICategory::menuBackground
    },
    {
        popupMenu::headerText,
        UICategory::text
    },
    {
        popupMenu::highlightedBackground,
        UICategory::highlightedTextField
    },
    {
        popupMenu::highlightedText,
        UICategory::highlightedText
    },
    {
        popupMenu::text,
        UICategory::text
    },
    {
        progressBar::background,
        UICategory::widgetBackground
    },
    {
        progressBar::foreground,
        UICategory::widgetOn
    },
    {
        propertyComponent::background,
        UICategory::menuBackground
    },
    {
        propertyComponent::labelText,
        UICategory::text
    },
    {
        resizeableWindow::background,
        UICategory::windowBackground
    },
    {
        scrollBar::background,
        UICategory::widgetBackground
    },
    {
        scrollBar::thumb,
        UICategory::widgetOn
    },
    {
        scrollBar::track,
        UICategory::widgetOff
    },
    {
        sidePanel::background,
        UICategory::windowBackground
    },
    {
        sidePanel::dismissButtonDown,
        UICategory::widgetOff
    },
    {
        sidePanel::dismissButtonNormal,
        UICategory::widgetOn
    },
    {
        sidePanel::dismissButtonOver,
        UICategory::widgetOn
    },
    {
        sidePanel::titleText,
        UICategory::text
    },
    {
        slider::background,
        UICategory::widgetBackground
    },
    {
        slider::rotarySliderFill,
        UICategory::widgetOn
    },
    {
        slider::rotarySliderOutline,
        UICategory::outline
    },
    {
        slider::textBoxText,
        UICategory::text
    },
    {
        slider::thumb,
        UICategory::widgetOn
    },
    {
        switchComponent::background,
        UICategory::widgetBackground
    },
    {
        switchComponent::handle,
        UICategory::widgetOn
    },
    {
        switchComponent::handleOff,
        UICategory::widgetOff
    },
    {
        tabbedButtonBar::frontOutline,
        UICategory::outline
    },
    {
        tabbedButtonBar::frontText,
        UICategory::text
    },
    {
        tabbedButtonBar::tabOutline,
        UICategory::outline
    },
    {
        tabbedButtonBar::tabText,
        UICategory::text
    },
    {
        tabbedComponent::background,
        UICategory::windowBackground
    },
    {
        tabbedComponent::outline,
        UICategory::outline
    },
    {
        tableHeaderComponent::background,
        UICategory::menuBackground
    },
    {
        tableHeaderComponent::highlight,
        UICategory::highlightedTextField
    },
    {
        tableHeaderComponent::outline,
        UICategory::outline
    },
    {
        tableHeaderComponent::text,
        UICategory::text
    },
    {
        textButton::button,
        UICategory::widgetBackground
    },
    {
        textButton::buttonOn,
        UICategory::highlightedTextField
    },
    {
        textButton::textOff,
        UICategory::text
    },
    {
        textButton::textOn,
        UICategory::text
    },
    {
        textEditor::background,
        UICategory::textField
    },
    {
        textEditor::focusedOutline,
        UICategory::focusedOutline
    },
    {
        textEditor::highlight,
        UICategory::highlightedTextField
    },
    {
        textEditor::highlightedText,
        UICategory::highlightedText
    },
    {
        textEditor::outline,
        UICategory::outline
    },
    {
        textEditor::text,
        UICategory::text
    },
    {
        textPropertyComponent::background,
        UICategory::textField
    },
    {
        textPropertyComponent::outline,
        UICategory::outline
    },
    {
        textPropertyComponent::text,
        UICategory::text
    },
    {
        toggleButton::text,
        UICategory::text
    },
    {
        toggleButton::tick,
        UICategory::widgetOn
    },
    {
        toggleButton::tickDisabled,
        UICategory::widgetOff
    },
    {
        toolbar::background,
        UICategory::menuBackground
    },
    {
        toolbar::buttonMouseDownBackground,
        UICategory::widgetOn
    },
    {
        toolbar::buttonMouseOverBackground,
        UICategory::widgetBackground
    },
    {
        toolbar::editingModeOutline,
        UICategory::outline
    },
    {
        toolbar::labelText,
        UICategory::text
    },
    {
        tooltipWindow::background,
        UICategory::windowBackground
    },
    {
        tooltipWindow::outline,
        UICategory::outline
    },
    {
        tooltipWindow::text,
        UICategory::text
    },
    {
        treeView::background,
        UICategory::windowBackground
    },
    {
        treeView::dragAndDropIndicator,
        UICategory::text
    },
    {
        treeView::evenItems,
        UICategory::text
    },
    {
        treeView::lines,
        UICategory::text
    },
    {
        treeView::oddItems,
        UICategory::text
    },
    {
        treeView::selectedItemBackground,
        UICategory::highlightedTextField
    },
    {
        colourPicker::outline,
        UICategory::outline
    },
    {
        colourPicker::focusedOutline,
        UICategory::focusedOutline        
    },
    {
        clockLabel::text,
        UICategory::text
    },
    {
        batteryIcon::text,
        UICategory::text
    }
};

}}}

/*
 * Gets all Juce ColourId values defined by the colour config file.
 */
const juce::Array<int>& ColourKeys::getColourIds()
{
    static juce::Array<int> idList;
    if(idList.isEmpty())
    {
        for(const auto& iter : ColourIds::idCategories)
        {
            idList.add(iter.first);
        }
        for(const auto& iter : ColourIds::colourIdKeys)
        {
            idList.addIfNotAlreadyThere(iter.first);
        }
    }
    return idList;
}
    
/*
 * Gets all UI category color keys.
 */
const juce::Array<juce::Identifier>& ColourKeys::getCategoryKeys()
{
    return uiCategoryKeys;
}

/*
 * Gets all keys used by the ColourConfigFile.
 */
const juce::Array<juce::Identifier>& ColourKeys::getColourKeys()
{
    static juce::Array<juce::Identifier> colourKeys;
    if(colourKeys.isEmpty())
    {
        colourKeys = uiCategoryKeys;
        for(const auto& colourId : ColourIds::colourIdKeys)
        {
            colourKeys.add(colourId.second);
        }
    }
    return colourKeys;
}

/*
 * Gets the UICategory assigned to a Juce ColourId value.
 */
Theme::Colour::UICategory ColourKeys::getUICategory
(const int colourId)
{
    auto searchIter = ColourIds::idCategories.find(colourId);
    if (searchIter == ColourIds::idCategories.end())
    {
        //DBG("Theme::Colour::JSONKeys::" << __func__ 
        //        << ": No category found for colorId "
        //        << juce::String::toHexString(colourId));
        return UICategory::none;
    }
    return searchIter->second;
}

//=================== Lookup values using keys: ============================
/*
 * Finds the Juce ColourId value of a UI element from its key.
 */
int ColourKeys::getColourId(const juce::Identifier& colourKey)
{
    auto searchIter = ColourIds::colourIds.find(colourKey);
    if (searchIter == ColourIds::colourIds.end())
    {
        DBG("Theme::Colour::JSONKeys::" << __func__ << ": No Id found for key "
                << colourKey.toString());
        return -1;
    }
    return searchIter->second;
}

/*
 * Finds the UICategory type represented by a specific key.
 */
Theme::Colour::UICategory ColourKeys::getCategoryType
(const juce::Identifier& categoryKey)
{
    int enumVal = uiCategoryKeys.indexOf(categoryKey);
    if(enumVal < 0)
    {
        DBG("Theme::Colour::JSONKeys::" << __func__ 
                << ": No category matches key " << categoryKey.toString());

        return UICategory::none;
    }
    return (UICategory) enumVal; 
}

 //====================== Lookup keys by value: ============================
/*
 * Gets the key used to store a specific Juce ColourId in the colour config 
 * file.
 */
const juce::Identifier& ColourKeys::getColourKey(const int colourId)
{
    using namespace ColourIds;
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
const juce::Identifier& ColourKeys::getCategoryKey
(const UICategory category)
{
    if(category == UICategory::none)
    {
        DBG("Theme::Colour::JSONKeys::" << __func__ 
                << ": No key, category == none");
        return invalidKey;
    }
    return uiCategoryKeys.getReference((int) category);
}
