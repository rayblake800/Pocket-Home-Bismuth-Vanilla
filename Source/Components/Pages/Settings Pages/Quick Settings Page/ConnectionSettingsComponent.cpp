#include "Utils.h"
#include "ConnectionSettingsComponent.h"

/* Padding space between child components, as a fraction of component height. */
static const constexpr float childPaddingFraction = 0.1;
/* Button alpha used when the connection button is being clicked. */
static const constexpr float buttonDownAlpha = 0.5;
/* Button alpha used when the connection button is not being clicked. */
static const constexpr float buttonUpAlpha = 1.0;
/* Connection button border size, as a fraction of the button's height. */
static const constexpr float borderFraction = 0.1;

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

/*
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

/*
 * Arranges child components to fit within bounds.
 */
void ConnectionSettingsComponent::resized()
{
    int height = getHeight();
    int padding = height * childPaddingFraction;
    icon.setBounds(0, 0, height, height);
    spinner.setBounds(0, 0, height, height);
    toggle.setBounds(
            icon.getRight() + padding,
            0,
            height * 1.1f,
            height);
    pageButton.setBounds(
            toggle.getRight() + padding,
            0,
            getWidth() - toggle.getRight() - padding,
            height);
}

/*
 * If text color changes, updates the icon color to match.
 */
void ConnectionSettingsComponent::colourChanged()
{
    using namespace juce;
    Colour iconColour = findColour(Label::textColourId);
    icon.setColour(DrawableImageComponent::imageColour0Id, iconColour);
}

/*
 * Handles click events from the connection page button or the connection toggle
 * switch.
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

/*
 * Runs refresh() when the component regains visibility.
 */
void ConnectionSettingsComponent::visibilityChanged()
{
    if (isShowing())
    {
        refresh();
    }
}

ConnectionSettingsComponent::ConnectionButton::ConnectionButton
(const juce::String name) : juce::Button(name){ }

/*
 * Sets the text that will be printed on the button.
 */
void ConnectionSettingsComponent::ConnectionButton::setText
(const juce::String text)
{
    displayText = text;
    resized();
}

/*
 * Draws the connection button outline and prints the button text
 */
void ConnectionSettingsComponent::ConnectionButton::paintButton
(juce::Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
    using namespace juce;
    const Rectangle<int>& bounds = getLocalBounds();

    g.setColour(findColour(TextButton::textColourOnId));
    setAlpha((isButtonDown ? buttonDownAlpha : buttonUpAlpha));

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

/*
 * Calculates button text height based on button size.
 */
void ConnectionSettingsComponent::ConnectionButton::resized()
{
    borderSize = getHeight() * borderFraction;
    ComponentConfigFile config;
    textHeight = config.getFontHeight(getLocalBounds().reduced(borderSize * 2),
            displayText);
}
