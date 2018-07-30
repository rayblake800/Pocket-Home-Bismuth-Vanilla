#pragma once
#include "ScalingLabel.h"
#include "DrawableImageButton.h"
#include "LayoutManager.h"
#include "ConfigurableImageComponent.h"
#include "JuceHeader.h"

/**
 * @file PopupEditorComponent.h
 * 
 * @brief A generic data editing overlay component.
 * 
 * PopupEditorComponent shows a small pop-up panel containing a title,
 * some type of content editing components, a cancel button, and a confirm 
 * button. Both buttons will remove the component, and the confirm button will
 * also run a callback function to save all changes.  Inheriting classes are 
 * responsible for adding all other controls to the component layout.
 */

class PopupEditorComponent : public ConfigurableImageComponent,
public juce::Button::Listener
{
public:
    virtual ~PopupEditorComponent() { }

    /**
     * Removes this component from the window.
     */
    void closePopup();

protected:
    /**
     * @param title      Text to print across the top of the component.
     * 
     * @param onConfirm  The callback function to run when the confirm
     *                    button is pressed.  When called, this editor passes 
     *                    in a reference to itself as the only argument.
     */
    PopupEditorComponent(juce::String title,
            std::function<void(PopupEditorComponent*) > onConfirm);

    /**
     * Add, make visible, and set the layout of components below the title
     * label and above the cancel and confirm buttons.
     * 
     * @param layout Defines the component layout. The title row will be added
     *                to the beginning, and the cancel/confirm row will be added
     *                to the end.  Each of these rows will have a vertical \
     *                weight of 1.
     */
    void setLayout(LayoutManager::Layout layout);

    /**
     * Button click events for any buttons other than the cancel and confirm
     * buttons will be passed to this method.  Inheriting classes should
     * override this instead of buttonClicked()
     * 
     * @param button
     */
    virtual void editorButtonClicked(juce::Button * button) { }

    /**
     * Set the space in pixels to leave between child components and outer 
     * component bounds
     * 
     * @param margin
     */
    void setMargin(int margin)
    {
        marginPixels = margin;
    }

    /**
     * Set the space in pixels to leave between child components.
     * 
     * @param xPadding
     * 
     * @param yPadding
     */
    void setPadding(int xPadding, int yPadding)
    {
        xPaddingPixels = xPadding;
        yPaddingPixels = yPadding;
    }

private:
    /**
     * Manages the cancel and confirm buttons, passing all other button events
     * to editorButtonClicked().
     * 
     * @param buttonClicked
     */
    void buttonClicked(juce::Button* buttonClicked) final override;

    /**
     * The escape and return keys work the same as pressing the cancel and 
     * confirm buttons, respectively.  When this component is open, it won't let
     * any button presses through to the components beneath it. 
     
     * @param  key  Any keyPress event not handled by the editor's child 
     *               components will be passed in here.
     * 
     * @return      true, to keep the Juce library key handling system from 
     *               passing the key event to another component. 
     */
    virtual bool keyPressed(const juce::KeyPress & key) final override;

    /**
     * Re-apply the layout to fit the new bounds.
     */
    virtual void resized() final override;

    /**
     * Grab keyboard focus when the component becomes visible.
     */
    void visibilityChanged() override;

    //Displays the editor title.
    ScalingLabel titleLabel;

    //Closes the editor without saving changes.
    DrawableImageButton cancelButton;

    //Closes the editor, saving all changes.
    DrawableImageButton confirmButton;

    //Manages editor child component layout.
    LayoutManager layoutManager;

    //Default layout margin/padding values.
    int marginPixels = 6;
    int xPaddingPixels = 3;
    int yPaddingPixels = 4;

    //Callback function to run when confirm is pressed, passes "this" as 
    //the sole parameter.
    std::function<void(PopupEditorComponent*) > onConfirm;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PopupEditorComponent)
};
