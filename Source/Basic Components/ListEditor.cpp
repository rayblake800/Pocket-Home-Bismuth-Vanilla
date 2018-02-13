#include "../Utils.h"
#include "ListEditor.h"

ListEditor::ListEditor(Array<String> initialList) :
listItems(initialList),
listContainer("ListEditor", nullptr),
addItemBtn("+")
{
    updateColours();

    addAndMakeVisible(listContainer);
    addAndMakeVisible(newItemField);
    addAndMakeVisible(addItemBtn);

    listContainer.setModel(this);
    listContainer.setOutlineThickness(1);
    listContainer.addMouseListener(this, true);

    addItemBtn.addListener(this);
    
    layoutManager.addRow(3);
    layoutManager.addRow(1);
    layoutManager.addComponent(&listContainer, 0, 1);
    layoutManager.addComponent(&newItemField, 1, 4);
    layoutManager.addComponent(&addItemBtn, 1, 1);


}

ListEditor::~ListEditor()
{
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

/**
 * Replace the existing item list entries with new ones.
 */
void ListEditor::setListItems(Array<String> newItems)
{
    listItems = newItems;
    listContainer.updateContent();
    repaint();
}

void ListEditor::colourChanged()
{
    updateColours();
}

/**
 * Sets the colors of child components to match ListEditor colors
 */
void ListEditor::updateColours()
{
    addItemBtn.setColour(TextButton::textColourOnId, findColour(textColourId));
    listContainer.setColour(ListBox::backgroundColourId,
            findColour(backgroundColourId));
    listContainer.setColour(ListBox::outlineColourId,
            findColour(selectedListItemColourId));

    ScrollBar* scrollbar = listContainer.getVerticalScrollBar();
    scrollbar->setColour(ScrollBar::trackColourId,
            findColour(listItemColourId));
    scrollbar->setColour(ScrollBar::thumbColourId, findColour(textColourId));
}

ListEditor::ListItemComponent::ListItemComponent
(String text, ListEditor * owner) : Label(text),
delBtn("cancel.svg")
{
    setJustificationType(Justification::left);
    setButtonColour(owner->findColour(textColourId));
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

void ListEditor::ListItemComponent::setButtonColour(Colour colour)
{
    delBtn.setColour(DrawableImageButton::imageColour0Id, findColour(textColourId));
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
    ScrollBar* scrollbar = listContainer.getVerticalScrollBar();

    DBG(String("Scrollbar:") + scrollbar->getBounds().toString());
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
        if (existingComponent != nullptr)
        {
            delete existingComponent;
        }
        return nullptr;
    }
    if (rowLabel == nullptr)
    {
        rowLabel = new ListItemComponent(listItems[rowNumber], this);
        rowLabel->addListener(this);
    }
    rowLabel->setText(listItems[rowNumber],
            NotificationType::dontSendNotification);
    rowLabel->setColour(Label::textColourId, findColour(textColourId));
    rowLabel->setButtonComponentID(String(rowNumber));
    rowLabel->setButtonColour(findColour(textColourId));

    rowLabel->setComponentID(String(rowNumber));

    rowLabel->setFont(rowLabel->getFont().withHeight(rowLabel->getHeight() - 1));
    rowLabel->setColour(Label::backgroundColourId, isRowSelected ?
            findColour(selectedListItemColourId) : findColour(listItemColourId));
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
