#pragma once
/**
 * @file  Settings_WifiList_ListButton.h
 *
 * @brief  Represents a single list item in the WifiList.
 */

#include "JuceHeader.h"
#include "Widgets_BoundedLabel.h"
#include "Info_SignalIcon.h"
#include "Theme_Image_Component.h"
#include "Wifi_AccessPoint.h"
#include "Layout_Group_Manager.h"

namespace Settings
{
    namespace WifiList 
    { 
        class ListButton; 
        class ControlComponent;
    } 
}
namespace Wifi { class AccessPoint; }

/**
 * @brief  A button representing a Wifi access point in the access point
 *         connection list.
 */
class Settings::WifiList::ListButton final : public juce::Button
{
public:
    /**
     * @brief  Sets up the initial button layout.
     */
    ListButton();

    virtual ~ListButton() { }

    /**
     * @brief  Updates this list button to represent a specific access point.
     *
     * @param accessPoint  The Wifi access point object this component will
     *                     represent in the connection list.
     */
    void updateForAccessPoint(const Wifi::AccessPoint accessPoint);

    /**
     * @brief  Adds a connection control component to the bottom of the 
     *         ListButton.
     *
     * @param controlComponent  The control component to add and show.
     */
    void addControlComponent(ControlComponent& controlComponent);

    /**
     * @brief  Removes the control component from the ListButton if it is
     *         present.
     */
    void removeControlComponent();
    
private:
    /**
     * @brief  Updates child components to fit the updated button bounds.
     */
    virtual void resized() final override;

    /**
     * @brief  Draws an outline around the ListButton.
     *
     * @param graphics           The graphics context used to draw the outline.
     *
     * @param isMouseOverButton  Whether the mouse is currently over this 
     *                           button.
     *
     * @param isButtonDown       Whether this button is currently being held
     *                           down.
     */
    virtual void paintButton(juce::Graphics& graphics, bool isMouseOverButton,
            bool isButtonDown) final override;

    /* Handles the relative layout of child components: */
    Layout::Group::Manager layoutManager;

    /* Prints the access point name: */
    Widgets::BoundedLabel apNameLabel;

    /* Shows an image representing signal strength: */
    Info::SignalIcon signalIcon;

    /* Shows a lock icon if the access point is secured: */
    Theme::Image::Component<> lockIcon;
};
