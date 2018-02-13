#include <openssl/sha.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "../PocketHomeApplication.h"
#include "../Utils.h"
#include "LoginSettingsPage.h"

LoginSettingsPage::LoginSettingsPage() :
root_label("RootLab", "Sudo password"), root_password("Root", 0x2022),
cur_label("CurLabel", "Current password"), cur_password("Current", 0x2022),
new_label("NewLabel", "New password"), new_password("New", 0x2022),
ret_label("ConfLabel", "Retype password"), ret_password("Confirmation", 0x2022),
bg_color(0xffd23c6d), title("Title", "Change your password"),
deletemode(false), has_file(false), has_password(false)
{
    //Title
    title.setFont(Font(25.f));
    //Back button
    backButton = createImageButton("Back", createImageFromFile(assetFile("backIcon.png")));
    backButton->addListener(this);
    backButton->setAlwaysOnTop(true);
    addAndMakeVisible(backButton);
    //Load the password if existing
    loadPassword();
    apply.setButtonText("Apply");
    apply.addListener(this);
    //Setting the location in the view
    setLocations();
    //Adding components to the view
    addAndMakeVisible(title);
    addAndMakeVisible(root_label);
    addAndMakeVisible(root_password);
    addAndMakeVisible(cur_label);
    addAndMakeVisible(cur_password);
    addAndMakeVisible(new_label);
    addAndMakeVisible(new_password);
    addAndMakeVisible(ret_label);
    addAndMakeVisible(ret_password);
    addAndMakeVisible(apply);
}

LoginSettingsPage::~LoginSettingsPage()
{
}

void LoginSettingsPage::paint(Graphics &g)
{
    g.fillAll(bg_color);
}

void LoginSettingsPage::setLocations()
{
    //Setting the position of the back button
    backButton->setBounds(0, 0, 60, 272);

    int x = 90;
    int edit_x = 190;
    int edit_width = 200;
    int y = 50;
    int offset = 45;

    title.setBounds(130, 0, 300, 50);

    root_label.setBounds(x, y, 100, 30);
    root_password.setBounds(edit_x, y, edit_width, 30);
    y += offset;

    cur_label.setBounds(x, y, 100, 30);
    cur_password.setBounds(edit_x, y, edit_width, 30);
    y += offset;

    new_label.setBounds(x, y, 100, 30);
    new_password.setBounds(edit_x, y, edit_width, 30);
    y += offset;

    ret_label.setBounds(x, y, 100, 30);
    ret_password.setBounds(edit_x, y, edit_width, 30);
    y += offset;

    apply.setBounds(x, y, 300, 30);
}

void LoginSettingsPage::switchToModify()
{
    if (new_password.isVisible()) return;
    new_label.setVisible(true);
    new_password.setVisible(true);
    ret_label.setVisible(true);
    ret_password.setVisible(true);
    title.setText("Change your password", dontSendNotification);
    deletemode = false;
}

void LoginSettingsPage::switchToRemove()
{
    if (!new_password.isVisible()) return;
    new_label.setVisible(false);
    new_password.setVisible(false);
    ret_label.setVisible(false);
    ret_password.setVisible(false);
    title.setText("Remove your password", dontSendNotification);
    deletemode = true;
}

void LoginSettingsPage::loadPassword()
{
    char* home_str = getenv("HOME");
    String home(home_str);
    File passwd(home + "/.pocket-home/.passwd/passwd");
    if (passwd.exists())
    {
        String content = passwd.loadFileAsString();
        content = content.removeCharacters("\n");
        if (content == String("none"))
        {
            has_file = true;
            has_password = false;
        } else
        {
            has_file = true;
            has_password = true;
            cur_hashed = content;
        }
    } else
    {
        File passwordFile(getHomePath() + String("/.pocket-home/.passwd"));
        if (!passwordFile.existsAsFile())
        {
            Result makeFile = passwordFile.create();
            if (makeFile.failed())
            {
                DBG("Failed to create password file!");
                return;
            }
            passwordFile.appendText("none\n");
        }
    }
}

bool LoginSettingsPage::isPasswordCorrect(const String& pass)
{
    return !has_password || (pass == cur_hashed);
}

String LoginSettingsPage::hashString(const String& string)
{
    const unsigned char* str = (unsigned char*) string.toStdString().c_str();
    unsigned char hash[21];
    int i;
    char hashed[41];
    SHA1(str, string.length(), hash);
    //The Sha1sum of the string is now stored in the hash array (as a byte array)
    //So let's convert the byte array into a C string (char*)
    for (i = 0; i < 20; i++)
        sprintf(hashed + (i * 2), "%02x", hash[i]);
    hashed[40] = 0;
    return String(hashed);
}

