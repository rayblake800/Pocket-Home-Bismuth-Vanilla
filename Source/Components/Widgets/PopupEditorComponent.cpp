#include "PopupEditorComponent.h"

PopupEditorComponent::PopupEditorComponent(String title,
        ComponentConfigFile& config,
        std::function<void(PopupEditorComponent*) > onConfirm) :
ConfigurableImageComponent(ComponentConfigFile::popupMenuKey,
config, 0, RectanglePlacement::stretchToFit),
onConfirm(onConfirm),
titleLabel(config, "EditorTitle", title, 2),
cancelButton("cancel.svg"),
confirmButton("confirm.svg")
{
    setWantsKeyboardFocus(true);
#    if JUCE_DEBUG
    setName(title + String("popupEditor"));
#    endif
    loadAllConfigProperties();

    titleLabel.setJustificationType(Justification::centred);

    cancelButton.setWantsKeyboardFocus(false);
    cancelButton.addListener(this);

    confirmButton.setWantsKeyboardFocus(false);
    confirmButton.addListener(this);

    setInterceptsMouseClicks(true, true);
}

PopupEditorComponent::~PopupEditorComponent() { }

/**
 * Removes this component from the window.
 */
void PopupEditorComponent::closePopup()
{
    setVisible(false);
    Component* parent = getParentComponent();
    if (parent != nullptr)
    {
        parent->removeChildComponent(this);
    }
}

/**
 * Add, make visible, and set the layout of components below the title
 * label and above the cancel and confirm buttons.
 */
void PopupEditorComponent::setLayout(RelativeLayoutManager::Layout layout)
{
    layout.insert(layout.begin(),{1,
        {
            {&titleLabel, 1}
        }});

    layout.push_back({1,
        {
            {&cancelButton, 1},
            {&confirmButton, 1}
        }});
    layoutManager.setLayout(layout, this);
    if (!getBounds().isEmpty())
    {
        resized();
    }
}

/**
 * Manages the cancel and confirm buttons passing all other button events
 * to editorButtonClicked().
 */
void PopupEditorComponent::buttonClicked(Button* buttonClicked)
{
    if (buttonClicked == &cancelButton)
    {
        closePopup();
    }
    else if (buttonClicked == &confirmButton)
    {
        onConfirm(this);
        closePopup();
    }
    else
    {
        editorButtonClicked(buttonClicked);
    }
}

/**
 * The escape and return keys work the same as pressing the cancel and confirm 
 * buttons, respectively.
 */
bool PopupEditorComponent::keyPressed(const KeyPress & key)
{
    DBG("PopupEditorComponent::" << __func__ << ": "
            << key.getTextDescription());
    if (key == KeyPress::escapeKey)
    {
        closePopup();
    }
    else if (key == KeyPress::returnKey)
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

/**
 * Grab keyboard focus when the component becomes visible.
 */
void PopupEditorComponent::visibilityChanged()
{
    if (isShowing())
    {
        grabKeyboardFocus();
    }
}
