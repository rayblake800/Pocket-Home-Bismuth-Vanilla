/*
  ==============================================================================

    PopupEditorComponent.h
    Created: 12 Jan 2018 9:53:18pm
    Author:  anthony

  ==============================================================================
 */
#pragma once
#include "../../GridLayoutManager.h"
#include "../../Configuration/Configurables/ConfigurableImageComponent.h"
#include "../../Basic Components/DrawableImageButton.h"
#include "../../Basic Components/ListEditor.h"
#include "../../Basic Components/ScalingLabel.h"

class PopupEditorComponent : public ConfigurableImageComponent,
public Button::Listener{
public:
    PopupEditorComponent(String title);
    virtual ~PopupEditorComponent();

    void closePopup();
protected:
    virtual void confirm()=0;
    
    protected:
    /**
     * run the parent class version to set image colors, then save
     * text color
     */
    virtual void applyConfigAssets(Array<String> assetNames,
            Array<Colour> colours);
    
    /**
     * Add the cancel and confirm buttons to the bottom of the layout manager.
     * This only needs to be called once, after adding all subclass-specific
     * menu components.
     */
    void addClosingButtons();
    void buttonClicked(Button* buttonClicked) override;
    GridLayoutManager layoutManager;
    
    Colour selectionColour;
    Colour bgColour;
    Colour textColour;
private:
    void resized() override;
    bool keyPressed(const KeyPress &) override;

    virtual void cancel();
  
    ScalingLabel titleLabel;
    DrawableImageButton cancelBtn;
    DrawableImageButton confirmBtn;

};