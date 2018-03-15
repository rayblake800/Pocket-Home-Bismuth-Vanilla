#include "Utils.h"
#include "ConnectionSettingsComponent.h"

ConnectionSettingsComponent::ConnectionSettingsComponent
(std::function<void() > openConnectionPage, const String& name) :
Component(name),
openConnectionPage(openConnectionPage),
pageButton(name + "Button")
{
    toggle.addListener(this);
    pageButton.addListener(this);
    addAndMakeVisible(icon);
    addAndMakeVisible(toggle);
    addAndMakeVisible(pageButton);
}

ConnectionSettingsComponent::~ConnectionSettingsComponent() { }

void ConnectionSettingsComponent::setIcon(String iconAsset)
{
    icon.setImage(iconAsset);
}

void ConnectionSettingsComponent::setIconVisible(bool visible)
{
    icon.setVisible(visible);
}

void ConnectionSettingsComponent::setToggleState(bool toggleState)
{
    if (toggle.getToggleState() != toggleState)
    {
        DBG("ConnectionSettingsComponent::" << __func__ << " turning "
                << (toggleState ? "on" : "off"));
        toggle.setToggleState(toggleState, NotificationType::sendNotification);
    }
}

void ConnectionSettingsComponent::setPageButtonEnabled(bool isEnabled)
{
    pageButton.setEnabled(isEnabled);
}

void ConnectionSettingsComponent::setPageButtonText(String text)
{
    pageButton.setText(text);
}

void ConnectionSettingsComponent::resized()
{
    int height = getHeight();
    int spacing = height/10;
    icon.setBounds(0, 0, height, height);
    toggle.setBounds(
            icon.getRight() + spacing,
            0,
            height * 1.1f,
            height);
    pageButton.setBounds(
            toggle.getRight() + spacing,
            0,
            getWidth() - toggle.getRight(),
            height);
}

void ConnectionSettingsComponent::buttonClicked(Button *b)
{
    if (b == &toggle)
    {
        enabledStateChanged(toggle.getToggleState());
    }
    else if (b == &pageButton)
    {
        openConnectionPage();
    }
}

void ConnectionSettingsComponent::enablementChanged()
{
    updateButtonText();
}

ConnectionSettingsComponent::ConnectionButton::ConnectionButton
(const String &name) : Button(name),
displayText(name) { }

ConnectionSettingsComponent::ConnectionButton::~ConnectionButton() { }

void ConnectionSettingsComponent::ConnectionButton::paintButton
(Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
    const auto& bounds = getLocalBounds();
    float borderThick = 4.0f;

    g.setColour(Colours::white);
    isButtonDown ? setAlpha(0.5f) : setAlpha(1.0f);

    if (isEnabled())
    {
        g.drawRoundedRectangle(bounds.getX() + borderThick,
                bounds.getY() + borderThick,
                bounds.getWidth() - 2 * borderThick,
                bounds.getHeight() - 2 * borderThick,
                1, borderThick);
    }

    // TODO: write button text as grey if choice is completely unset?
    g.setFont(20);
    g.drawText(displayText, bounds.getX(), bounds.getY(),
            bounds.getWidth(), bounds.getHeight(),
            Justification::centred);
}

void ConnectionSettingsComponent::ConnectionButton::resized()
{
    pillBounds.setSize(getLocalBounds().getWidth(), 42);
    fitRectInRect(pillBounds, getLocalBounds(), Justification::centred, false);
}

void ConnectionSettingsComponent::ConnectionButton::setText(const String &text)
{
    displayText = text;
    repaint();
}