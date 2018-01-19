/*
  ==============================================================================

    PopupEditorComponent.cpp
    Created: 12 Jan 2018 9:53:18pm
    Author:  anthony

  ==============================================================================
 */
#include "../../Utils.h"
#include "../../PocketHomeApplication.h"
#include "PopupEditorComponent.h"

PopupEditorComponent::PopupEditorComponent(String title) :
ConfigurableImageComponent(ComponentConfigFile::popupMenuKey,
0,RectanglePlacement::stretchToFit),
titleLabel("EditorTitle", title, 2),
cancelBtn("cancel.svg"),
confirmBtn("confirm.svg")
{
    setName("popupEditor");
    loadAllConfigProperties();

    titleLabel.setColour(Label::textColourId, textColour);
    titleLabel.setJustificationType(Justification::centred);
    layoutManager.addComponent(&titleLabel, 0, 1);
    addAndMakeVisible(titleLabel);
    
    setInterceptsMouseClicks(true,true);
    MessageManager::callAsync([this]
    {
        this->grabKeyboardFocus();
    });
}

PopupEditorComponent::~PopupEditorComponent()
{
}

void PopupEditorComponent::closePopup()
{
    setVisible(false);
    getParentComponent()->removeChildComponent(this);
}

/**
 * Add the cancel and confirm buttons to the bottom of the layout manager.
 * This only needs to be called once, after adding all subclass-specific
 * menu components.
 */
void PopupEditorComponent::addClosingButtons()
{
    int btnRow = layoutManager.getNumRows();
    layoutManager.addComponent(&cancelBtn, btnRow, 1);
    layoutManager.addComponent(&confirmBtn, btnRow, 1);

    cancelBtn.addListener(this);
    confirmBtn.addListener(this);
    cancelBtn.replaceColour(Colours::black,textColour);
    confirmBtn.replaceColour(Colours::black,textColour);
    addAndMakeVisible(cancelBtn);
    addAndMakeVisible(confirmBtn);
}

void PopupEditorComponent::buttonClicked(Button* buttonClicked)
{
    if (buttonClicked == &cancelBtn)
    {
        cancel();

    } else if (buttonClicked == &confirmBtn)
    {
        confirm();
    }
}

void PopupEditorComponent::resized()
{
    this->ConfigurableImageComponent::resized();
    layoutManager.layoutComponents(getLocalBounds(), 3, 3);
}


bool PopupEditorComponent::keyPressed(const KeyPress & key)
{
    if (key == KeyPress::escapeKey)
    {
        cancel();
        return true;
    } else if (key == KeyPress::returnKey)
    {
        confirm();
        return true;
    }
    return false;
}

void PopupEditorComponent::cancel()
{
    DBG("Canceled!");
    closePopup();
}

/**
 * run the parent class version to set image colors, then save
 * text color
 */
void PopupEditorComponent::applyConfigAssets(Array<String> assetNames,
        Array<Colour> colours)
{
    this->ConfigurableImageComponent::applyConfigAssets(assetNames,colours);
    while(colours.size()<3){
        colours.add(Colours::transparentBlack);
    }
    selectionColour=colours[0];
    bgColour=colours[1];
    textColour=colours[2];
}