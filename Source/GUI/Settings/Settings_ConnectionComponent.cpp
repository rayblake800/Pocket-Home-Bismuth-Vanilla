#include "Settings_ConnectionComponent.h"
#include "Layout_Component_ConfigFile.h"

// Component layout constants:

// Padding space between child components, as a fraction of component height:
static const constexpr float childPaddingFraction = 0.1;

// Connection button border size, as a fraction of the button's height:
static const constexpr float borderFraction = 0.08;
// Connection button text padding, as a fraction of the button's height:
static const constexpr float textPaddingFraction = 0.1;

// Toggle button width, as a multiple of the toggle's height:
static const constexpr float toggleWidthMultiplier = 1.1;

// Button alpha used when the connection button is being clicked:
static const constexpr float buttonDownAlpha = 0.5;
// Button alpha used when the connection button is not being clicked:
static const constexpr float buttonUpAlpha = 1.0;

Settings::ConnectionComponent::ConnectionComponent
(std::function<void()> openConnectionPage) :
openConnectionPage(openConnectionPage)
{
    toggle.addListener(this);
    pageButton.addListener(this);
    addAndMakeVisible(icon);
    addAndMakeVisible(toggle);
    addAndMakeVisible(pageButton);
    addChildComponent(spinner);
    juce::Colour iconColour = findColour(juce::Label::textColourId);
    icon.setColour(Widgets::DrawableImage::imageColour0Id, iconColour);
}


// Updates the icon, switch, and connection button based on the current
// connection status.
void Settings::ConnectionComponent::refresh()
{
    const bool busy = shouldShowSpinner();
    const bool enabled = connectionEnabled();
    icon.setVisible(!busy);
    spinner.setVisible(busy);
    if (!busy)
    {
        icon.setImage(getIconAsset());
        toggle.setToggleState(enabled,
                juce::NotificationType::dontSendNotification, true);
    }
    toggle.setEnabled(allowConnectionToggle());
    pageButton.setText(updateButtonText());
    pageButton.setEnabled(connectionPageAvailable());
    resized();
    repaint();
}


// Arranges child components to fit within the component bounds.
void Settings::ConnectionComponent::resized()
{
    int padding = getHeight() * childPaddingFraction;
    const int iconSize = getHeight() - 2 * padding;
    icon.setBounds(padding, padding, iconSize, iconSize);
    spinner.setBounds(padding, padding, iconSize, iconSize);
    toggle.setBounds(icon.getRight() + padding, padding,
            iconSize * toggleWidthMultiplier, iconSize);
    pageButton.setBounds(toggle.getRight() + padding, padding,
            getWidth() - toggle.getRight() - padding, iconSize);
}


// Updates the icon colour to match when the text colour changes.
void Settings::ConnectionComponent::colourChanged()
{
    juce::Colour iconColour = findColour(juce::Label::textColourId);
    icon.setColour(Widgets::DrawableImage::imageColour0Id, iconColour);
}


// Handles click events from the connection page button or the connection
// toggle switch.
void Settings::ConnectionComponent::buttonClicked(juce::Button* button)
{
    if (button == &toggle)
    {
        enabledStateChanged(toggle.getToggleState());
        refresh();
    }
    else if (button == &pageButton)
    {
        openConnectionPage();
    }
}


// Calls the refresh method when the component regains visibility.
void Settings::ConnectionComponent::visibilityChanged()
{
    if (isVisible())
    {
        refresh();
    }
}


// Sets the text that will be printed on the button.
void Settings::ConnectionComponent::ConnectionButton::setText
(const juce::String text)
{
    displayText = text;
    resized();
}


// Draws the connection button outline and prints the button text.
void Settings::ConnectionComponent::ConnectionButton::paintButton
(juce::Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
    g.setFont(textHeight);
    g.setColour(findColour(juce::Label::textColourId));
    const int textPadding = getHeight() * textPaddingFraction;
    g.drawText(displayText, textPadding, textPadding,
            getWidth() - 2 * textPadding, getHeight() - 2 * textPadding,
            juce::Justification::centred);

    g.setColour(findColour(juce::TextButton::textColourOnId));
    setAlpha( (isButtonDown ? buttonDownAlpha : buttonUpAlpha));

    if (isEnabled())
    {
        g.drawRoundedRectangle(borderSize, borderSize,
                std::max(0, getWidth() - 2 * borderSize),
                std::max(0, getHeight() - 2 * borderSize), 1, borderSize);
    }
}


// Calculates button text height based on button size.
void Settings::ConnectionComponent::ConnectionButton::resized()
{
    const int textPadding = getHeight() * textPaddingFraction;
    borderSize = getHeight() * borderFraction;
    Layout::Component::ConfigFile config;
    textHeight = config.getFontHeight(
            getLocalBounds().reduced(textPadding * 2), displayText);
}
