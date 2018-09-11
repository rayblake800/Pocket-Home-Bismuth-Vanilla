#pragma once
#include "LayoutManager.h"
#include "ListEditor.h"
#include "Localized.h"
#include "FileSelectTextEditor.h"
#include "PopupEditorComponent.h"
#include "CategoryPopupEditor.h"
#include "SwitchComponent.h"

/**
 * @file AppMenuPopupEditor.h
 * 
 * @brief AppMenuPopupEditor is a pop-up editor that provides a user interface 
 * for editing application and folder data.
 *
 * TODO Consider adding a description TextEdit field
 */

class IconThread;

class AppMenuPopupEditor : public PopupEditorComponent,
public FileSelectTextEditor::Listener, private Localized
{
public:
    /**
     * @param title             Text to print at the top of the pop-up editor 
     *                           window.
     * 
     * @param onConfirm         A callback function to run when the confirm 
     *                           button is pressed.
     * 
     * @param onDelete          A callback function to run when the delete 
     *                           button is pressed.
     * 
     * @param showCategoryList  Sets if the editor will contain button that
     *                           shows the category list.
     * 
     * @param showCommandField  Sets if the editor will contain a terminal
     *                           command field and an "open in terminal" check 
     *                           box.
     * 
     * @param showDeleteButton  Sets if the editor will contain a delete 
     *                           button.
     */
    AppMenuPopupEditor(juce::String title,
            std::function<void(AppMenuPopupEditor*) > onConfirm,
            bool showCategoryList = true,
            bool showCommandField = true);

    virtual ~AppMenuPopupEditor();

    /**
     * @return the contents of the editable name field 
     */
    juce::String getNameField();


    /**
     * @return the contents of the editable icon field 
     */
    juce::String getIconField();

    /**
     * @return the contents of the editable category list. 
     */
    juce::StringArray getCategories();

    /**
     * @return the contents of the editable launch command field.
     */
    juce::String getCommandField();

    /**
     * @return true if the command should run in the terminal
     */
    bool launchInTerm();

    /**
     * Set the value stored in the editable name field.
     */
    void setNameField(juce::String name);

    /**
     * Set the value stored in the editable icon field, and update
     * the preview icon.
     */
    void setIconField(juce::String icon);


    /**
     * Set the values stored in the editable category list.
     */
    void setCategories(juce::StringArray categories);

    /**
     * Set the value displayed in the editable launch command field.
     */
    void setCommandField(juce::String command);

    /**
     * Sets the state of the "launch in terminal" checkbox.
     */
    void setTerminalCheckbox(bool launchInTerm);


private:

    /**
     * Triggers whenever the icon field is set to a new value.
     * 
     * @param edited
     */
    void fileSelected(FileSelectTextEditor* edited);

    /**
     * Handles the category editor button.
     * 
     * @param button
     */
    void editorButtonClicked(juce::Button* button) override;

    ScalingLabel nameLabel; //text:"Name:"
    //Edits an application/folder display name
    juce::TextEditor nameEditor;

    ScalingLabel iconLabel; //text:"Icon path:"
    //Draws a preview of the selected icon
    DrawableImageComponent iconPreview;
    //Edits the application/folder icon
    FileSelectTextEditor iconPathEditor;

    //Launches a category editor pop-up
    juce::TextButton categoryEditButton; 
    //Holds the category editor when it's launched.
    juce::ScopedPointer<CategoryPopupEditor> categoryEditor;
    //List of editable categories associated with this application/folder
    juce::StringArray categories;

    ScalingLabel commandLabel; //text:"Command"
    //Edits the application launch command
    juce::TextEditor commandEditor;
    //Labels the terminal check
    ScalingLabel terminalCheckboxLabel; //text:"Run in terminal:"
    //Sets if this application launches as a terminal application
    SwitchComponent terminalCheckbox;
      
    //localized text keys;
    static const constexpr char * name = "name";
    static const constexpr char * icon_path = "icon_path";
    static const constexpr char * select_icon = "select_icon";
    static const constexpr char * edit_categories = "edit_categories";
    static const constexpr char * command = "command";
    static const constexpr char * run_in_terminal = "run_in_terminal";
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuPopupEditor)
};
