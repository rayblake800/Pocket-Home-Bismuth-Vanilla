#pragma once

#include "SwitchComponent.h"
#include "DrawableImageComponent.h"
#include "ScalingLabel.h"
#include "Spinner.h"
#include "BluetoothStatus.h"
#include "ConnectionPage.h"

/**
 * @file BluetoothSettingsPage.h
 * 
 * TODO: implement bluetooth
 */

class BluetoothSettingsPage :
public ConnectionPage<BluetoothDevice>, 
        public TextEditor::Listener  {
public:
    
    BluetoothSettingsPage(BluetoothStatus& bluetoothStatus);
    
    ~BluetoothSettingsPage() { }
private:
    /**
     * @return the list of all visible bluetooth devices
     */
    Array<BluetoothDevice> loadConnectionPoints() override;
    
    /**
     * This method will be used to sort the list of bluetooth devices.
     * 
     * @param first    Some bluetooth device in the list.
     * 
     * @param second   Some other bluetooth device in the list.
     * 
     * @return  a negative number if first should come before second, zero if 
     *          the two bluetooth devices are equal, or a positive number if
     *          second should come before first.
     */
    int compareConnectionPoints
    (const BluetoothDevice& first, const BluetoothDevice& second) override
    {
        return 0;
    }

    /**
     * @param button
     * This is called whenever a button other than the navigation buttons
     * is clicked.
     */
    void connectionButtonClicked(Button* button) override;

    /**
     * Construct a button component to represent a bluetooth device.
     * This button will display the device name:
     * TODO: design connection button
     * 
     * @param device
     */
    Button* getConnectionButton(const BluetoothDevice& device) override;

    /**
     * Get the layout for the Bluetooth device controls.
     * @param connection the control components will be updated to suit
     * this bluetooth device.
     */
    RelativeLayoutManager::Layout getConnectionControlsLayout
    (const BluetoothDevice& device) override;

    /**
     * Update connection control components to match the current bluetooth
     * device connection state.
     */
    void updateConnectionControls() override;

    /**
     * Attempt to connect if return is pressed.
     * 
     * @param editor
     */
    void textEditorReturnKeyPressed(TextEditor& editor) override;

    /**
     * Set the spinner's bounds within the connection button.
     */
    void connectionPageResized() override;


    /**
     * Reload the device list, re-select the selected connection, 
     * update and enable connection controls.
     */
    void reloadPage();

    /**
     * Custom button type to use for getConnectionButton
     */
    class BTDeviceButton : public Button{
    public:
        BTDeviceButton(BluetoothDevice connection, 
                bool isConnected);
    private:
        void resized() override;

        void paintButton(Graphics& g, bool isMouseOverButton,
                bool isButtonDown) {
        }
        ScalingLabel deviceLabel;
        DrawableImageComponent bluetoothIcon;
        ScopedPointer<DrawableImageComponent> lockIcon;

    };
    
    BluetoothStatus& bluetoothStatus;
    TextButton connectionButton;
    ScalingLabel errorLabel;
    Spinner spinner;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BluetoothSettingsPage)
};
