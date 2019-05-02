# System Command File
The [commands.json](../../assets/commands.json) system command file stores system commands used by the application that may vary between different systems. Because system commands usually shouldn't be changed, and because of the potential security risks involved, the command file is stored in the pocket-home assets directory instead of with other configuration files.

Changes to commands.json will be overwritten whenever Pocket-Home Bismuth is updated. To make changes to system commands that don't get undone by updates, create a file called overrideCommands.json within the same directory as the commands.json file. Commands defined in this file will override commands defined in the default command file.

## Script Directory Value
The commands.json file includes a path to the default application script directory stored under the key "POCKET_HOME_SCRIPTS". Within all command values in this file, the literal string "POCKET_HOME_SCRIPTS" will be replaced with the script directory defined by this value.

## System Command List
Command Name        | Parameters Passed | Return Value Used | Expected Output | Expected Action
------------------- | ----------------- | ----------------- | --------------- | ---
shutdown            |                   |                   |                 | The system shuts down.
restart             |                   |                   |                 | The system restarts.
sleep               |                   |                   |                 | The display turns off.
wake                |                   |                   |                 | The display turns on.
calibrate           |                   |                   | New XInput calibration settings. | A display calibration window opens.
set volume          | Volume percentage.|                   |                 | The system volume levels are updated.
set brightness      | Display brightness, from one to ten. |      |      | The system's display brightness is updated.
sleep check         |                   | 0 if the display is off, 1 if it is on.
command check       | The name of a system command. |       | The full path to the command, or the empty string if the command is invalid.
set password        | The user name and the new password hash to save, or just the user name if removing the password. | One of the result codes at the top of [passwordManager.sh](../../assets/scripts/passwordManager.sh) |      | If successful, the application password was changed or removed.
get volume          |                   |                   | Volume percentage, or an error message if the command failed.
get battery percent |                   |                   | Remaining battery percentage, or an error message if the command failed.
get brightness      |                   |                   | Display brightness between one and ten, or an error message if the command failed.
get local IP        |                   |                   | The system's IP on the local network.
get public IP       |                   |                   | The system's current global IP.
