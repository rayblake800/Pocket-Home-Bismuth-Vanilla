#include "Utils.h"
#include "ConnectionSettingsComponent.h"

ConnectionSettingsComponent::ConnectionSettingsComponent(
        std::function<void() > openConnectionPage,
        const juce::String& name) :
Component(name),
openConnectionPage(openConnectionPage),
pageButton(name + "Button")
{
    using namespace juce;
    toggle.addListener(this);
    pageButton.addListener(this);
    addAndMakeVisible(icon);
    addAndMakeVisible(toggle);
    addAndMakeVisible(pageButton);
    addChildComponent(spinner);
    Colour iconColour = findColour(Label::textColourId);
    icon.setColour(DrawableImageComponent::imageColour0Id, iconColour);
}

/**
 * Updates the icon, switch, and connection button based on current
 * connection status.
 */
void ConnectionSettingsComponent::refresh()
{
    using namespace juce;
    //DBG("ConnectionSettingsComponent::refresh()");
    bool busy = shouldShowSpinner();
    bool enabled = connectionEnabled();
    
    //DBG("ConnectionSettingsComponent::toggle should be "
    //        << (enabled ? "on" : "off"));
    icon.setVisible(!busy);
    spinner.setVisible(busy);
    if (!busy)
    {
        icon.setImage(getIconAsset());
        toggle.setToggleState(enabled, NotificationType::dontSendNotification,
                true);
    }
    toggle.setEnabled(allowConnectionToggle());
    pageButton.setText(updateButtonText());
    pageButton.setEnabled(connectionPageAvailable());
    resized();
    repaint();
}

/**
 * Arranges child components to fit within bounds.
 */
void ConnectionSettingsComponent::resized()
{
    int height = getHeight();
    int spacing = height / 10;
    icon.setBounds(0, 0, height, height);
    spinner.setBounds(0, 0, height, height);
    toggle.setBounds(
            icon.getRight() + spacing,
            0,
            height * 1.1f,
            height);
    pageButton.setBounds(
            toggle.getRight() + spacing,
            0,
            getWidth() - toggle.getRight() - spacing,
            height);
}

/**
 * Update the icon color if text color changes.
 */
void ConnectionSettingsComponent::colourChanged()
{
    using namespace juce;
    Colour iconColour = findColour(Label::textColourId);
    icon.setColour(DrawableImageComponent::imageColour0Id, iconColour);
}

/**
 * If the connection button is clicked this will run openConnectionPage(). 
 * If the switch is clicked, this will call  enabledStateChanged(), passing
 * it the switch toggle state.
 */
void ConnectionSettingsComponent::buttonClicked(juce::Button *b)
{
    if (b == &toggle)
    {
        enabledStateChanged(toggle.getToggleState());
        refresh();
    }
    else if (b == &pageButton)
    {
        openConnectionPage();
    }
}

/**
 * Run refresh() when the component regains visibility.
 */
void ConnectionSettingsComponent::visibilityChanged()
{
    if (isShowing())
    {
        refresh();
    }
}

/**
 * @param name internal component name
 */
ConnectionSettingsComponent::ConnectionButton::ConnectionButton
(const juce::String& name) : juce::Button(name){ }

/**
 * Sets the text that will be printed on the button.
 */
void ConnectionSettingsComponent::ConnectionButton::setText
(const juce::String &text)
{
    displayText = text;
    resized();
}

/**
 * Draws the connection button outline and prints the button text
 */
void ConnectionSettingsComponent::ConnectionButton::paintButton
(juce::Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
    using namespace juce;
    const Rectangle<int>& bounds = getLocalBounds();

    g.setColour(findColour(TextButton::textColourOnId));
    isButtonDown ? setAlpha(0.5f) : setAlpha(1.0f);

    if (isEnabled())
    {
        g.drawRoundedRectangle(bounds.getX() + borderSize,
                bounds.getY() + borderSize,
                bounds.getWidth() - 2 * borderSize,
                bounds.getHeight() - 2 * borderSize,
                1, borderSize);
    }

    g.setFont(textHeight);
    g.setColour(findColour(Label::textColourId));
    g.drawText(displayText, bounds.getX(), bounds.getY(),
            bounds.getWidth(), bounds.getHeight(),
            Justification::centred);
}

/**
 * Calculates button text height based on button size.
 */
void ConnectionSettingsComponent::ConnectionButton::resized()
{
    borderSize = getHeight() / 10;
    ComponentConfigFile config;
    textHeight = config.getFontHeight(getLocalBounds().reduced(borderSize * 2),
            displayText);
}
