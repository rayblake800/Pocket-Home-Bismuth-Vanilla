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
 * TODO: documentation, cleanup
 */

class BluetoothSettingsPage :
public ConnectionPage<BluetoothDevice::Ptr>, 
        public TextEditor::Listener  {
public:
    
    BluetoothSettingsPage(BluetoothStatus& bluetoothStatus);
    
    ~BluetoothSettingsPage() { }
private:
    /**
     * @return the list of all visible bluetooth devices
     */
    Array<BluetoothDevice::Ptr> loadConnectionList();


    /**
     * Attempts to connect to a Bluetooth device.
     *
     *  @param device
     */
    void connect(BluetoothDevice::Ptr device);

    /**
     * @param device  If the system is currently connected to this
     *                 bluetooth device, this method closes that connection.
     *  
     * @param connection
     */
    void disconnect(BluetoothDevice::Ptr device);

    /**
     * @param connection
     * 
     * @return true iff the system is connected to this bluetooth device.
     */
    bool isConnected(BluetoothDevice::Ptr device);

    /**
     * @param button
     * This is called whenever a button other than the navigation buttons
     * is clicked.
     */
    void connectionButtonClicked(Button* button);

    /**
     * Construct a button component to represent a bluetooth device.
     * This button will display the device name:
     * TODO: design connection button
     * 
     * @param device
     */
    Button* getConnectionButton(BluetoothDevice::Ptr device);

    /**
     * Get the layout for the Bluetooth device controls.
     * @param connection the control components will be updated to suit
     * this bluetooth device.
     */
    RelativeLayoutManager::Layout getConnectionControlsLayout
    (BluetoothDevice::Ptr device);

    /**
     * Update connection control components to match the current bluetooth
     * device connection state and the provided Bluetooth device.
     * 
     * @param BluetoothDevice
     */
    virtual void updateConnectionControls
    (BluetoothDevice::Ptr device) override;


    /**
     * When currentlyConnecting, disable bluetooth controls and show a loading
     * spinner.  Otherwise, enable controls and hide the loading spinner.
     * 
     * @param currentlyConnecting Indicates if bluetooth is trying to connect 
     *                             to a device, or is otherwise busy.
     */
    void setCurrentlyConnecting(bool currentlyConnecting);



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
        BTDeviceButton(BluetoothDevice::Ptr connection, 
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
