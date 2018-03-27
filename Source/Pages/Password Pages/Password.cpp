#include <openssl/sha.h>
#include <unistd.h>
#include "AssetFiles.h"
#include "Password.h"

static const constexpr char * passwordScript = "scripts/passwordManager.sh";

static const constexpr char * passwordPath = "~/.pocket-home/.passwd/passwd";

/**
 * Gets the SHA1 hashed value of a string.
 * 
 * @param string   The string to encrypt.
 * 
 * @return         the hashed string value.
 */
static String hashString(const String& string)
{
    const unsigned char* str = (unsigned char*) string.toRawUTF8();
    //(unsigned char*) string.toStdString().c_str();
    unsigned char hash[21];
    char hashed[41];
    SHA1(str, string.length(), hash);
    //The Sha1sum of the string is now stored in the hash array(as a byte array)
    //So let's convert the byte array into a C string (char*)
    for (int i = 0; i < 20; i++)
    {
        sprintf(hashed + (i * 2), "%02x", hash[i]);
    }
    hashed[40] = 0;
    return String(hashed);
}

static bool passwordFileExists()
{
    return File(passwordPath).existsAsFile();
}

static bool passwordFileProtected()
{
    return !File(passwordPath).hasWriteAccess();
}

static bool pkexecInstalled()
{
    return system("command -v pkexec") != 0;
}

/**
 * Checks if a string matches the existing password.
 */
bool Password::checkPassword(const String& password)
{
    if (!isPasswordSet())
    {
        return password.isEmpty();
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
    return pwd.existsAsFile() && pwd.loadFileAsString().trim() != "none";
}


/**
 * Attempts to change or remove the current password.
 */
static Password::ChangeResult runPasswordScript
(const String& currentPass, const String& newPass)
{
    using namespace Password;
    if (isPasswordSet() && !checkPassword(currentPass))
    {
        return wrongPasswordError;
    }
    File pwdScriptFile = AssetFiles::findAssetFile(passwordScript, false);
    if (!pwdScriptFile.existsAsFile())
    {
        DBG("Password::" << __func__
                << ": password update script is missing!");
        return noPasswordScript;
    }
    String commandStr("pkexec " + pwdScriptFile.getFullPathName() + " "
            + String(getlogin()));
    if (newPass.isNotEmpty())
    {
        commandStr += String(" \"" + hashString(newPass) + "\"");
    }
    system(commandStr.toRawUTF8());
    if(newPass.isEmpty())
    {
        return passwordFileExists() ? fileDeleteFailed : passwordRemoveSuccess;
    }
    return paswordSetSuccess;
}

/**
 * Attempts to set, change, or remove the current pocket-home password, if
 * possible.
 */
Password::ChangeResult Password::changePassword
(const String& currentPass, const String& newPass)
{
    if (!newPass.containsNonWhitespaceChars() || currentPass == newPass)
    {
        return missingNewPassword;
    }
    ChangeResult result = runPasswordScript(currentPass, newPass);
    if(result != paswordSetSuccess)
    {
        return result;
    }
    if(!passwordFileExists())
    {
        result = fileCreateFailed;
    }
    if(!passwordFileProtected())
    {
        File(passwordPath).deleteFile();
        result = fileSecureFailed;
    }
    if(!checkPassword(newPass)){
        result = fileWriteFailed;
    }
    if(result == paswordSetSuccess)
    {
        return result;
    }
    return pkexecInstalled() ? result : noPKExec;
}

/**
 * Attempts to remove the current pocket-home password.
 */
Password::ChangeResult Password::removePassword(const String& currentPass)
{
    ChangeResult result = runPasswordScript(currentPass, "");
    if(result != fileDeleteFailed)
    {
        return result;
    }
    return pkexecInstalled() ? result : noPKExec;  
}
#if JUCE_DEBUG
void Password::scriptChecksum()
{
    File scriptFile = AssetFiles::findAssetFile(passwordScript, false);
    if(!scriptFile.existsAsFile())
    {
        //Finish tomorrow
        DBG("");
    }
}
#endif