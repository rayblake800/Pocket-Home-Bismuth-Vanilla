#include "Wifi_ConnectionList_ListButton.h"
#include "Wifi_ConnectionList_ControlComponent.h"
#include "Theme_Image_JSONKeys.h"

/* List item border width in pixels: */
static const constexpr unsigned int borderWidth = 4;

//===================== List item Layout values ================================
/* Horizontal weights for components in each access point list item: */
static const constexpr unsigned int labelWeight      = 8;
static const constexpr unsigned int iconWeight       = 1;
/* Relative weights of the main component row and the control component row: */
static const constexpr unsigned int mainRowWeight    = 2;
static const constexpr unsigned int controlRowWeight = 14;
/* Margin and Padding fractions: */
static const constexpr float xMarginFraction = 0.03;
static const constexpr float yMarginFraction = 0.17;
static const constexpr float xPaddingFraction = 0.04;
static const constexpr float yPaddingFraction = 0.03;
static const constexpr float selectedYMarginFraction = 0.03;

/*
 * Sets up the initial button layout.
 */
Wifi::ConnectionList::ListButton::ListButton() : 
        juce::Button("Wifi::ConnectionList::ListButton"),
        lockIcon(Theme::Image::JSONKeys::lockIcon)
{
    using namespace Layout::Group;
    RelativeLayout initialLayout(
    {
        Row(mainRowWeight,
        {
            RowItem(&apNameLabel, labelWeight),
            RowItem(&lockIcon, iconWeight),
            RowItem(&signalIcon, iconWeight)
        })
    });
    initialLayout.setXPaddingFraction(xPaddingFraction);
    initialLayout.setYPaddingFraction(yPaddingFraction);
    initialLayout.setXMarginFraction(xMarginFraction);
    initialLayout.setYMarginFraction(yMarginFraction);
    layoutManager.setLayout(initialLayout, this);
    lockIcon.setColour(Widgets::DrawableImage::ColourIds::imageColour0Id,
            findColour(juce::Label::ColourIds::textColourId));
    apNameLabel.setInterceptsMouseClicks(false, false);
    lockIcon.setInterceptsMouseClicks(false, false);
    signalIcon.setInterceptsMouseClicks(false, false);
}

/*
 * Updates this list button to represent a specific access point.
 */
void Wifi::ConnectionList::ListButton::updateForAccessPoint
(const AccessPoint accessPoint)
{
    apNameLabel.setText(accessPoint.getSSID().toString(),
            juce::NotificationType::dontSendNotification);
    lockIcon.setVisible(accessPoint.getSecurityType()
            != LibNM::SecurityType::unsecured);
    signalIcon.setSignalStrengthImage(accessPoint.getSignalStrength());
}


/*
 * Adds a connection control component to the bottom of the ListButton.
 */
void Wifi::ConnectionList::ListButton::addControlComponent
(ControlComponent& controlComponent)
{
    using namespace Layout::Group;
    Component* oldParent = controlComponent.getParentComponent();
    if(oldParent != nullptr && oldParent != this)
    {
        static_cast<ListButton*>(oldParent)->removeControlComponent();
    }

    RelativeLayout layout = layoutManager.getLayout();
    if(layout.rowCount() == 1)
    {
        layout.addRow(Row(controlRowWeight,
        {
            RowItem(&controlComponent)
        }));
        addAndMakeVisible(controlComponent);
        layout.setYMarginFraction(selectedYMarginFraction);
        layoutManager.setLayout(layout);
        layoutManager.layoutComponents(getLocalBounds()); 
    }
}

/*
 * Removes the control component from the ListButton if it is present.
 */
void Wifi::ConnectionList::ListButton::removeControlComponent()
{
    using namespace Layout::Group;
    RelativeLayout layout = layoutManager.getLayout();
    while(layout.rowCount() > 1)
    {
        layout.removeRow(1, true);
    }
    layout.setYMarginFraction(yMarginFraction);
    layoutManager.setLayout(layout);
    layoutManager.layoutComponents(getLocalBounds()); 
}

/*
 * Updates child components to fit the updated button bounds.
 */
void Wifi::ConnectionList::ListButton::resized()
{
    layoutManager.layoutComponents(getLocalBounds()); 
}

/*
 * Draws an outline around the ListButton.
 */
void Wifi::ConnectionList::ListButton::paintButton
(juce::Graphics& graphics, bool isMouseOverButton, bool isButtonDown)
{
    /* TODO: define custom ColourIDs rather than using label text color. */
    graphics.setColour(findColour(juce::Label::ColourIds::textColourId));
    graphics.drawRoundedRectangle(0, 0, getWidth(), getHeight(), 1, borderWidth);
}
