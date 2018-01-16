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
#include "../../Basic Components/ListEditor.h"

class PopupEditorComponent : public Component,
public Button::Listener,
public Configurable {
public:
    PopupEditorComponent(String title);
    virtual ~PopupEditorComponent();

    void closePopup();
protected:
    virtual void confirm(){};// =0;
    virtual void loadConfigProperties(ConfigFile * config, String key);
    
    /**
     * Add the cancel and confirm buttons to the bottom of the layout manager.
     * This only needs to be called once, after adding all subclass-specific
     * menu components.
     */
    void addClosingButtons();
    
    GridLayoutManager layoutManager;
private:
    void buttonClicked(Button* buttonClicked) override;
    void resized() override;
    void paint(Graphics &) override;
    bool keyPressed(const KeyPress &) override;

    virtual void cancel();
  
    Label titleLabel;
    ListEditor listEditor;
    ImageButton cancelBtn;
    ImageButton confirmBtn;

    Colour bgColour;
};