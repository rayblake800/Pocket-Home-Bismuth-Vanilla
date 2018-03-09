#include "PopupEditorComponent.h"

PopupEditorComponent::PopupEditorComponent(String title,
        std::function<void(PopupEditorComponent*) > onConfirm) :
ConfigurableImageComponent(ComponentConfigFile::popupMenuKey,
0, RectanglePlacement::stretchToFit),
onConfirm(onConfirm),
titleLabel("EditorTitle", title, 2),
cancelButton("cancel.svg"),
confirmButton("confirm.svg")
{
    setWantsKeyboardFocus(true);
#if JUCE_DEBUG
    setName(title + String("popupEditor"));
#endif
    loadAllConfigProperties();

    titleLabel.setJustificationType(Justification::centred);

    cancelButton.setWantsKeyboardFocus(false);
    cancelButton.addListener(this);

    confirmButton.setWantsKeyboardFocus(false);
    confirmButton.addListener(this);
    
    setInterceptsMouseClicks(true, true);
    MessageManager::callAsync([this]
    {
        this->grabKeyboardFocus();
    });
}

PopupEditorComponent::~PopupEditorComponent() { }

/**
 * Removes this component from the window.
 */
void PopupEditorComponent::closePopup()
{
    setVisible(false);
    getParentComponent()->removeChildComponent(this);
}

/**
 * Manages the cancel and confirm buttons. Inherited classes should either
 * call this method if they're handling button clicks, or handle clicks
 * on the cancel/confirm buttons themselves.
 */
void PopupEditorComponent::buttonClicked(Button* buttonClicked)
{
    if (buttonClicked == &cancelButton)
    {
        closePopup();
    }
    if (buttonClicked == &confirmButton)
    {
        onConfirm(this);
        closePopup();
    }
}


/**
 * The escape and return keys work the same
 * as pressing the cancel and confirm buttons, respectively.
 */
bool PopupEditorComponent::keyPressed(const KeyPress & key)
{
    DBG("PopupEditorComponent:key clicked");
    if (key == KeyPress::escapeKey)
    {
        closePopup();
    } else if (key == KeyPress::returnKey)
    {
        onConfirm(this);
        closePopup();
    }
    return true;
}

/**
 * Re-apply the layout to fit the new bounds.
 */
void PopupEditorComponent::resized()
{
    this->ConfigurableImageComponent::resized();
    layoutManager.layoutComponents(getLocalBounds().reduced(marginPixels),
            xPaddingPixels, yPaddingPixels);
}
