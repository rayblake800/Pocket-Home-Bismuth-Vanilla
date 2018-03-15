#include <openssl/sha.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "Utils.h"
#include "RemovePasswordPage.h"

RemovePasswordPage::RemovePasswordPage() :
PageComponent("RemovePasswordPage",{
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
            {&apply, 1}}}
}, true),
root_label("RootLab", "Sudo password"), root_password("Root", 0x2022),
cur_label("CurLabel", "Current password"), cur_password("Current", 0x2022),
title("Title", "Remove your password"),
 has_file(false), has_password(false)
{
    
#if JUCE_DEBUG
    setName("RemovePasswordPage");
#endif
    title.setJustificationType(Justification::centred);
    //Load the password if existing
    loadPassword();
    apply.setButtonText("Apply");
    apply.addListener(this);
    addAndShowLayoutComponents();
}

RemovePasswordPage::~RemovePasswordPage()
{
}

void RemovePasswordPage::loadPassword()
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
                DBG("RemovePasswordPage::" << __func__ 
                        << ": Failed to create password file!");
                return;
            }
            passwordFile.appendText("none\n");
        }
    }
}

bool RemovePasswordPage::isPasswordCorrect(const String& pass)
{
    return !has_password || (pass == cur_hashed);
}

String RemovePasswordPage::hashString(const String& string)
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


bool RemovePasswordPage::hasPassword()
{
    //We need to often read the file in case it's manually modified
    loadPassword();
    return has_password;
}

void RemovePasswordPage::deletePassword()
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


void RemovePasswordPage::clearAllFields()
{
    root_password.clear();
    cur_password.clear();
}

void RemovePasswordPage::pageButtonClicked(Button* button)
{
    if (button == &apply)
    {
        deletePassword();
        clearAllFields();
    }
}