/**
 * @file PopupEditorComponent.h
 * @author Anthony Brown
 * 
 * PopupEditorComponent shows a small pop-up window containing a title,
 * some type of content editing components, a cancel button, and a confirm 
 * button. Inheriting classes are responsible for adding all other controls
 * and registering them with the layout manager.
 */

#pragma once
#include "../../Basic Components/ScalingLabel.h"
#include "../../Basic Components/DrawableImageButton.h"
#include "../../GridLayoutManager.h"
#include "../../Configuration/Configurables/ConfigurableImageComponent.h"
#include "../../../JuceLibraryCode/JuceHeader.h"

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
     * Manages the cancel and confirm buttons. Inherited classes should either
     * call this method if they're handling button clicks, or handle clicks
     * on the cancel/confirm buttons themselves.
     * @param buttonClicked is the button that was clicked by the user.
     */
    virtual void buttonClicked(Button* buttonClicked) override;
    
    /**
     * When this component is open, it should not let any button presses through
     * to the components beneath it. The escape and return keys work the same
     * as pressing the cancel and confirm buttons, respectively.
     * @param key is the key pressed by the user.
     * @return true always to indicate to the key manager that key presses
     * should not be passed on to the components beneath it.
     */
    virtual bool keyPressed(const KeyPress & key) override;
    
    /**
     * Calls layoutManager to resize all child components.
     */
    virtual void resized() override;
    ScalingLabel titleLabel;
    DrawableImageButton cancelButton;
    DrawableImageButton confirmButton;

    Colour selectionColour;
    Colour bgColour;
    Colour textColour;
    
    GridLayoutManager layoutManager;
    int marginPixels=2;
    int xPaddingPixels=3;
    int yPaddingPixels=4;
    
private:
    void applyConfigAssets(Array<String> assetNames,
            Array<Colour> colours) override;
    std::function<void(PopupEditorComponent*) > onConfirm;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PopupEditorComponent)
};
