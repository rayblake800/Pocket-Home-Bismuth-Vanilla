/**
 * @file PopupEditorComponent.h
 * @author Anthony Brown
 * 
 * PopupEditorComponent shows a small pop-up panel containing a title,
 * some type of content editing components, a cancel button, and a confirm 
 * button. Inheriting classes are responsible for adding all other controls
 * and for registering all components with the layout manager.
 */

#pragma once
#include "ScalingLabel.h"
#include "DrawableImageButton.h"
#include "RelativeLayoutManager.h"
#include "Configuration/Configurables/ConfigurableImageComponent.h"
#include "JuceHeader.h"

class PopupEditorComponent : public ConfigurableImageComponent,
public Button::Listener {
public:
    virtual ~PopupEditorComponent();
    
    /**
     * Removes this component from the window.
     */
    void closePopup();

protected:
    /**
     * @param title will be printed across the top of this component.
     * @param onConfirm is a callback function to run when the confirm
     * button is pressed.  When called, this editor passes in a reference to
     * itself as the only argument.
     */
    PopupEditorComponent(String title,
            std::function<void(PopupEditorComponent*)> onConfirm);
    
    /**
     * Add, make visible, and set the layout of components below the title
     * label and above the cancel and confirm buttons.
     * @param layout defines component layout. The title row will be added
     * to the beginning and the cancel/confirm row will be added to the end.
     * Each of these rows will have a vertical weight of 1.
     */
    void setLayout(RelativeLayoutManager::Layout layout);
    
    /**
     * Button click events for any buttons other than the cancel and confirm
     * buttons will be passed to this method.
     * @param button
     */
    virtual void editorButtonClicked(Button * button) {}
    
    /**
     * Set the space in pixels to leave between child components and outer 
     * component bounds
     * @param margin
     */
    void setMargin(int margin)
    {
        marginPixels = margin;
    }
    
    /**
     * Set the space in pixels to leave between child components.
     * @param xPadding
     * @param yPadding
     */
    void setPadding(int xPadding, int yPadding)
    {
        xPaddingPixels = xPadding;
        yPaddingPixels = yPadding;
    }
    
private:
    /**
     * Manages the cancel and confirm buttons.
     * @param buttonClicked
     */
     void buttonClicked(Button* buttonClicked) final override;
    
    /**
     * The escape and return keys work the same
     * as pressing the cancel and confirm buttons, respectively.
     * When this component is open, it won't let any button presses through
     * to the components beneath it. 
     * @param key
     * @return
     */
    virtual bool keyPressed(const KeyPress & key) final override;
  
    /**
     * Re-apply the layout to fit the new bounds.
     */
    virtual void resized() final override;
    
    /**
     * Grab keyboard focus when the component becomes visible.
     */
    void visibilityChanged() override;

    
    ScalingLabel titleLabel;
    //Closes the editor without saving changes.
    DrawableImageButton cancelButton;
    //Closes the editor, saving all changes
    DrawableImageButton confirmButton;
    RelativeLayoutManager layoutManager;
    //default layout margin/padding values
    int marginPixels=6;
    int xPaddingPixels=3;
    int yPaddingPixels=4;
    
    //Callback function to run when confirm is pressed, passes "this" as 
    //the sole parameter.
    std::function<void(PopupEditorComponent*) > onConfirm;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PopupEditorComponent)
};