bool LoginSettingsPage::passwordIdentical()
{
    if (new_password.getText() != ret_password.getText())
    {
        AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::WarningIcon,
                "Wrong password",
                "Confirmation password is wrong, try again",
                "Ok");
        return false;
    }
    return true;
}

bool LoginSettingsPage::hasPassword()
{
    //We need to often read the file in case it's manually modified
    loadPassword();
    return has_password;
}

void LoginSettingsPage::deletePassword()
{
    String typed_root = root_password.getText();
    String typed_current = cur_password.getText();
    char* home_str = getenv("HOME");
    String home(home_str);
    String passwd_path = home + "/.pocket-home/.passwd/passwd";
    String user(getlogin());
    String root_pass = root_password.getText();
    String cmd_passwd = "echo \"" + root_pass + "\" | ";
    //Changing the owner of the password file (to be able to write inside)
    String command1 = cmd_passwd + "sudo -kS chown " + user + ":" + user + " " + passwd_path;
    int returned = system(command1.toRawUTF8());
    if (returned != 0)
    {
        //An error occured !
        AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::WarningIcon,
                "Wrong password",
                "Impossible to modify the password, check your root password",
                "Ok");
        return;
    }
    //Write inside the file
    int fd = open(passwd_path.toRawUTF8(), O_WRONLY | O_TRUNC);
    if (fd < 0)
        perror("File couldn't be opened");
    int wr = write(fd, "none\n", 5);
    if (wr != 5)
        perror("Couldn't write in the file");
    close(fd);
    //After writing, we put back the owner (root)
    String command2 = cmd_passwd + "sudo -kS chown root:root " + passwd_path;
    returned = system(command2.toRawUTF8());
    AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::InfoIcon,
            "Success",
            "Password removed !",
            "Ok");
    has_password = false;
}

void LoginSettingsPage::savePassword()
{
    if (!passwordIdentical()) return;
    loadPassword();
    String typed_root = root_password.getText();
    String typed_current = cur_password.getText();
    String typed_new = new_password.getText();

    if (has_password)
    {
        String hashed_typed = hashString(typed_current);
        if (hashed_typed != cur_hashed)
        {
            AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::WarningIcon,
                    "Wrong password",
                    "Your current password is wrong, try again",
                    "Ok");

            return;
        }
    }

    String hashed_new = hashString(typed_new);
    char* home_str = getenv("HOME");
    String home(home_str);
    String passwd_folder = home + "/.pocket-home/.passwd";
    String passwd_path = home + "/.pocket-home/.passwd/passwd";
    String user(getlogin());
    String root_pass = root_password.getText();
    String cmd_passwd = "echo \"" + root_pass + "\" |";
    //Changing the owner of the password file (to be able to write inside)
    String command1 = cmd_passwd + "sudo -kS chown " + user + ":" + user + " " + passwd_path;
    int returned = system(command1.toRawUTF8());
    if (returned != 0)
    {
        //An error occured !
        AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::WarningIcon,
                "Wrong password",
                "Impossible to modify the password, check your root password",
                "Ok");
        return;
    }
    //Write inside the file
    int fd = open(passwd_path.toRawUTF8(), O_WRONLY | O_TRUNC);
    if (fd < 0)
        perror("File couldn't be opened");
    int wr = write(fd, hashed_new.toRawUTF8(), hashed_new.length());
    if (wr != hashed_new.length())
        perror("Couldn't write in the file");
    close(fd);
    //After writing, we put back the owner (root)
    String command2 = cmd_passwd + "sudo -kS chown root:root " + passwd_path;
    String command3 = cmd_passwd + "sudo -kS chown root:root " + passwd_folder;
    returned = system(command2.toRawUTF8());
    returned = system(command3.toRawUTF8());
    AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::InfoIcon,
            "Success",
            "Password changed !",
            "Ok");
    cur_hashed = hashed_new;
    has_password = true;
    has_file = true;
}

void LoginSettingsPage::clearAllFields()
{
    root_password.clear();
    cur_password.clear();
    new_password.clear();
    ret_password.clear();
}

void LoginSettingsPage::buttonClicked(Button* button)
{
    if (button == backButton)
    {
        PocketHomeApplication::getInstance()->getMainStack().popPage
                (PageStackComponent::kTransitionTranslateHorizontal);
        clearAllFields();
    } else if (button == &apply)
    {
        if (!deletemode) savePassword();
        else deletePassword();
    }
}