#define APPMENU_IMPLEMENTATION
#include "AppMenu_PopupEditor.h"
#include "Layout_Component_ConfigFile.h"
#include "Layout_Component_Manager.h"
#include "Layout_Component_JSONKeys.h"
#include "Layout_Component_TextSize.h"
#include "Icon_Loader.h"

/* Localized object class key: */
static const juce::Identifier localeClassKey = "AppMenu::PopupEditor";

/* Localized text value keys: */
namespace TextKey
{
    static const juce::Identifier name         = "name";
    static const juce::Identifier icon         = "icon";
    static const juce::Identifier selectIcon   = "selectIcon";
    static const juce::Identifier editCategory = "editCategory";
    static const juce::Identifier command      = "command";
    static const juce::Identifier runInTerm    = "runInTerm";
}

/*
 * Creates a new editor component for an application menu item.
 */
AppMenu::PopupEditor::PopupEditor(
        std::function<void() > onConfirm,
        bool showCategoryList,
        bool showCommandField) :
Widgets::PopupEditor([this, onConfirm](Widgets::PopupEditor* e)
{
    commitEdits();
    onConfirm();
}),
Locale::TextUser(localeClassKey),
nameLabel("nameLabel", localeText(TextKey::name)),
titleEditor("titleEditor"),
iconLabel("iconLabel", localeText(TextKey::icon)),
iconNameEditor(localeText(TextKey::selectIcon)),
categoryEditButton(localeText(TextKey::editCategory)),
commandLabel("commandLabel", localeText(TextKey::command)),
commandEditor("commandEditor"),
terminalCheckboxLabel("runInTermLabel", localeText(TextKey::runInTerm))
{
    categoryEditButton.addListener(this);
    iconNameEditor.addFileSelectListener(this);
    
    using TextSize = Layout::Component::TextSize;
    using juce::Font;
    Layout::Component::ConfigFile config;
    titleEditor.setFont(Font(config.getFontHeight(TextSize::smallText)));
    commandEditor.setFont(Font(config.getFontHeight(TextSize::smallText)));

    using namespace Layout::Group;
    RelativeLayout layout(
    {
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
            RowItem(&commandLabel, 20),
            RowItem(&commandEditor, 41)
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
 * Cancels any pending icon request.
 */
AppMenu::PopupEditor::~PopupEditor()
{
    if(iconRequestID != 0)
    {
        Icon::Loader iconLoader;
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
    Icon::Loader iconLoader;
    if(iconRequestID != 0)
    {
        iconLoader.cancelImageRequest(iconRequestID);
    }
    iconRequestID = iconLoader.loadIcon(iconName, iconPreview.getWidth(), 
        [this](Image iconImg)
    {
        iconRequestID = 0;
        iconPreview.setImage(iconImg);
    });
}

/*
 * Triggers whenever the icon field is set to a new value.
 */
void AppMenu::PopupEditor::fileSelected(Widgets::FilePathEditor* iconEditor)
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
        categoryEditor.reset(new CategoryEditor(categories, 
                [this](StringArray newCategories)
        {
            categories = newCategories;
        }));
        categoryEditor->setBounds(getLocalBounds());
        addAndMakeVisible(categoryEditor.get());
    }
}

/*
 * Ensures that the CategoryEditor bounds always match the main editor bounds.
 */
void AppMenu::PopupEditor::editorResized()
{
    if(categoryEditor != nullptr)
    {
        categoryEditor->setBounds(getLocalBounds());
    }
}
