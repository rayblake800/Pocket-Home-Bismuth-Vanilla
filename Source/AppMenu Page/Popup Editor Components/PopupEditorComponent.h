/*
  ==============================================================================

    PopupEditorComponent.h
    Created: 12 Jan 2018 9:53:18pm
    Author:  anthony

  ==============================================================================
 */
#pragma once
#include "../../GridLayoutManager.h"
#include "../../Configuration/Configurable.h"

class PopupEditorComponent : public Component,
public Button::Listener,
public Configurable {
public:
    PopupEditorComponent(String title);
    virtual ~PopupEditorComponent();
    
    void closePopup();
private:
    void buttonClicked(Button* buttonClicked) override;
    void resized() override;
    void paint(Graphics &) override;
    bool keyPressed(const KeyPress &) override;
    
    virtual void confirm();
    virtual void cancel();
    
    virtual void loadConfigProperties(ConfigFile * config,String key);

    GridLayoutManager layoutManager;

    Label titleLabel;
    ImageButton cancelBtn;
    ImageButton confirmBtn;
    
    Colour bgColour;
};