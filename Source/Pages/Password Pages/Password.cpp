#include <openssl/sha.h>
#include <unistd.h>
#include "AssetFiles.h"
#include "Password.h"

static const constexpr char * passwordScript = "scripts/passwordManager.sh";

/**
 * Gets the SHA1 hashed value of a string.
 */
String Password::hashString(const String& string)
{
    const unsigned char* str = (unsigned char*) string.toRawUTF8();
    //(unsigned char*) string.toStdString().c_str();
    unsigned char hash[21];
    char hashed[41];
    SHA1(str, string.length(), hash);
    //The Sha1sum of the string is now stored in the hash array (as a byte array)
    //So let's convert the byte array into a C string (char*)
    for (int i = 0; i < 20; i++)
    {
        sprintf(hashed + (i * 2), "%02x", hash[i]);
    }
    hashed[40] = 0;
    return String(hashed);
}

/**
 * Checks if a string value matches the saved password.
 */
bool Password::checkPassword(const String& password)
{

    File passFile(passwordPath);
    if (!isPasswordSet())
    {
        return true;
    }
    String savedHash = File(passwordPath).loadFileAsString()
            .removeCharacters("\n");
    return savedHash == hashString(password);
}

/**
 * Checks if a password has been set for the application.
 */
bool Password::isPasswordSet()
{
    File pwd(passwordPath);
    return pwd.existsAsFile() && pwd.loadFileAsString() != "none\n";
}

/**
 * Attempts to set, change, or remove the current pocket-home password, if
 * possible.
 */
bool Password::changePassword
(const String& currentPass, const String& newPass)
{
    if (!checkPassword(currentPass))
    {
        DBG("Password::" << __func__ << ": wrong password!");
        return false;
    }
    else
    {
        File pwdScriptFile = AssetFiles::findAssetFile(passwordScript, false);
        if (!pwdScriptFile.existsAsFile())
        {
            DBG("Password::" << __func__
                    << ": password update script is missing!");
            return false;
        }
        String commandStr("pkexec " + pwdScriptFile.getFullPathName() + " "
                + String(getlogin()));
        if(newPass.containsNonWhitespaceChars())
        {
            commandStr += " \"" + hashString(newPass) + "\"";
        }
        DBG("Running:"<<commandStr<<", newpass="<<newPass);
        int scriptResult = system(commandStr.toRawUTF8());
        DBG("Password::" << __func__ << "Script returned " << scriptResult);
        return scriptResult == 0 && checkPassword(newPass);
    }
}