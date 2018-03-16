/**
 * @file AppMenuPopupEditor.h
 * 
 * AppMenuPopupEditor is a pop-up editor that provides a user interface for
 * editing application and folder data.
 */
#pragma once
#include "RelativeLayoutManager.h"
#include "ListEditor.h"
#include "FileSelectTextEditor.h"
#include "PopupEditorComponent.h"
#include "IconThread.h"
#include "CategoryPopupEditor.h"
#include "SwitchComponent.h"

/**
 * TODO:
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
     */
    void setNameField(String name);

    /**
     * Set the value stored in the editable icon field, and update
     * the preview icon.
     */
    void setIconField(String icon);


    /**
     * Set the values stored in the editable category list.
     */
    void setCategories(Array<String> categories);

    /**
     * Set the value displayed in the editable launch command field.
     */
    void setCommandField(String command);

    /**
     * Sets the state of the "launch in terminal" checkbox.
     */
    void setTerminalCheckbox(bool launchInTerm);


private:

    /**
     * Triggers whenever the icon field is set to a new value.
     * @param edited
     */
    void fileSelected(FileSelectTextEditor* edited);
    
    /**
     * Handles the category editor button.
     * @param button
     */
    void editorButtonClicked(Button* button) override;


    //Used for loading icon previews.
    IconThread& iconThread;

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
    SwitchComponent terminalCheckbox;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuPopupEditor)
};