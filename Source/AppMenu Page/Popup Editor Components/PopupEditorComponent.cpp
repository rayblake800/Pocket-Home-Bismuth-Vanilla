/*
  ==============================================================================

    PopupEditorComponent.cpp
    Created: 12 Jan 2018 9:53:18pm
    Author:  anthony

  ==============================================================================
 */
#include "../../Utils.h"
#include "../../PocketHomeApplication.h"
#include "PopupEditorComponent.h"

PopupEditorComponent::PopupEditorComponent
(String title, IconThread& iconThread,
        bool showCategoryList, bool showCommandField, bool showDeleteButton) :
ConfigurableImageComponent(ComponentConfigFile::popupMenuKey,
0, RectanglePlacement::stretchToFit),
iconThread(iconThread),
titleLabel("EditorTitle", title, 2),
nameLabel("nameLabel", "Name:"),
nameEditor("nameEditor"),
iconLabel("iconLabel", "Icon path:"),
iconPathEditor("iconPathEditor"),
categoryLabel("categoryLabel", "Categories:"),
categoryList({}),
commandLabel("commandLabel", "Command:"),
commandEditor("commandEditor"),
terminalCheckboxLabel("runInTermLabel","Run in terminal:"),
deleteButton("Delete"),
cancelBtn("cancel.svg"),
confirmBtn("confirm.svg")
{
    setName("popupEditor");
    loadAllConfigProperties();

    std::vector<ScalingLabel*> labels = {
        &titleLabel, &nameLabel, &iconLabel,&terminalCheckboxLabel, &categoryLabel
    };
    std::vector<Component*> editors = {
        &nameEditor, &iconPathEditor, &commandEditor
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

    categoryList.setColours(bgColour, bgColour, selectionColour, textColour);
    deleteButton.setColour(TextButton::textColourOnId, textColour);
    deleteButton.setColour(TextButton::textColourOffId, textColour);
    deleteButton.setColour(TextButton::buttonColourId, selectionColour);
    deleteButton.setColour(TextButton::buttonOnColourId, bgColour);
    cancelBtn.replaceColour(Colours::black, textColour);
    confirmBtn.replaceColour(Colours::black, textColour);
    titleLabel.setJustificationType(Justification::centred);
    
    
    deleteButton.addListener(this);
    cancelBtn.addListener(this);
    confirmBtn.addListener(this);
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
    //category list rows:
    if (showCategoryList)
    {
        addRow(1);
        layoutManager.addComponent(&categoryLabel, currentRow, 1, this);
        addRow(3);
        layoutManager.addComponent(&categoryList, currentRow, 1, this);
    }
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
    //delete button
    if (showDeleteButton)
    {
        addRow(1);
        layoutManager.addComponent(nullptr, currentRow, 1, this);
        layoutManager.addComponent(&deleteButton, currentRow, 2, this);
        layoutManager.addComponent(nullptr, currentRow, 1, this);
    }
    //confirm/cancel buttons
    addRow(1);
    layoutManager.addComponent(&cancelBtn, currentRow, 1, this);
    layoutManager.addComponent(&confirmBtn, currentRow, 1, this);

    setInterceptsMouseClicks(true, true);
    resized();
    MessageManager::callAsync([this]
    {
        this->grabKeyboardFocus();
    });
}

PopupEditorComponent::~PopupEditorComponent()
{
}

void PopupEditorComponent::closePopup()
{
    setVisible(false);
    getParentComponent()->removeChildComponent(this);
}

/**
 * run the parent class version to set image colors, then save
 * text color
 */
void PopupEditorComponent::applyConfigAssets(Array<String> assetNames,
        Array<Colour> colours)
{
    this->ConfigurableImageComponent::applyConfigAssets(assetNames, colours);
    while (colours.size() < 3)
    {
        colours.add(Colours::transparentBlack);
    }
    selectionColour = colours[0];
    bgColour = colours[1];
    textColour = colours[2];
}

/**
 * @return the contents of the editable name field 
 */
String PopupEditorComponent::getNameField()
{
    return nameEditor.getText();
}

/**
 * @return the contents of the editable icon field 
 */
String PopupEditorComponent::getIconField()
{
    return iconPathEditor.getText();
}

/**
 * @return the contents of the editable category list. 
 */
Array<String> PopupEditorComponent::getCategories()
{
    return categoryList.getListItems();
}

/**
 * @return the contents of the editable launch command field.
 */
String PopupEditorComponent::getCommandField()
{
    return commandEditor.getText();
}

/**
 * @return true if the command should run in the terminal
 */
bool PopupEditorComponent::launchInTerm()
{
    return terminalCheckbox.getToggleState();
}

/**
 * Set the value stored in the editable name field.
 */
void PopupEditorComponent::setName(String name)
{
    nameEditor.setText(name);
}

/**
 * Set the value stored in the editable icon field, and update
 * the preview icon.
 */
void PopupEditorComponent::setIcon(String icon)
{
    iconPathEditor.setText(icon);
}

/**
 * Set the values stored in the editable category list.
 */
void PopupEditorComponent::setCategories(Array<String> categories)
{
    categoryList.setListItems(categories);
}

/**
 * Set the value stored in the editable launch command field.
 */
void PopupEditorComponent::setCommandField(String command)
{
    commandEditor.setText(command);
}

/**
 * @param launchInTerm new value to apply to the terminal launch checkbox.
 */
void PopupEditorComponent::setTerminalCheckbox(bool launchInTerm)
{
    terminalCheckbox.setToggleState(launchInTerm,
            NotificationType::dontSendNotification);
}

void PopupEditorComponent::buttonClicked(Button* buttonClicked)
{
    if (buttonClicked == &cancelBtn)
    {
        cancel();

    }
    else if (buttonClicked == &confirmBtn)
    {
        confirm();
    } else if (buttonClicked == &deleteButton)
    {
        deleteBtnPressed();
        closePopup();
    }
}

void PopupEditorComponent::resized()
{
    this->ConfigurableImageComponent::resized();
    layoutManager.layoutComponents(getLocalBounds().reduced(2), 3, 4);
}

bool PopupEditorComponent::keyPressed(const KeyPress & key)
{
    if (key == KeyPress::escapeKey)
    {
        cancel();
        return true;
    } else if (key == KeyPress::returnKey)
    {
        confirm();
        return true;
    }
    return false;
}

void PopupEditorComponent::cancel()
{
    DBG("Canceled!");
    closePopup();
}

/**
 * Triggers whenever the icon field is set to a new value.
 */
void PopupEditorComponent::fileSelected(FileSelectTextEditor* edited)
{
    String newIcon = getIconField();
    iconThread.loadIcon(newIcon, [this](Image iconImg)
    {
        iconPreview.setImage(iconImg);
    });
}


