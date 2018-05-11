#include "Utils.h"
#include "BluetoothSettingsPage.h"

BluetoothSettingsPage::BluetoothSettingsPage
(BluetoothStatus& bluetoothStatus) :
ConnectionPage<BluetoothDevice>(),
bluetoothStatus(bluetoothStatus) { }

/**
 * @return the list of all visible bluetooth devices
 */
Array<BluetoothDevice> BluetoothSettingsPage::loadConnectionPoints()
{
    return {};
}

/**
 * @param button
 * 
 * This is called whenever a button other than the navigation buttons
 * is clicked.
 */
void BluetoothSettingsPage::connectionButtonClicked(Button* button) { }

/**
 * Construct a button component to represent a bluetooth device.
 * This button will display the device name:
 * TODO: design connection button
 * 
 * @param device
 */
Button* BluetoothSettingsPage::getConnectionButton
(const BluetoothDevice& device)
{
    return new BTDeviceButton(device, device.connected);
}

/**
 * Get the layout for the Bluetooth device controls.
 * 
 * @param connection The control components will be updated to suit
 *                    this bluetooth device.
 */
RelativeLayoutManager::Layout
BluetoothSettingsPage::getConnectionControlsLayout
(const BluetoothDevice& device)
{
    return {};
}

/**
 * Update connection control components to match the current bluetooth
 * device connection state and the provided Bluetooth device.
 * 
 * @param BluetoothDevice
 */
void BluetoothSettingsPage::updateConnectionControls() { }

/**
 * Attempts to connect if return is pressed.
 * 
 * @param editor
 */
void BluetoothSettingsPage::textEditorReturnKeyPressed
(TextEditor& editor) { }

/**
 * Set the spinner's bounds within the connection button.
 */
void BluetoothSettingsPage::connectionPageResized() { }

/**
 * Reload the device list, re-select the selected connection, 
 * update and enable connection controls.
 */
void BluetoothSettingsPage::reloadPage() { }

BluetoothSettingsPage::BTDeviceButton::BTDeviceButton(
        BluetoothDevice connection,
        bool isConnected) :
Button(connection.name + String("Button")),
deviceLabel("deviceLabel", connection.name) { }

void BluetoothSettingsPage::BTDeviceButton::resized() { }
