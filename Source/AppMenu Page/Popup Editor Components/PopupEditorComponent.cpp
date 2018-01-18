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
Configurable(&PocketHomeApplication::getInstance()->getComponentConfig(),
{

    ComponentConfigFile::popupMenuKey
}),
titleLabel("EditorTitle",title,2),
cancelBtn("cancel"),
confirmBtn("confirm"),
listEditor({}, Colours::white, Colours::aquamarine, Colours::black)
{
    setName("popupEditor");
    layoutManager.addComponent(&titleLabel, 0, 1);

    titleLabel.setColour(Label::textColourId, Colours::black);
    titleLabel.setJustificationType(Justification::centred);

    addAndMakeVisible(titleLabel);
    loadAllConfigProperties();
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
    cancelBtn.setImages(true, true, true,
            createImageFromFile(assetFile("cancel.svg")),
            1.0f, Colours::transparentWhite, // normal
            Image::null, 1.0f, Colours::transparentWhite, // over
            Image::null, 0.5f, Colours::transparentWhite, // down
            0);

    confirmBtn.setImages(true, true, true,
            createImageFromFile(assetFile("confirm.svg")),
            1.0f, Colours::transparentWhite, // normal
            Image::null, 1.0f, Colours::transparentWhite, // over
            Image::null, 0.5f, Colours::transparentWhite, // down
            0);

    cancelBtn.addListener(this);
    confirmBtn.addListener(this);
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
    layoutManager.layoutComponents(getLocalBounds(), 3, 3);
}

void PopupEditorComponent::paint(Graphics & g)
{
    g.setColour(bgColour);
    g.fillRect(getLocalBounds());
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

void PopupEditorComponent::loadConfigProperties
(ConfigFile * config, String key)
{
    ComponentConfigFile::ComponentSettings settings =
            (static_cast<ComponentConfigFile*> (config))
            ->getComponentSettings(key);
    Array<Colour> colours = settings.getColours();
    if (colours.size() > 0)
    {
        bgColour = colours[0];
    }
    Rectangle<int> bounds = settings.getBounds();
    Rectangle<int> windowBounds = getWindowSize();
    bounds.setCentre(windowBounds.getCentre());
    setBounds(bounds);

}