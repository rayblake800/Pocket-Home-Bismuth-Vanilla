/**
 * @file ListEditor.h
 * 
 * ListEditor provides a UI component for editing a list of strings.
 * The user can add new strings, edit existing ones by double clicking them,
 * and delete list items.  These changes are copied into the underlying String
 * Array, which can be obtained using 
 */

#pragma once
#include "../GridLayoutManager.h"
#include "DrawableImageButton.h"
#include "../../JuceLibraryCode/JuceHeader.h"

class ListEditor : public Component,
private Button::Listener,
private Label::Listener,
private ListBoxModel {
public:

    /**
     * Create a new ListEditor component
     * @param initialList the list begins with these list items
     */
    ListEditor(Array<String> initialList);

    virtual ~ListEditor();

    enum ColourIds {
        backgroundColourId = 0x1900100,
        listItemColourId = 0x1900101,
        selectedListItemColourId = 0x1900102,
        textColourId = 0x1900103
    };

    /**
     * @return the number of rows in the list
     */
    int getNumRows();

    /**
     * @return all list row strings.
     */
    Array<String> getListItems() const;

    /**
     * Replace the existing item list entries with new ones.
     * @param newItems replacement list of Strings
     */
    void setListItems(Array<String> newItems);

    void colourChanged() override;

private:
    
    /**
     * Sets the colors of child components to match ListEditor colors
     */
    void updateColours();
    
    /**
     * Receives notifications when ListItemComponent text is changed.
     * These changes are then copied back to the appropriate string in the list
     * items.
     * @param source the label component for one of the list rows.
     */
    void labelTextChanged(Label *source);

    //Clicking a listBoxItem selects it.
    void listBoxItemClicked(int row, const MouseEvent& mouseEvent);

    //Double clicking a listBoxItem makes it editable.
    virtual void listBoxItemDoubleClicked
    (int row, const MouseEvent & mouseEvent);

    //Pressing the delete key removes the selected row.
    void deleteKeyPressed(int lastRowSelected);

    //Remove a string from the list and update the underlying ListBox
    void removeRow(int rowNumber);

    //Custom ListBox item component class, basically just a label with a 
    //delete button.

    class ListItemComponent : public Label {
    public:
        ListItemComponent(String text, ListEditor * owner);
        virtual ~ListItemComponent();
        //button component id should be set to String(rowIndex)
        void setButtonComponentID(String id);
        void setButtonColour(Colour colour);
        void resized() override;
    private:
        DrawableImageButton delBtn;
    };

    //Updates ListBox rows
    Component* refreshComponentForRow(int rowNumber,
            bool isRowSelected,
            Component * existingComponentToUpdate);

    //Handles add and delete item buttons
    void buttonClicked(Button* buttonClicked) override;


    void paintListBoxItem(int rowNumber, Graphics&g, int width,
            int height, bool rowIsSelected);
    void resized() override;


    Array<String> listItems;
    GridLayoutManager layoutManager;
    ListBox listContainer;
    TextEditor newItemField;
    TextButton addItemBtn;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ListEditor)
};