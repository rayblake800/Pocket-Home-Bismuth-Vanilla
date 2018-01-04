#pragma once

#include "../Basic Components/SwitchComponent.h"
#include "../BluetoothStatus.h"
#include "../PageStackComponent.h"
#include "../Basic Components/Grid.h"

struct BTIcons {
    ScopedPointer<Drawable> checkIcon;
    ScopedPointer<Drawable> arrowIcon;
};

class BluetoothDeviceListItem : public Button {
public:
    BluetoothStatus::BluetoothDevice *device;
    BTIcons *icons;

    BluetoothDeviceListItem(BluetoothStatus::BluetoothDevice *device,
            BTIcons *icons);

    void paintButton(Graphics &g, bool isMouseOverButton,
            bool isButtonDown) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BluetoothDeviceListItem)
};

class SettingsPageBluetoothComponent : public Component,
private Button::Listener {
public:
    SettingsPageBluetoothComponent();
    ~SettingsPageBluetoothComponent();

    bool init = false;

    bool bluetoothEnabled = true;
    int currentDeviceIndex;

    BluetoothStatus::BluetoothDevice *selectedDevice;

    ScopedPointer<PageStackComponent> pageStack;

    ScopedPointer<ImageButton> backButton;
    ScopedPointer<Drawable> checkIcon;
    ScopedPointer<ImageComponent> btIcon;

    ScopedPointer<Grid> deviceListPage;
    OwnedArray<Component> deviceListItems;

    ScopedPointer<Component> connectionPage;
    ScopedPointer<Label> connectionLabel;
    ScopedPointer<TextButton> connectionButton;

    BTIcons icons;

    void paint(Graphics &) override;
    void resized() override;

private:

    void buttonClicked(Button *) override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsPageBluetoothComponent)
};
