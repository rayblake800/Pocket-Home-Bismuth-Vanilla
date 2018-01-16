/**
 * @file ListEditor.h
 * @author Anthony Brown
 * 
 * ListEditor provides a UI component for editing a list of strings.
 * The user can add new strings, edit existing ones by double clicking them,
 * and delete list items.  These changes are copied into the underlying String
 * Array, which can be obtained using 
 */

#pragma once
#include "../GridLayoutManager.h"
#include "../../JuceLibraryCode/JuceHeader.h"

class ListEditor : public Component,
private Button::Listener,
private Label::Listener,
private ListBoxModel {
public:

    /**
     * Create a new ListEditor component
     * @param initialList the list begins with these list items
     * @param bgColour draw color of an unselected list item background
     * @param selectedBgColour draw color of a selected list item background
     * @param textColour draw color of list item text.
     */
    ListEditor(Array<String> initialList, Colour bgColour,
            Colour selectedBgColour, Colour textColour);

    virtual ~ListEditor();

    /**
     * @return the number of rows in the list
     */
    int getNumRows();

    /**
     * @return all list row strings.
     */
    Array<String> getListItems() const;

private:
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
        void resized() override;
    private:
        ImageButton delBtn;
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


    const Colour bgColour;
    const Colour selectedBgColour;
    const Colour textColour;


    Array<String> listItems;
    OwnedArray<ListItemComponent> listComponents;
    GridLayoutManager layoutManager;
    ListBox listContainer;
    TextEditor newItemField;
    TextButton addItemBtn;
};