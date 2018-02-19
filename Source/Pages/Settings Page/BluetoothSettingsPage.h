/**
 * @file BluetoothSettingsPage.h
 * 
 * TODO: documentation, cleanup
 */
#pragma once

#include "../../Basic Components/SwitchComponent.h"
#include "../../Basic Components/DrawableImageComponent.h"
#include "../../Basic Components/ScalingLabel.h"
#include "../../BluetoothStatus.h"
#include "../../PageStackComponent.h"
#include "../../PageComponent.h"
#include "../../Basic Components/Grid.h"

struct BTIcons {
    ScopedPointer<DrawableImageComponent> checkIcon;
    ScopedPointer<DrawableImageComponent> arrowIcon;
};

class BluetoothSettingsPage : public PageComponent {
public:
    BluetoothSettingsPage();
    ~BluetoothSettingsPage();


private:
    void pageResized() override;
    void pageButtonClicked(Button *) override;

    bool bluetoothEnabled = true;
    int currentDeviceIndex;

    BluetoothStatus::BluetoothDevice *selectedDevice;

    PageStackComponent pageStack;
    DrawableImageComponent checkIcon;
    DrawableImageComponent btIcon;
    Grid deviceListPage;
    ScalingLabel connectionLabel;
    TextButton connectionButton;

    class BluetoothDeviceListItem : public Button {
    public:
        BluetoothStatus::BluetoothDevice *device;
        BTIcons *icons;

        BluetoothDeviceListItem(BluetoothStatus::BluetoothDevice *device);
        
        void paintButton(Graphics &g, bool isMouseOverButton,
                bool isButtonDown) override;
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BluetoothDeviceListItem)
    };
    OwnedArray<BluetoothDeviceListItem> deviceListItems;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BluetoothSettingsPage)
};
