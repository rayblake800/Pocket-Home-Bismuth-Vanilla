#ifndef APPMENU_IMPLEMENTATION
  #error File included directly outside of AppMenu implementation.
#endif
#pragma once
/**
 * @file  AppMenu_PopupEditor.h
 * 
 * @brief  A pop-up editor component that provides a user interface for creating
 *         and editing application menu items.
 */

#include "Locale/TextUser.h"
#include "Widgets/FileSelectTextEditor.h"
#include "Widgets/PopupEditorComponent.h"
#include "Widgets/SwitchComponent.h"
#include "Icon_RequestID.h"
#include "AppMenu_CategoryEditor.h"

namespace AppMenu { class PopupEditor; }

/**
 *  The PopupEditor provides editor components for setting all relevant menu 
 * item properties of a new or existing menu item. The user may edit any of 
 * these data fields, then press the cancel button to discard all changes, or 
 * the confirm button to save changes.
 *
 *  All saved changes are applied by the abstract commitEdits function, which
 * subclasses should implement to create or edit whatever types of menu items
 * they handle.
 */
class AppMenu::PopupEditor : public PopupEditorComponent,
    public FileSelectTextEditor::Listener, public Locale::TextUser
{
public:
    /**
     * @brief  Creates a new editor component for an application menu item.
     *
     * @param onConfirm         An optional callback function to run when the 
     *                          confirm button is pressed.
     * 
     * @param showCategoryList  Sets if the editor will contain a button that
     *                          shows the category list.
     * 
     * @param showCommandField  Sets if the editor will contain a terminal
     *                          command field and an "open in terminal" check 
     *                          box.
     */
    PopupEditor(
            const std::function<void()> onConfirm = [](){},
            bool showCategoryList = true,
            bool showCommandField = true);

    /**
     * @brief  Cancels any pending icon request.
     */
    virtual ~PopupEditor();

    /**
     * @brief  Gets the contents of the menu item title field.
     *
     * @return  The contents of the editable title field. 
     */
    juce::String getTitleField() const;

    /**
     * @brief  Gets the contents of the menu item icon name field.
     *
     * @return  The contents of the editable icon field.
     */
    juce::String getIconNameField() const;

    /**
     * @brief  Gets the list of application categories assigned in the category
     *         editor.
     *
     * @return  The contents of the editable category list. 
     */
    const juce::StringArray& getCategories() const;

    /**
     * @brief  Gets the contents of the command field.
     *
     * @return  The contents of the editable launch command field.
     */
    juce::String getCommandField() const;

    /**
     * @brief  Gets the state of the terminal checkbox.
     *
     * @return  Whether the checkbox is selected and the command should run in 
     *          the terminal.
     */
    bool getTerminalCheckboxState() const;

    /**
     * @brief  Set the value stored in the editable title field.
     *
     * @param name  The new title to copy into the title field.
     */
    void setTitleField(const juce::String& title);

    /**
     * @brief  Sets the value stored in the editable icon name field, and 
     *         updates the preview icon.
     *
     * @param icon  The new icon name or path.
     */
    void setIconField(const juce::String& icon);

    /**
     * @brief  Sets the category values stored in the editable category list.
     *
     * @param categories  The new list of application categories to show in the
     *                    editor.
     */
    void setCategoryList(const juce::StringArray& categories);

    /**
     * @brief  Sets the value displayed in the editable launch command field.
     *
     * @param command  The new launch command to copy into the command field.
     */
    void setCommandField(const juce::String& command);

    /**
     * Sets the state of the "launch in terminal" checkbox.
     */
    void setTerminalCheckboxState(const bool launchInTerm);

protected:
    /**
     * @brief  Updates the icon preview component.
     */
    void loadIconPreview();

    /**
     * @brief  Saves editor field values back to the application menu.
     */
    virtual void commitEdits() = 0;
private:

    /**
     * @brief  Triggers whenever the icon field is set to a new value.
     * 
     * @param iconEditor  The icon name editor.
     */
    virtual void fileSelected(FileSelectTextEditor* iconEditor) override;

    /**
     * @brief  Triggers whenever the category editor button is clicked.
     * 
     * @param button  The category editor button.
     */
    virtual void editorButtonClicked(juce::Button* button) override;

    /* Used to cancel pending icon requests on destruction. */
    Icon::RequestID iconRequestID;

    /* Label text: "Name:" */
    ScalingLabel nameLabel;     
    /* Edits the application/folder display name. */
    juce::TextEditor titleEditor;

    /* Label text: "Icon path:" */
    ScalingLabel iconLabel; 
    /* Draws a preview of the selected icon. */
    DrawableImageComponent iconPreview;
    /* Edits the application/folder icon */
    FileSelectTextEditor iconNameEditor;

    /* Launches a category editor pop-up */
    juce::TextButton categoryEditButton; 
    /* Holds the category editor when it's launched. */
    juce::ScopedPointer<CategoryEditor> categoryEditor;
    /* List of editable categories associated with this application/folder: */
    juce::StringArray categories;

    /* Label text: "Command:" */
    ScalingLabel commandLabel;
    /* Edits the application launch command */
    juce::TextEditor commandEditor;

    /* Label text: "Run in terminal:" */
    ScalingLabel terminalCheckboxLabel;
    /* Sets if this application launches as a terminal application: */
    SwitchComponent terminalCheckbox;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PopupEditor)
};
