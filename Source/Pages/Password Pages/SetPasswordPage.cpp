#include <openssl/sha.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "Utils.h"
#include "SetPasswordPage.h"

SetPasswordPage::SetPasswordPage() :
PageComponent("SetPasswordPage",{
    {2,
        {
            {&title, 1}}},
    {1,
        {
            {&root_label, 1},
            {&root_password, 3}}},
    {1,
        {
            {&cur_label, 1},
            {&cur_password, 3}}},
    {1,
        {
            {&new_label, 1},
            {&new_password, 3}}},
    {1,
        {
            {&ret_label, 1},
            {&ret_password, 3}}},
    {1,
        {
            {&apply, 1}}}
}, true),
root_label("RootLab", "Sudo password"), root_password("Root", 0x2022),
cur_label("CurLabel", "Current password"), cur_password("Current", 0x2022),
new_label("NewLabel", "New password"), new_password("New", 0x2022),
ret_label("ConfLabel", "Retype password"), ret_password("Confirmation", 0x2022),
title("Title", "Change your password"),
has_file(false), has_password(false)
{
    
#if JUCE_DEBUG
    setName("SetPasswordPage");
#endif
    title.setJustificationType(Justification::centred);
    //Load the password if existing
    loadPassword();
    apply.setButtonText("Apply");
    apply.addListener(this);
    addAndShowLayoutComponents();
}

SetPasswordPage::~SetPasswordPage() { }


void SetPasswordPage::loadPassword()
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
        }
        else
        {
            has_file = true;
            has_password = true;
            cur_hashed = content;
        }
    }
    else
    {
        File passwordFile(getHomePath() + String("/.pocket-home/.passwd"));
        if (!passwordFile.existsAsFile())
        {
            Result makeFile = passwordFile.create();
            if (makeFile.failed())
            {
                DBG("SetPasswordPage::" << __func__ 
                        << ": Failed to create password file!");
                return;
            }
            passwordFile.appendText("none\n");
        }
    }
}

bool SetPasswordPage::isPasswordCorrect(const String& pass)
{
    return !has_password || (pass == cur_hashed);
}

String SetPasswordPage::hashString(const String& string)
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

bool SetPasswordPage::passwordIdentical()
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

bool SetPasswordPage::hasPassword()
{
    //We need to often read the file in case it's manually modified
    loadPassword();
    return has_password;
}


void SetPasswordPage::savePassword()
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

void SetPasswordPage::clearAllFields()
{
    root_password.clear();
    cur_password.clear();
    new_password.clear();
    ret_password.clear();
}

void SetPasswordPage::pageButtonClicked(Button* button)
{
    if (button == &apply)
    {
        savePassword();
    }
}