#pragma once
/**
 * @file BluetoothSettingsPage.h
 * 
 * @brief  Shows visible bluetooth devices, providing controls for initializing
 *         or removing connections.  
 * 
 * As Bluetooth support is not yet implemented, this class is currently just an
 * empty FocusingListPage.
 */

#include "Bluetooth_Status.h"
#include "FocusingListPage.h"
#include "SwitchComponent.h"
#include "DrawableImageComponent.h"
#include "ScalingLabel.h"
#include "Spinner.h"

namespace Bluetooth { class SettingsPage; }

class Bluetooth::SettingsPage : public FocusingListPage 
{
public:
    SettingsPage() { }
    
    ~SettingsPage() { }

private:
    /**
     * Gets the total number of items that should be in the list.
     * 
     * @return   The number of visible Bluetooth devices.
     */
    virtual unsigned int getListSize() override { return 0;}

    /**
     * Loads or updates the Component layout for each Bluetooth device in the 
     * list.
     * 
     * @param layout  The layout of a single list item.  If empty, appropriate
     *                components should be added to the layout.  Otherwise, any 
     *                existing components in the layout should be updated to fit
     *                the provided list index.  
     *                
     * @param index   The index of a Bluetooth device in the list. 
     */
    virtual void updateListItemLayout(Layout::Group::RelativeLayout& layout,
            const unsigned int index) override { }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsPage)
};
