#include "Utils.h"
#include "AppMenuPopupEditor.h"
#include "ComponentConfigFile.h"

AppMenuPopupEditor::AppMenuPopupEditor(
        juce::String title,
        std::function<void(AppMenuPopupEditor*) > onConfirm,
        bool showCategoryList,
        bool showCommandField) :
PopupEditorComponent(title,
[this, onConfirm](PopupEditorComponent* thisPopup)
{
    onConfirm(this);
}),
Localized("AppMenuPopupEditor"),
nameLabel("nameLabel", localeText(name)),
nameEditor("nameEditor"),
iconLabel("iconLabel", localeText(icon_path)),
iconPathEditor(localeText(select_icon)),
categoryEditButton(localeText(edit_categories)),
commandLabel("commandLabel", localeText(command)),
commandEditor("commandEditor"),
terminalCheckboxLabel("runInTermLabel", localeText(run_in_terminal))
{
    using namespace juce;
    categoryEditButton.addListener(this);
    iconPathEditor.addFileSelectListener(this);
    
    ComponentConfigFile config;
    nameEditor.setFont(Font(config.getFontHeight
            (ComponentConfigFile::smallText)));
    commandEditor.setFont(Font(config.getFontHeight
            (ComponentConfigFile::smallText)));

    using Row = LayoutManager::Row;
    using RowItem = LayoutManager::RowItem;
    LayoutManager::Layout layout({
        Row(10,
        {
            RowItem(&nameLabel, 20),
            RowItem(&nameEditor, 41)
        }),
        Row(10,
        {
            RowItem(&iconLabel, 20),
            RowItem(&iconPathEditor, 30),
            RowItem(&iconPreview, 10)
        })
    });
    //launch command row
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
    //category list rows:
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

AppMenuPopupEditor::~AppMenuPopupEditor() { }

/**
 * @return the contents of the editable name field 
 */
juce::String AppMenuPopupEditor::getNameField()
{
    return nameEditor.getText();
}

/**
 * @return the contents of the editable icon field 
 */
juce::String AppMenuPopupEditor::getIconField()
{
    return iconPathEditor.getText();
}

/**
 * @return the contents of the editable category list. 
 */
juce::StringArray AppMenuPopupEditor::getCategories()
{
    return categories;
}

/**
 * @return the contents of the editable launch command field.
 */
juce::String AppMenuPopupEditor::getCommandField()
{
    return commandEditor.getText();
}

/**
 * @return true if the command should run in the terminal
 */
bool AppMenuPopupEditor::launchInTerm()
{
    return terminalCheckbox.getToggleState();
}

/**
 * Set the value stored in the editable name field.
 */
void AppMenuPopupEditor::setNameField(juce::String name)
{
    nameEditor.setText(name);
}

/**
 * Set the value stored in the editable icon field, and update
 * the preview icon.
 */
void AppMenuPopupEditor::setIconField(juce::String icon)
{
    iconPathEditor.setText(icon);
}

/**
 * Set the values stored in the editable category list.
 */
void AppMenuPopupEditor::setCategories(juce::StringArray categories)
{
    this->categories = categories;
}

/**
 * Set the value stored in the editable launch command field.
 */
void AppMenuPopupEditor::setCommandField(juce::String command)
{
    commandEditor.setText(command);
}

/**
 * @param launchInTerm new value to apply to the terminal launch checkbox.
 */
void AppMenuPopupEditor::setTerminalCheckbox(bool launchInTerm)
{
    terminalCheckbox.setToggleState(launchInTerm,
            juce::NotificationType::dontSendNotification);
}

/**
 * Handles the category editor button.
 */
void AppMenuPopupEditor::editorButtonClicked(juce::Button* button)
{
    using namespace juce;
    if (button == &categoryEditButton)
    {
        categoryEditor = new CategoryPopupEditor(categories,
                [this](StringArray newCategories)
                {
                    categories = newCategories;
                });
        addAndMakeVisible(categoryEditor);
    }
}

/**
 * Triggers whenever the icon field is set to a new value.
 */
void AppMenuPopupEditor::fileSelected(FileSelectTextEditor* edited)
{
    using namespace juce;
    String newIcon = getIconField();
    IconThread iconThread;
    iconThread.loadIcon(newIcon, iconPreview.getWidth(), [this](Image iconImg)
    {
        iconPreview.setImage(iconImg);
    });
}


