/**
 * @file AppMenuPopupEditor.h
 * 
 * AppMenuPopupEditor is a pop-up editor that provides a user interface for
 * editing application and folder data.
 */
#pragma once
#include "../../GridLayoutManager.h"
#include "../IconThread.h"
#include "../../Basic Components/ListEditor.h"
#include "../../Basic Components/FileSelectTextEditor.h"
#include "CategoryPopupEditor.h"
#include "PopupEditorComponent.h"

/**
 * TODO:
 * - Consider removing the delete button
 * - Consider adding a description TextEdit field
 */

class IconThread;

class AppMenuPopupEditor : public PopupEditorComponent,
public FileSelectTextEditor::Listener {
public:
    /**
     * @param title is printed at the top of the pop-up editor window
     * @param iconThread is used for loading preview icons
     * @param onConfirm is the callback function to run when the confirm button
     * is pressed.
     * @param onDelete is the callback function to run when the delete button 
     * is pressed.
     * @param showCategoryList sets if the editor will contain button that
     * shows the category list.
     * @param showCommandField sets if the editor will contain a terminal
     * command field and an "open in terminal" check box.
     * @param showDeleteButton sets if the editor will contain a delete button.
     */
    AppMenuPopupEditor(String title, IconThread& iconThread,
            std::function<void(AppMenuPopupEditor*) > onConfirm,
            bool showCategoryList = true,
            bool showCommandField = true);

    virtual ~AppMenuPopupEditor();

    /**
     * @return the contents of the editable name field 
     */
    String getNameField();


    /**
     * @return the contents of the editable icon field 
     */
    String getIconField();

    /**
     * @return the contents of the editable category list. 
     */
    Array<String> getCategories();

    /**
     * @return the contents of the editable launch command field.
     */
    String getCommandField();

    /**
     * @return true if the command should run in the terminal
     */
    bool launchInTerm();

    /**
     * Set the value stored in the editable name field.
     * @param name
     */
    void setNameField(String name);

    /**
     * Set the value stored in the editable icon field, and update
     * the preview icon.
     * @param icon
     */
    void setIconField(String icon);


    /**
     * Set the values stored in the editable category list.
     * @param categories
     */
    void setCategories(Array<String> categories);

    /**
     * Set the value stored in the editable launch command field.
     * @param command
     */
    void setCommandField(String command);

    /**
     * @param launchInTerm new value to apply to the terminal launch checkbox.
     */
    void setTerminalCheckbox(bool launchInTerm);


private:

    /**
     * Triggers whenever the icon field is set to a new value.
     * @param edited
     */
    void fileSelected(FileSelectTextEditor* edited);
    /**
     * Handles the delete, confirm, and cancel buttons.
     * @param buttonClicked
     */
    void buttonClicked(Button* buttonClicked) override;
    /**
     * If the delete button is drawn, run askToDelete() on backspace/delete
     * key events. Otherwise, handle key events as usual for a PopupEditor.
     */
    bool keyPressed(const KeyPress &) override;
    
    /**
     * If the delete button/delete key is pressed, show a confirmation
     * window, and run the onDelete callback if the user confirms the
     * action.
     */
    void askToDelete();

    //Used for loading icon previews.
    IconThread& iconThread;
    //Callback function for deleting the edited item, set on construction
    std::function<void() > onDelete;

    ScalingLabel nameLabel;//text:"Name:"
    //Edits an application/folder display name
    TextEditor nameEditor;

    ScalingLabel iconLabel;//text:"Icon path:"
    //Draws a preview of the selected icon
    DrawableImageComponent iconPreview;
    //Edits the application/folder icon
    FileSelectTextEditor iconPathEditor;

    //Launches a category editor pop-up
    TextButton categoryEditButton;//
    //Holds the category editor when it's launched.
    ScopedPointer<CategoryPopupEditor> categoryEditor;
    //List of editable categories associated with this application/folder
    Array<String> categories;

    ScalingLabel commandLabel;//text:"Command"
    //Edits the application launch command
    TextEditor commandEditor;
    //Labels the terminal check
    ScalingLabel terminalCheckboxLabel;//text:"Run in terminal:"
    //Sets if this application launches as a terminal application
    ToggleButton terminalCheckbox;
    //runs askToDelete()
    TextButton deleteButton;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuPopupEditor)
};