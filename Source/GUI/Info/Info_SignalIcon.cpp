#include "Info_SignalIcon.h"
#include "Theme_Image_JSONKeys.h"

// Initializes the icon with the image assets defined in the image
// configuration file.
Info::SignalIcon::SignalIcon() :
    Theme::Image::Component<>(Theme::Image::JSONKeys::signalIcon) { }

/**
 * @brief  All signal state icon images.
 *
 * Each should correspond to an image asset file defined in components.json.
 */
enum class ImageAsset
{
    off,
    strength0,
    strength1,
    strength2,
    strength3
};

// Updates the Wifi signal icon to represent a new signal strength.
void Info::SignalIcon::setSignalStrengthImage
(const unsigned int signalStrength)
{
    const int minStrengthIdx = (int) ImageAsset::strength0;
    const int maxStrengthIdx = (int) ImageAsset::strength3;
    const int numStrengthImages = 1 + maxStrengthIdx - minStrengthIdx;
    const int strengthIndex = std::min<int>(maxStrengthIdx,
            minStrengthIdx + signalStrength / (100 / numStrengthImages));
    juce::MessageManager::getInstance()->callAsync([this, strengthIndex]()
    {
        setImageAssetIndex(strengthIndex);
    });
}


// Updates the Wifi signal icon to show that Wifi is disabled.
void Info::SignalIcon::setDisabledImage()
{
    setImageAssetIndex( (int) ImageAsset::off);
}
