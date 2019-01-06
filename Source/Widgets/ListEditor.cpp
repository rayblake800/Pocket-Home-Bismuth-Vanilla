#include "ListEditor.h"
#include "Layout_Component_ConfigFile.h"

ListEditor::ListEditor(juce::StringArray initialList) :
listItems(initialList),
listContainer("ListEditor", nullptr),
addItemBtn("+")
{
#    if JUCE_DEBUG
    setName("ListEditor");
#    endif

    using namespace Layout::Group;
    RelativeLayout layout(
    {
        Row(30, { RowItem(&listContainer) }),
        Row(10,
        {
            RowItem(&newItemField, 40),
            RowItem(&addItemBtn, 10)
        })
    });
    layout.setXPaddingWeight(1);
    layout.setYPaddingWeight(1);
    layout.setXMarginFraction(0.02);
    layout.setYMarginFraction(0.02);
    layoutManager.setLayout(layout, this);
    
    Layout::Component::ConfigFile config;
    newItemField.setFont(juce::Font(config.getFontHeight
            (Layout::Component::TextSize::smallText)));

    updateColours();

    listContainer.setModel(this);
    listContainer.setOutlineThickness(1);
    listContainer.addMouseListener(this, true);

    addItemBtn.addListener(this);
}

/*
 * Gets the number of rows in the list.
 */
int ListEditor::getNumRows()
{
    return listItems.size();
}

/*
 * Gets all list row strings.
 */
juce::StringArray ListEditor::getListItems() const
{
    return listItems;
}

/*
 * Replaces the existing item list entries with new ones.
 */
void ListEditor::setListItems(juce::StringArray newItems)
{
    listItems = newItems;
    listContainer.updateContent();
    repaint();
}

/*
 * Calls updateColours whenever component color values are changed. 
 */
void ListEditor::colourChanged()
{
    updateColours();
}

/*
 * Sets the colors of child components to match ListEditor colors
 */
void ListEditor::updateColours()
{
    using juce::TextButton;
    using juce::ListBox;
    using juce::ScrollBar;
    addItemBtn.setColour(TextButton::textColourOnId, findColour(textColourId));
    listContainer.setColour(ListBox::backgroundColourId,
            findColour(backgroundColourId));
    listContainer.setColour(ListBox::outlineColourId,
            findColour(selectedListItemColourId));

    ScrollBar& scrollbar = listContainer.getVerticalScrollBar();
    scrollbar.setColour(ScrollBar::trackColourId,
            findColour(listItemColourId));
    scrollbar.setColour(ScrollBar::thumbColourId, findColour(textColourId));
}

/*
 * Receives notifications when ListItemComponent text is changed.
 */
void ListEditor::labelTextChanged(juce::Label *source)
{
    ListItemComponent * listItem = dynamic_cast<ListItemComponent*> (source);
    if (listItem != nullptr)
    {
        int rowNum = listItem->getComponentID().getIntValue();
        listItems.getReference(rowNum) = listItem->getText(true);
        DBG("ListEditor::" << __func__ << ": Text set to " 
                << listItem->getText(true));
    }
    listContainer.updateContent();
    listContainer.repaint();
}

/*
 * Clicking a listBoxItem selects it.
 */
void ListEditor::listBoxItemClicked
(int row, const juce::MouseEvent & mouseEvent)
{
    listContainer.selectRow(row);
}

/*
 * Double clicking a listBoxItem makes it editable.
 */
void ListEditor::listBoxItemDoubleClicked
(int row, const juce::MouseEvent & mouseEvent)
{
    using juce::Label;
    Label * rowClicked =
            static_cast<Label*> (listContainer.getComponentForRowNumber(row));
    rowClicked->showEditor();
    juce::TextEditor * editor = rowClicked->getCurrentTextEditor();
    if (editor != nullptr)
    {
        editor->setBounds(rowClicked->getBounds());
    }
}

/*
 * Pressing the delete key removes the selected row.
 */
void ListEditor::deleteKeyPressed(int lastRowSelected)
{
    int selected = listContainer.getSelectedRow(0);
    removeRow(selected);
}

/*
 * Removes a string from the list and update the underlying ListBox.
 */
void ListEditor::removeRow(int rowNumber)
{
    listItems.remove(rowNumber);
    listContainer.updateContent();

}

ListEditor::ListItemComponent::ListItemComponent
(juce::String text, ListEditor * owner) : juce::Label(text),
delBtn("cancel.svg")
{
    setJustificationType(juce::Justification::left);
    setButtonColour(owner->findColour(textColourId));
    addAndMakeVisible(delBtn);
    delBtn.addListener(owner);
    delBtn.setWantsKeyboardFocus(false);
    setInterceptsMouseClicks(false, true);
}

void ListEditor::ListItemComponent::setButtonComponentID(juce::String id)
{
    delBtn.setComponentID(id);
}

/*
 * Sets button colour, used by the ListEditor to apply its color scheme to all 
 * list items.
 */
void ListEditor::ListItemComponent::setButtonColour(juce::Colour colour)
{
    delBtn.setColour(DrawableImageButton::imageColour0Id,
            findColour(textColourId));
}

/*
 * Updates the font and delete button to fit new bounds.
 */
void ListEditor::ListItemComponent::resized()
{
    setFont(getFont().withHeight(getHeight() - 1));

    delBtn.setBounds(getLocalBounds()
            .withWidth(getHeight()).withX(getRight() - getHeight())
            .reduced(2));
}

/*
 * Creates or recycles a list component to fit a list row.
 */
juce::Component * ListEditor::refreshComponentForRow
(int rowNumber, bool isRowSelected, juce::Component * existingComponent)
{
    using juce::String;
    using juce::Label;
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
            juce::NotificationType::dontSendNotification);
    rowLabel->setColour(Label::textColourId, findColour(textColourId));
    rowLabel->setButtonComponentID(String(rowNumber));
    rowLabel->setButtonColour(findColour(textColourId));

    rowLabel->setComponentID(String(rowNumber));

    rowLabel->setFont(rowLabel->getFont()
            .withHeight(rowLabel->getHeight() - 1));
    rowLabel->setColour(Label::backgroundColourId, isRowSelected ?
            findColour(selectedListItemColourId) 
            : findColour(listItemColourId));
    return rowLabel;
}

/*
 * Handles the add and delete item buttons.
 */
void ListEditor::buttonClicked(juce::Button* buttonClicked)
{
    if (buttonClicked == &addItemBtn)
    {
        juce::String newListItem = newItemField.getText();
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

/*
 * Re-applies the layout and adjusts list item height to fit the new
 * bounds.
 */
void ListEditor::resized()
{
    layoutManager.layoutComponents(getLocalBounds());
    listContainer.setRowHeight(newItemField.getHeight());
}
