/*
  ==============================================================================

    ListEditor.cpp
    Created: 14 Jan 2018 12:52:45pm
    Author:  anthony

  ==============================================================================
 */

#include "../Utils.h"
#include "ListEditor.h"

ListEditor::ListEditor(Array<String> initialList, Colour bgColour,
        Colour selectedBgColour, Colour textColour) :
listItems(initialList),
listContainer("ListEditor", nullptr),
addItemBtn("+"),
bgColour(bgColour),
selectedBgColour(selectedBgColour),
textColour(textColour)
{
    addAndMakeVisible(listContainer);
    addAndMakeVisible(newItemField);
    addAndMakeVisible(addItemBtn);

    listContainer.setModel(this);
    listContainer.addMouseListener(this, true);

    addItemBtn.setColour(TextButton::textColourOnId, textColour);
    addItemBtn.addListener(this);


    layoutManager.addRow(3);
    layoutManager.addRow(1);
    layoutManager.addComponent(&listContainer, 0, 1);
    layoutManager.addComponent(&newItemField, 1, 4);
    layoutManager.addComponent(&addItemBtn, 1, 1);
}

ListEditor::~ListEditor()
{
    listItems.clear();
}

/**
 * @return the number of rows in the list
 */
int ListEditor::getNumRows()
{
    return listItems.size();
}

/**
 * @return all list row strings.
 */
Array<String> ListEditor::getListItems() const
{
    return listItems;
}

ListEditor::ListItemComponent::ListItemComponent
(String text, ListEditor * owner) : Label(text),
delBtn("x")
{
    setJustificationType(Justification::left);
    delBtn.setImages(true, true, true,
            createImageFromFile(assetFile("cancel.svg")),
            1.0f, Colours::transparentWhite, // normal
            Image::null, 1.0f, Colours::transparentWhite, // over
            Image::null, 0.5f, Colours::transparentWhite, // down
            0);
    addAndMakeVisible(delBtn);
    delBtn.addListener(owner);
    delBtn.setWantsKeyboardFocus(false);
    setInterceptsMouseClicks(false, true);
}

ListEditor::ListItemComponent::~ListItemComponent()
{
}

void ListEditor::ListItemComponent::setButtonComponentID(String id)
{
    delBtn.setComponentID(id);
}

void ListEditor::ListItemComponent::resized()
{
    setFont(getFont().withHeight(getHeight() - 1));
    delBtn.setBounds(getLocalBounds()
            .withWidth(getHeight()).withX(getRight() - getHeight())
            .reduced(2));

}

void ListEditor::buttonClicked(Button* buttonClicked)
{
    if (buttonClicked == &addItemBtn)
    {
        String newListItem = newItemField.getText();
        if (newListItem.isNotEmpty())
        {
            listItems.add(newListItem);
            newItemField.clear();
            listContainer.updateContent();
            listContainer.repaint();
        }
    }//otherwise it's a delete button
    else
    {
        int buttonRow = buttonClicked->getComponentID().getIntValue();
        removeRow(buttonRow);
    }
}

void ListEditor::listBoxItemDoubleClicked
(int row, const MouseEvent & mouseEvent)
{
    DBG("Double Clicked");
    Label * rowClicked =
            static_cast<Label*> (listContainer.getComponentForRowNumber(row));
    rowClicked->showEditor();
    TextEditor * editor = rowClicked->getCurrentTextEditor();
    if (editor != nullptr)
    {
        editor->setBounds(rowClicked->getBounds());
    }
}

void ListEditor::resized()
{
    layoutManager.layoutComponents(getLocalBounds(), 0, 2);
    listContainer.setRowHeight(newItemField.getHeight());
    DBG(String("ListEditor:") + getBounds().toString());
}

/**
 * Receives notifications when ListItemComponent text is changed.
 */
void ListEditor::labelTextChanged(Label *source)
{
    ListItemComponent * listItem = dynamic_cast<ListItemComponent*> (source);
    if (listItem != nullptr)
    {
        int rowNum = listItem->getComponentID().getIntValue();
        listItems.getReference(rowNum) = listItem->getText(true);
        DBG(String("Text set to ") + listItem->getText(true));
    }
    listContainer.updateContent();
    listContainer.repaint();
}

void ListEditor::paintListBoxItem
(int rowNumber, Graphics&g, int width, int height, bool rowIsSelected)
{

}

Component * ListEditor::refreshComponentForRow
(int rowNumber, bool isRowSelected, Component * existingComponent)
{
    ListItemComponent * rowLabel =
            static_cast<ListItemComponent*> (existingComponent);
    if (rowNumber >= getNumRows())
    {
        return nullptr;
    }
    if (rowLabel == nullptr)
    {
        rowLabel = new ListItemComponent(listItems[rowNumber], this);
        listComponents.add(rowLabel);
        rowLabel->setColour(Label::textColourId, textColour);
        rowLabel->addListener(this);
    }
    rowLabel->setText(listItems[rowNumber],
            NotificationType::dontSendNotification);
    rowLabel->setButtonComponentID(String(rowNumber));
    rowLabel->setComponentID(String(rowNumber));

    rowLabel->setFont(rowLabel->getFont().withHeight(rowLabel->getHeight() - 1));
    rowLabel->setColour(Label::backgroundColourId, isRowSelected ?
            selectedBgColour : bgColour);
    return rowLabel;

}

void ListEditor::listBoxItemClicked
(int row, const MouseEvent & mouseEvent)
{
    DBG(String("Clicked ") + String(row));
    listContainer.selectRow(row);
}

void ListEditor::deleteKeyPressed(int lastRowSelected)
{
    int selected = listContainer.getSelectedRow(0);
    removeRow(selected);

}

void ListEditor::removeRow(int rowNumber)
{
    listItems.remove(rowNumber);
    listContainer.updateContent();

}
