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
            std::function<void() > onDelete,
            bool showCategoryList = true,
            bool showCommandField = true,
            bool showDeleteButton = true);

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
    bool keyPressed(const KeyPress &) override;
    
    /**
     * If the delete button/delete key is pressed, show a confirmation
     * window, and run the onDelete callback if the user confirms the
     * action.
     */
    void askToDelete();

    IconThread& iconThread;
    std::function<void() > onDelete;

    ScalingLabel nameLabel;
    TextEditor nameEditor;

    ScalingLabel iconLabel;
    DrawableImageComponent iconPreview;
    FileSelectTextEditor iconPathEditor;

    TextButton categoryEditButton;
    ScopedPointer<CategoryPopupEditor> categoryEditor;
    Array<String> categories;

    ScalingLabel commandLabel;
    TextEditor commandEditor;
    ScalingLabel terminalCheckboxLabel;
    ToggleButton terminalCheckbox;

    TextButton deleteButton;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuPopupEditor)
};