# Password Pages
The Set Password page, Change Password page, and Remove Password page set, update, and remove the Pocket-Home application password. When a password is set, Pocket-Home will show a login screen on launch and after exiting sleep mode that requests the password.

![Polkit Authentication](../images/screenshots/passwordAuth.png?raw=true "Polkit authentication")

To securely save password changes, you will be asked to enter your admin password in an authentication window. Make sure you have a PolicyKit authentication application running in the background, or this window will not appear and all password changes will fail.

![Set Password](../images/screenshots/pages/setPassword.png?raw=true "Set password")
1. Closes the page and returns to the [settings list page](settingsList.md).
2. Enter the new application password here.
3. Confirm the new password.
4. If a new password is provided and both password fields match, clicking this button will attempt to save the password. 

![Change Password](../images/screenshots/pages/changePassword.png?raw=true "Change password")
1. Closes the page and returns to the [settings list page](settingsList.md).
2. Enter the current application password here.
3. Enter the new application password here.
4. Confirm the new password.
5. If the current password is correct, a new password is provided, and both password fields match, clicking this button will attempt to change the password. 

![Remove Password](../images/screenshots/pages/removePassword.png?raw=true "Remove password")
1. Closes the page and returns to the [settings list page](settingsList.md).
2. Enter the current application password here.
5. If the current password is correct, clicking this button will attempt to remove the password. 
