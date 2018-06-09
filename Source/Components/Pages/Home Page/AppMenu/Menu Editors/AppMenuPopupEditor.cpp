#include "Utils.h"
#include "AppMenuPopupEditor.h"

AppMenuPopupEditor::AppMenuPopupEditor(
        String title,
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
    categoryEditButton.addListener(this);
    iconPathEditor.addFileSelectListener(this);

    RelativeLayoutManager::Layout layout = {
	.xMarginFraction = 0.1,
	.yMarginFraction = 0.1,
        .rows = {
            {
	        .rowWeight = 20,
		.yPaddingWeight = 2,
		.rowItems = {
                    {
			.component = &nameLabel,
			.componentWeight = 10,
			.xPaddingWeight = 2
	            },
                    {
			.component = &nameEditor,
			.componentWeight =  20,
			.xPaddingWeight = 2
	            }
                }
	    },
            {
	        .rowWeight = 20,
		.yPaddingWeight = 2,
		.rowItems = {
                    {
		        .component = &iconLabel,
		        .componentWeight = 20,
		        .xPaddingWeight = 2
		    },
                    {
		        .component = &iconPathEditor,
		        .componentWeight = 30,
		        .xPaddingWeight = 2
		    },
                    {
		        .component = &iconPreview,
		        .componentWeight = 10,
		        .xPaddingWeight = 2
		    }
                }
	    }
	}
    };
    //launch command row
    if (showCommandField)
    {
        layout.rows.push_back({
	    .rowWeight = 10,
	    .yPaddingWeight = 2,
	    .rowItems = {
                {
		    .component = &commandLabel,
		    .componentWeight = 10,
		    .xPaddingWeight = 2
		},
                {
		    .component = &commandEditor,
		    .componentWeight = 20,
		    .xPaddingWeight = 2
		}
	    }
        });
        layout.rows.push_back({
	    .rowWeight = 10,
	    .yPaddingWeight = 2,
	    .rowItems = {
                {
		    .component = &terminalCheckboxLabel,
		    .componentWeight = 60,
		    .xPaddingWeight = 2
		},
                {
		    .component = &terminalCheckbox,
		    .componentWeight = 10,
		    .xPaddingWeight = 2
		}
            }
	});
    }
    //category list rows:
    if (showCategoryList)
    {

        layout.rows.push_back({
	    .rowWeight = 10,
	    .yPaddingWeight = 2,
	    .rowItems = {
                {
		    .component = nullptr,
		    .componentWeight = 10,
		    .xPaddingWeight = 2
		},
                {
		    .component = &categoryEditButton,
		    .componentWeight = 40,
		    .xPaddingWeight = 2
		},
                {
		    .component = nullptr,
		    .componentWeight = 10,
		    .xPaddingWeight = 2
		}
            }
	});
    }
    setLayout(layout);
}

AppMenuPopupEditor::~AppMenuPopupEditor() { }

/**
 * @return the contents of the editable name field 
 */
String AppMenuPopupEditor::getNameField()
{
    return nameEditor.getText();
}

/**
 * @return the contents of the editable icon field 
 */
String AppMenuPopupEditor::getIconField()
{
    return iconPathEditor.getText();
}

/**
 * @return the contents of the editable category list. 
 */
StringArray AppMenuPopupEditor::getCategories()
{
    return categories;
}

/**
 * @return the contents of the editable launch command field.
 */
String AppMenuPopupEditor::getCommandField()
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
void AppMenuPopupEditor::setNameField(String name)
{
    nameEditor.setText(name);
}

/**
 * Set the value stored in the editable icon field, and update
 * the preview icon.
 */
void AppMenuPopupEditor::setIconField(String icon)
{
    iconPathEditor.setText(icon);
}

/**
 * Set the values stored in the editable category list.
 */
void AppMenuPopupEditor::setCategories(StringArray categories)
{
    this->categories = categories;
}

/**
 * Set the value stored in the editable launch command field.
 */
void AppMenuPopupEditor::setCommandField(String command)
{
    commandEditor.setText(command);
}

/**
 * @param launchInTerm new value to apply to the terminal launch checkbox.
 */
void AppMenuPopupEditor::setTerminalCheckbox(bool launchInTerm)
{
    terminalCheckbox.setToggleState(launchInTerm,
            NotificationType::dontSendNotification);
}

/**
 * Handles the category editor button.
 */
void AppMenuPopupEditor::editorButtonClicked(Button* button)
{
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
    String newIcon = getIconField();
    IconThread iconThread;
    iconThread.loadIcon(newIcon, iconPreview.getWidth(), [this](Image iconImg)
    {
        iconPreview.setImage(iconImg);
    });
}


