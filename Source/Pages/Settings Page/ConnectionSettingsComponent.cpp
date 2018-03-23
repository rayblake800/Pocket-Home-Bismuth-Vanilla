#include "Utils.h"
#include "ConnectionSettingsComponent.h"

ConnectionSettingsComponent::ConnectionSettingsComponent(
        std::function<void() > openConnectionPage,
        ComponentConfigFile& config,
        const String& name) :
Component(name),
config(config),
openConnectionPage(openConnectionPage),
pageButton(config, name + "Button")
{
    toggle.addListener(this);
    pageButton.addListener(this);
    addAndMakeVisible(icon);
    addAndMakeVisible(toggle);
    addAndMakeVisible(pageButton);
    addChildComponent(spinner);
}

/**
 * Updates the icon, switch, and connection button based on current
 * connection status.
 */
void ConnectionSettingsComponent::refresh()
{
    bool busy = isBusy();
    bool enabled = connectionEnabled();
    icon.setVisible(!busy);
    spinner.setVisible(busy);
    if (!busy)
    {
        icon.setImage(getIconAsset());
        toggle.setToggleState(enabled, NotificationType::dontSendNotification,
                true);
    }
    toggle.setEnabled(!busy);
    pageButton.setText(updateButtonText());
    pageButton.setEnabled(enabled && !busy);
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
 * If the connection button is clicked this will run openConnectionPage(). 
 * If the switch is clicked, this will call  enabledStateChanged(), passing
 * it the switch toggle state.
 */
void ConnectionSettingsComponent::buttonClicked(Button *b)
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
ConnectionSettingsComponent::ConnectionButton::ConnectionButton(
        ComponentConfigFile& config,
        const String& name) : Button(name), config(config) {
 }

/**
 * Sets the text that will be printed on the button.
 */
void ConnectionSettingsComponent::ConnectionButton::setText
(const String &text)
{
    displayText = text;
    resized();
}

/**
 * Draws the connection button outline and prints the button text
 */
void ConnectionSettingsComponent::ConnectionButton::paintButton
(Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
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
    textHeight = config.getFontHeight(getLocalBounds().reduced(borderSize * 2),
            displayText);
}
