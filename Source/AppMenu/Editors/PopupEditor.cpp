#define APPMENU_IMPLEMENTATION_ONLY
#include "Utils.h"
#include "IconLoader.h"
#include "PopupEditor.h"
#include "ComponentConfigFile.h"
      
/* Localized text keys: */
static const juce::Identifier localizedObjectKey = "AppMenu::PopupEditor";
static const juce::Identifier name = "name";
static const juce::Identifier icon_path = "icon_path";
static const juce::Identifier select_icon = "select_icon";
static const juce::Identifier edit_categories = "edit_categories";
static const juce::Identifier command = "command";
static const juce::Identifier run_in_terminal = "run_in_terminal";

/*
 * Creates a new editor component for an application menu item.
 */
AppMenu::PopupEditor::PopupEditor(
        std::function<void() > onConfirm,
        bool showCategoryList,
        bool showCommandField) :
PopupEditorComponent([this, onConfirm](PopupEditorComponent* e)
{
    commitEdits();
    onConfirm();
}),
Locale::TextUser(localizedObjectKey),
nameLabel("nameLabel", localeText(name)),
titleEditor("titleEditor"),
iconLabel("iconLabel", localeText(icon_path)),
iconNameEditor(localeText(select_icon)),
categoryEditButton(localeText(edit_categories)),
commandLabel("commandLabel", localeText(command)),
commandEditor("commandEditor"),
terminalCheckboxLabel("runInTermLabel", localeText(run_in_terminal))
{
    using namespace juce;
    categoryEditButton.addListener(this);
    iconNameEditor.addFileSelectListener(this);
    
    ComponentConfigFile config;
    titleEditor.setFont(Font(config.getFontHeight
            (ComponentConfigFile::smallText)));
    commandEditor.setFont(Font(config.getFontHeight
            (ComponentConfigFile::smallText)));

    using Row = LayoutManager::Row;
    using RowItem = LayoutManager::RowItem;
    LayoutManager::Layout layout({
        Row(10,
        {
            RowItem(&nameLabel, 20),
            RowItem(&titleEditor, 41)
        }),
        Row(10,
        {
            RowItem(&iconLabel, 20),
            RowItem(&iconNameEditor, 30),
            RowItem(&iconPreview, 10)
        })
    });
    // Launch command row:
    if (showCommandField)
    {
        layout.addRow(Row(10,{
            RowItem(&commandLabel, 10),
            RowItem(&commandEditor, 20)
        }));
        layout.addRow(Row(10,{
            RowItem(&terminalCheckboxLabel, 60),
            RowItem(&terminalCheckbox, 10)
        }));
    }
    // Category list rows:
    if (showCategoryList)
    {

        layout.addRow(Row(10,{
            RowItem(10),
            RowItem(&categoryEditButton, 40),
            RowItem(10)
        }));
    }
    layout.setXMarginFraction(0.03);
    layout.setYMarginFraction(0.03);
    layout.setXPaddingWeight(1);
    layout.setYPaddingWeight(2);
    setLayout(layout);
}

/*
 * Gets the contents of the menu item name field.
 */
juce::String AppMenu::PopupEditor::getTitleField() const
{
    return titleEditor.getText();
}

/*
 * Gets the contents of the menu item icon name field.
 */
juce::String AppMenu::PopupEditor::getIconNameField() const
{
    return iconNameEditor.getText();
}

/*
 * Gets the list of application categories assigned in the category editor.
 */
const juce::StringArray& AppMenu::PopupEditor::getCategories() const
{
    return categories;
}

/*
 * Gets the contents of the command field.
 */
juce::String AppMenu::PopupEditor::getCommandField() const
{
    return commandEditor.getText();
}

/*
 * Gets the state of the terminal checkbox.
 */
bool AppMenu::PopupEditor::getTerminalCheckboxState() const
{
    return terminalCheckbox.getToggleState();
}

/*
 * Set the value stored in the editable name field.
 */
void AppMenu::PopupEditor::setTitleField(const juce::String& name)
{
    titleEditor.setText(name);
}

/*
 * Sets the value stored in the editable icon name field, and updates the 
 * preview icon.
 */
void AppMenu::PopupEditor::setIconField(const juce::String& icon)
{
    iconNameEditor.setText(icon);
    loadIconPreview();
}

/*
 * Sets the category values stored in the editable category list.
 */
void AppMenu::PopupEditor::setCategoryList(const juce::StringArray& categories)
{
    this->categories = categories;
}

/*
 * Sets the value displayed in the editable launch command field.
 */
void AppMenu::PopupEditor::setCommandField(const juce::String& command)
{
    commandEditor.setText(command);
}

/*
 * Sets the state of the "launch in terminal" checkbox.
 */
void AppMenu::PopupEditor::setTerminalCheckboxState(const bool launchInTerm)
{
    terminalCheckbox.setToggleState(launchInTerm,
            juce::NotificationType::dontSendNotification);
}

/*
 * Updates the icon preview component.
 */
void AppMenu::PopupEditor::loadIconPreview()
{
    using juce::String;
    using juce::Image;
    String iconName = getIconNameField();
    IconLoader iconThread;
    iconThread.loadIcon(iconName, iconPreview.getWidth(), [this](Image iconImg)
    {
        iconPreview.setImage(iconImg);
    });
}

/*
 * Triggers whenever the icon field is set to a new value.
 */
void AppMenu::PopupEditor::fileSelected(FileSelectTextEditor* iconEditor)
{
    loadIconPreview();
}

/*
 * Triggers whenever the category editor button is clicked.
 */
void AppMenu::PopupEditor::editorButtonClicked(juce::Button* button)
{
    using juce::StringArray;
    if (button == &categoryEditButton)
    {
        categoryEditor = new CategoryEditor(categories,
                [this](StringArray newCategories)
                {
                    categories = newCategories;
                });
        addAndMakeVisible(categoryEditor);
    }
}

