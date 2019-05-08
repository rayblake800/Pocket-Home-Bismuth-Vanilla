#include "Settings_ConnectionComponent.h"
#include "Layout_Component_ConfigFile.h"
#include "Util_Math.h"
#include "Windows_Info.h"

// Component layout constants:

// Landscape mode layout values:
namespace Settings { namespace LandscapeMode
{
    // Padding space between child components, as a fraction of width:
    static const constexpr float childPaddingFraction = 0.1;

    // Connection button border size, as a fraction of the button's height:
    static const constexpr float borderFraction = 0.08;
    //Minimum border size in pixels:
    static const constexpr int minBorderSize = 2;

    // Connection button text padding, as a fraction of the button's height:
    static const constexpr float textPaddingFraction = 0.1;

    // Minimum icon width in pixels:
    static const constexpr int minIconWidth = 6;
    // Ideal icon width, as a multiple of the component height:
    static const constexpr float iconSizeMultiplier = 1.0;
    // Maximum icon width, as a fraction of the component width:
    static const constexpr float maxIconWidthFraction = 0.2;

    // Minimum toggle button width in pixels:
    static const constexpr int minToggleWidth = 6;
    // Ideal toggle button width, as a multiple of the component height:
    static const constexpr float toggleSizeMultiplier = 1.1;
    // Maximum toggle button width, as a fraction of the component width:
    static const constexpr float maxToggleWidthFraction = 0.2;
} }


// Portrait mode layout values:
namespace Settings { namespace PortraitMode
{
    // Padding space between child components, as a fraction of width:
    static const constexpr float childPaddingFraction = 0.06;

    // Connection button border size, as a fraction of the button's height:
    static const constexpr float borderFraction = 0.08;
    //Minimum border size in pixels:
    static const constexpr int minBorderSize = 2;

    // Connection button text padding, as a fraction of the button's height:
    static const constexpr float textPaddingFraction = 0.05;

    // Minimum icon width in pixels:
    static const constexpr int minIconWidth = 6;
    // Ideal icon width, as a multiple of the component height:
    static const constexpr float iconSizeMultiplier = 1.1;
    // Maximum icon width, as a fraction of the component width:
    static const constexpr float maxIconWidthFraction = 0.4;

    // Minimum toggle button width in pixels:
    static const constexpr int minToggleWidth = 6;
    // Ideal toggle button width, as a multiple of the component height:
    static const constexpr float toggleSizeMultiplier = 1.5;
    // Maximum toggle button width, as a fraction of the component width:
    static const constexpr float maxToggleWidthFraction = 0.6;
} }


// Button alpha used when the connection button is being clicked:
static const constexpr float buttonDownAlpha = 0.5;
// Button alpha used when the connection button is not being clicked
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
    int padding, height, iconMin, toggleMin;
    float iconMult, iconMaxMult, toggleMult, toggleMaxMult;
    const bool portraitMode = Windows::Info::inPortraitMode();
    if (portraitMode)
    {
        using namespace PortraitMode;
        padding       = getHeight() * childPaddingFraction;
        height        = (getHeight() - padding * 2) / 2;
        iconMin       = minIconWidth;
        iconMult      = iconSizeMultiplier;
        iconMaxMult   = maxIconWidthFraction;
        toggleMin     = minToggleWidth;
        toggleMult    = toggleSizeMultiplier;
        toggleMaxMult = maxToggleWidthFraction;
    }
    else // landscape mode
    {
        using namespace LandscapeMode;
        padding       = getHeight() * childPaddingFraction;
        height        = getHeight() - padding * 2;
        iconMin       = minIconWidth;
        iconMult      = iconSizeMultiplier;
        iconMaxMult   = maxIconWidthFraction;
        toggleMin     = minToggleWidth;
        toggleMult    = toggleSizeMultiplier;
        toggleMaxMult = maxToggleWidthFraction;
    }
    int iconWidth = Util::Math::median<int>(iconMin, height * iconMult,
            getWidth() * iconMaxMult);
    int toggleWidth = Util::Math::median<int>(toggleMin, height * toggleMult,
            getWidth() * toggleMaxMult);
    icon.setBounds(padding, padding, iconWidth, height);
    spinner.setBounds(padding, padding, iconWidth, height);

    // In portrait mode, place the page button below the icon and toggle switch.
    if (portraitMode)
    {
        toggle.setBounds(getWidth() - toggleWidth - padding, padding,
                toggleWidth, height);
        pageButton.setBounds(padding, height + padding * 2,
                getWidth() - padding * 2, height);
    }
    else
    {
        toggle.setBounds(icon.getRight() + padding, padding, toggleWidth,
                height);
        pageButton.setBounds(toggle.getRight() + padding, padding,
                getWidth() - toggle.getRight() - padding, height);
    }
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


// Calls the refresh function when the component regains visibility.
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
    const int textPadding = (getHeight() - textHeight - borderSize) / 2;
    g.drawText(displayText, textPadding, textPadding,
            std::max(0, getWidth() - 2 * textPadding),
            std::max(0, getHeight() - 2 * textPadding),
            juce::Justification::centred);

    g.setColour(findColour(juce::TextButton::textColourOnId));
    setAlpha((isButtonDown ? buttonDownAlpha : buttonUpAlpha));

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
    const bool portraitMode = Windows::Info::inPortraitMode();
    const float& textMultiplier = (portraitMode
            ? PortraitMode::textPaddingFraction
            : LandscapeMode::textPaddingFraction);
    const float& borderMultiplier = (portraitMode
            ? PortraitMode::borderFraction
            : LandscapeMode::borderFraction);
    const int& borderMin = (portraitMode
            ? PortraitMode::minBorderSize
            : LandscapeMode::minBorderSize);

    const int textPadding = getHeight() * textMultiplier;
    borderSize = std::max<int>(getHeight() * borderMultiplier, borderMin);
    Layout::Component::ConfigFile config;
    textHeight = config.getFontHeight(getLocalBounds().reduced(textPadding * 2),
            displayText);
}
