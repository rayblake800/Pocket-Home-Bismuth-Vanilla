#include <openssl/sha.h>
#include <unistd.h>
#include "AssetFiles.h"
#include "SystemCommands.h"
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
static juce::String hashString(const juce::String& string)
{
    using namespace juce;
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
    using namespace juce;
    return File(passwordPath).existsAsFile();
}

static bool passwordFileProtected()
{
    using namespace juce;
    return !File(passwordPath).hasWriteAccess();
}

static bool pkexecInstalled()
{
    using namespace juce;
    ChildProcess checkCmd;
    checkCmd.start("command -v pkexec");
    return checkCmd.readAllProcessOutput().containsNonWhitespaceChars();
}

/**
 * Checks if a string matches the existing password.
 */
bool Password::checkPassword(const juce::String& password)
{
    using namespace juce;
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
    using namespace juce;
    File pwd(passwordPath);
    return pwd.existsAsFile() && pwd.loadFileAsString().trim() != "none";
}


/**
 * Attempts to change or remove the current password.
 */
static Password::ChangeResult runPasswordScript
(const juce::String& currentPass, const juce::String& newPass)
{
    using namespace juce;
    using namespace Password;
    if (isPasswordSet() && !checkPassword(currentPass))
    {
        return wrongPasswordError;
    }
    SystemCommands sysCommands;
    String args(getlogin());
    if(newPass.isNotEmpty())
    {
        args += String(" \"" + hashString(newPass) + "\"");
    }
    int result = sysCommands.runIntCommand
        (SystemCommands::IntCommand::setPassword, args);
    if (result == -1)
    {
        DBG("Password::" << __func__
                << ": password update command is missing!");
        return noPasswordScript;
    }
    if(newPass.isEmpty())
    {
        return passwordFileExists() ? fileDeleteFailed : passwordRemoveSuccess;
    }
    return passwordSetSuccess;
}

/**
 * Attempts to set, change, or remove the current pocket-home password, if
 * possible.
 */
Password::ChangeResult Password::changePassword
(const juce::String& currentPass, const juce::String& newPass)
{
    using namespace juce;
    if (!newPass.containsNonWhitespaceChars() || currentPass == newPass)
    {
        return missingNewPassword;
    }
    ChangeResult result = runPasswordScript(currentPass, newPass);
    if(result != passwordSetSuccess)
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
    if(result == passwordSetSuccess)
    {
        return result;
    }
    return pkexecInstalled() ? result : noPKExec;
}

/**
 * Attempts to remove the current pocket-home password.
 */
Password::ChangeResult Password::removePassword(const juce::String& currentPass)
{
    ChangeResult result = runPasswordScript(currentPass, "");
    if(result != fileDeleteFailed)
    {
        return result;
    }
    return pkexecInstalled() ? result : noPKExec;  
}
