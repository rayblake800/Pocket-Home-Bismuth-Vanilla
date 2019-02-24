#pragma once
/**
 * @file BluetoothSettingsPage.h
 * 
 * @brief  Shows visible bluetooth devices, providing controls for initializing
 *         or removing connections.  
 * 
 * As Bluetooth support is not yet implemented, this class is currently just an
 * empty Page::Component.
 */

#include "Bluetooth_Status.h"
#include "Page_Component.h"
#include "SwitchComponent.h"
#include "DrawableImageComponent.h"
#include "ScalingLabel.h"
#include "Spinner.h"

namespace Bluetooth { class SettingsPage; }

class Bluetooth::SettingsPage : public Page::Component 
{
public:
    SettingsPage() { }
    
    ~SettingsPage() { }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsPage)
};
