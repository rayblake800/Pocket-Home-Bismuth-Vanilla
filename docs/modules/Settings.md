# Settings Module Documentation
The Settings module provides UI components that allow the user to set system and application settings.

#### [Settings\::ClockModePicker](../../Source/GUI/Settings/Settings_ClockModePicker.h)
ClockModePicker allows the user to select the display mode of the HomePage clock.

#### [Settings\::BrightnessSlider](../../Source/GUI/Settings/Settings_BrightnessSlider.h)
BrightnessSlider allows the user to set the display brightness level.

#### [Settings\::VolumeSlider](../../Source/GUI/Settings/Settings_VolumeSlider.h)
VolumeSlider allows the user to set the system volume level.

#### [Settings\::CursorModePicker](../../Source/GUI/Settings/Settings_CursorModePicker.h)
CursorModePicker allows the user to show or hide the mouse cursor.

#### [Settings\::HomeBackgroundPicker](../../Source/GUI/Settings/Settings_HomeBackgroundPicker.h)
HomeBackgroundPicker allows the user to set the image shown behind the application's home page.

#### [Settings\::CalibrationButton](../../Source/GUI/Settings/Settings_CalibrationButton.h)
CalibrationButton launches a touchscreen calibration application when clicked.

#### [Settings\::ConnectionComponent](../../Source/GUI/Settings/Settings_ConnectionComponent.h)
ConnectionComponent is an abstract basis for connection status and control widgets.

#### [Settings\::WifiControl](../../Source/GUI/Settings/Settings_WifiControl.h)
WifiControl is a ConnectionComponent used to enable or disable Wifi, view the active Wifi connection, and open the Wifi settings page.

## WifiList Connection Settings
The WifiList submodule creates a Wifi connection list component that allows the user to set or change the active Wifi connection.

#### [Settings\::WifiList\::ListComponent](../../Source/GUI/Settings/WifiList/Settings_WifiList_ListComponent.h)
ListComponent gets the list of all visible Wifi access points, and displays them within a [Widgets\::FocusingPageList](../../Source/Foundation/Widgets/Widgets_FocusingPagedList.h) UI component, where selecting a Wifi access point brings up connection controls and information for that access point.

#### [Settings\::WifiList\::ListButton](../../Source/GUI/Settings/WifiList/Settings_WifiList_ListButton.h)
ListButton UI components represents an unselected list item in the Wifi connection list. It displays the name of its Wifi access point, the signal strength of the access point, and whether the access point is connected or secured.

#### [Settings\::WifiList\::ControlComponent](../../Source/GUI/Settings/WifiList/Settings_WifiList_ControlComponent.h)
The ControlComponent shows connection controls and information for the selected list item. It shows last connection time, and provides controls to create a connection, destroy a connection, or delete a saved connection for the selected access point.

