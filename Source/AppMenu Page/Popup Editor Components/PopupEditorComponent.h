/*
  ==============================================================================

    PopupEditorComponent.h
    Created: 12 Jan 2018 9:53:18pm
    Author:  anthony

  ==============================================================================
 */
#pragma once
#include "../../GridLayoutManager.h"
#include "../IconThread.h"
#include "../../Configuration/Configurables/ConfigurableImageComponent.h"
#include "../../Basic Components/DrawableImageButton.h"
#include "../../Basic Components/ListEditor.h"
#include "../../Basic Components/ScalingLabel.h"
#include "../../Basic Components/FileSelectTextEditor.h"

class IconThread;

class PopupEditorComponent : public ConfigurableImageComponent,
public Button::Listener,
public FileSelectTextEditor::Listener {
public:
    /**
     * @param title Display title to print at the top of the popup editor
     * window
     * @param iconThread used for loading preview icons
     * @param showCategoryList if true, the editor will contain a category 
     * list.
     * @param showCommandField if true, the editor will contain a terminal
     * command field and an "open in terminal" checkbox.
     * @param showDeleteButton if true, the editor will contain a delete
     *  button.
     */
    PopupEditorComponent(String title, IconThread& iconThread,
            bool showCategoryList,
            bool showCommandField, bool showDeleteButton);

    virtual ~PopupEditorComponent();

    /**
     * Removes this component from the window.
     */
    void closePopup();
protected:
    /**
     * run the parent class version to set image colors, then save
     * text color
     */
    virtual void applyConfigAssets(Array<String> assetNames,
            Array<Colour> colours);

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
    void setName(String name);

    /**
     * Set the value stored in the editable icon field, and update
     * the preview icon.
     * @param icon
     */
    void setIcon(String icon);


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

    void resized() override;

private:
    /**
     * This determines what happens if the delete button is pressed.
     */
    virtual void deleteBtnPressed()= 0;
    
    /**
     * Apply all values in the editor to the edited item.
     */
    virtual void confirm() = 0;
    
    /**
     * Close the popup editor without saving changes.
     */
    virtual void cancel();

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


    IconThread& iconThread;

    Colour selectionColour;
    Colour bgColour;
    Colour textColour;

    GridLayoutManager layoutManager;
    ScalingLabel titleLabel;

    ScalingLabel nameLabel;
    TextEditor nameEditor;

    ScalingLabel iconLabel;
    DrawableImageComponent iconPreview;
    FileSelectTextEditor iconPathEditor;

    ScalingLabel categoryLabel;
    ListEditor categoryList;

    ScalingLabel commandLabel;
    TextEditor commandEditor;
    ScalingLabel terminalCheckboxLabel;
    ToggleButton terminalCheckbox;

    TextButton deleteButton;

    DrawableImageButton cancelBtn;
    DrawableImageButton confirmBtn;
};