#include "Wifi_SignalIcon.h"
#include "Theme_Image_JSONKeys.h"

/*
 * Initializes the icon with the image assets defined in the image
 * configuration file.
 */
Wifi::SignalIcon::SignalIcon() : 
    Theme::Image::Component<>(Theme::Image::JSONKeys::wifiIcon) { }

/*
 * Updates the Wifi signal icon to represent a new signal strength.
 */
void Wifi::SignalIcon::setSignalStrengthImage
(const unsigned int signalStrength)
{
    const int minStrengthIdx = (int) APIcon::wifiStrength0;
    const int maxStrengthIdx = (int) APIcon::wifiStrength3;
    const int numStrengthImages = 1 + maxStrengthIdx - minStrengthIdx;
    const int strengthIndex = std::min<int>(maxStrengthIdx,
            minStrengthIdx + signalStrength / (100 / numStrengthImages));
    setImageAssetIndex(strengthIndex);
}

/*
 * Updates the Wifi signal icon to show that Wifi is disabled.
 */
void Wifi::SignalIcon::setWifiDisabledImage()
{
    setImageAssetIndex((int) APIcon::wifiOff);
}
