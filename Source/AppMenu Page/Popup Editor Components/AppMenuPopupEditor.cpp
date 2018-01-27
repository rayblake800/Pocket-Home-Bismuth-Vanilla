/*
  ==============================================================================

    PopupEditorComponent.cpp
    Created: 12 Jan 2018 9:53:18pm
    Author:  anthony

  ==============================================================================
 */
#include "../../Utils.h"
#include "../../PocketHomeApplication.h"
#include "AppMenuPopupEditor.h"

AppMenuPopupEditor::AppMenuPopupEditor
(String title, IconThread& iconThread,
        std::function<void(AppMenuPopupEditor*) > onConfirm,
        std::function<void() > onDelete,
        bool showCategoryList, bool showCommandField, bool showDeleteButton) :
PopupEditorComponent(title, [this, onConfirm](PopupEditorComponent* thisPopup)
{

    onConfirm(this);
}),
iconThread(iconThread),
onDelete(onDelete),
nameLabel("nameLabel", "Name:"),
nameEditor("nameEditor"),
iconLabel("iconLabel", "Icon path:"),
iconPathEditor("iconPathEditor"),
categoryEditButton("Edit categories"),
commandLabel("commandLabel", "Command:"),
commandEditor("commandEditor"),
terminalCheckboxLabel("runInTermLabel", "Run in terminal:"),
deleteButton("Delete")
{

    std::vector<ScalingLabel*> labels = {
        &nameLabel, &iconLabel, &terminalCheckboxLabel
    };
    std::vector<Component*> editors = {
        &nameEditor, &iconPathEditor, &commandEditor
    };

    std::vector<TextButton*> textButtons = {
        &categoryEditButton, &deleteButton
    };

    //set colours:
    for (ScalingLabel* label : labels)
    {
        label->setColour(Label::textColourId, textColour);
    }

    for (Component* editor : editors)
    {
        editor->setColour(TextEditor::textColourId, textColour);
    }

    for (TextButton* button : textButtons)
    {
        button->setColour(TextButton::textColourOnId, textColour);
        button->setColour(TextButton::textColourOffId, textColour);
        button->setColour(TextButton::buttonColourId, selectionColour);
        button->setColour(TextButton::buttonOnColourId, bgColour);
        button->addListener(this);
    }

    iconPathEditor.addFileSelectListener(this);

    //build component layout
    int currentRow = -1;
    std::function<void(int) > addRow = [this, &currentRow](int rowWeight)
    {
        layoutManager.addRow(rowWeight);
        currentRow++;
    };
    //title row:
    addRow(1);
    layoutManager.addComponent(&titleLabel, currentRow, 1, this);
    //name row:
    addRow(1);
    layoutManager.addComponent(&nameLabel, currentRow, 1, this);
    layoutManager.addComponent(&nameEditor, currentRow, 2, this);
    //icon row:
    addRow(1);
    layoutManager.addComponent(&iconLabel, currentRow, 2, this);
    layoutManager.addComponent(&iconPathEditor, currentRow, 3, this);
    layoutManager.addComponent(&iconPreview, currentRow, 1, this);
    //launch command row
    if (showCommandField)
    {
        addRow(1);
        layoutManager.addComponent(&commandLabel, currentRow, 1, this);
        layoutManager.addComponent(&commandEditor, currentRow, 2, this);
        addRow(1);
        layoutManager.addComponent(&terminalCheckboxLabel, currentRow, 6, this);
        layoutManager.addComponent(&terminalCheckbox, currentRow, 1, this);
    }
    //category list rows:
    if (showCategoryList)
    {
        addRow(1);
        layoutManager.addComponent(nullptr, currentRow, 1, this);
        layoutManager.addComponent(&categoryEditButton, currentRow, 4, this);
        layoutManager.addComponent(nullptr, currentRow, 1, this);
    }
    //delete button
    if (showDeleteButton)
    {
        addRow(1);
        layoutManager.addComponent(nullptr, currentRow, 1, this);
        layoutManager.addComponent(&deleteButton, currentRow, 4, this);
        layoutManager.addComponent(nullptr, currentRow, 1, this);
    }
    //confirm/cancel buttons
    addRow(1);
    layoutManager.addComponent(&cancelButton, currentRow, 1, this);
    layoutManager.addComponent(&confirmButton, currentRow, 1, this);

    resized();
}

AppMenuPopupEditor::~AppMenuPopupEditor()
{
}

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
Array<String> AppMenuPopupEditor::getCategories()
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
void AppMenuPopupEditor::setCategories(Array<String> categories)
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

void AppMenuPopupEditor::buttonClicked(Button* buttonClicked)
{
    if (buttonClicked == &deleteButton)
    {
        askToDelete();
    } else if (buttonClicked == &categoryEditButton)
    {
        categoryEditor = new CategoryPopupEditor(categories,
                [this](Array<String> newCategories)
                {
                    categories = newCategories;
                });
        addAndMakeVisible(categoryEditor);
    } else
    {
        this->PopupEditorComponent::buttonClicked(buttonClicked);
    }
}

bool AppMenuPopupEditor::keyPressed(const KeyPress & key)
{
    if (key == KeyPress::backspaceKey || key == KeyPress::deleteKey)
    {
        askToDelete();
        return true;
    } else
    {
        return this->PopupEditorComponent::keyPressed(key);
    }
}

/**
 * Triggers whenever the icon field is set to a new value.
 */
void AppMenuPopupEditor::fileSelected(FileSelectTextEditor* edited)
{
    String newIcon = getIconField();
    iconThread.loadIcon(newIcon, [this](Image iconImg)
    {
        iconPreview.setImage(iconImg);
    });
}

/**
 * If the delete button/delete key is pressed, show a confirmation
 * window, and run the onDelete callback if the user confirms the
 * action.
 */
void AppMenuPopupEditor::askToDelete()
{
    confirmAction("Delete", "Delete this menu item?",
            [this](){
                onDelete();
                closePopup();
            });
}


