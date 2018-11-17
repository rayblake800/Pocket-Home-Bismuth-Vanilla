#define APPMENU_IMPLEMENTATION_ONLY
#include "Utils.h"
#include "ComponentConfigFile.h"
#include "AppMenu/Components/Editors/PopupEditor.h"

/* Localized object class key: */
static const juce::Identifier localeClassKey = "AppMenu::PopupEditor";

/* Localized text value keys: */
static const juce::Identifier nameTextKey         = "name";
static const juce::Identifier iconTextKey         = "icon";
static const juce::Identifier selectIconTextKey   = "selectIcon";
static const juce::Identifier editCategoryTextKey = "editCategory";
static const juce::Identifier commandTextKey      = "command";
static const juce::Identifier runInTermTextKey    = "runInTerm";

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
Locale::TextUser(localeClassKey),
nameLabel("nameLabel", localeText(nameTextKey)),
titleEditor("titleEditor"),
iconLabel("iconLabel", localeText(iconTextKey)),
iconNameEditor(localeText(selectIconTextKey)),
categoryEditButton(localeText(editCategoryTextKey)),
commandLabel("commandLabel", localeText(commandTextKey)),
commandEditor("commandEditor"),
terminalCheckboxLabel("runInTermLabel", localeText(runInTermTextKey))
{
    using juce::Font;
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
 * Cancel any pending icon request.
 */
AppMenu::PopupEditor::~PopupEditor()
{
    if(iconRequestID != 0)
    {
        IconLoader iconLoader;
        iconLoader.cancelImageRequest(iconRequestID);
        iconRequestID = 0;
    }
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
 * Sets the value stored in the editable title field.
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
    if(iconRequestID != 0)
    {
        iconThread.cancelImageRequest(iconRequestID);
    }
    iconRequestID = iconThread.loadIcon(iconName, iconPreview.getWidth(), 
        [this](Image iconImg)
        {
            iconRequestID = 0;
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

