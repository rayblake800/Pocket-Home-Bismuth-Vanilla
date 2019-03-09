#include "Password.h"
#include <openssl/sha.h>
#include <unistd.h>
#include "Assets.h"
#include "SystemCommands.h"

#ifdef JUCE_DEBUG
/* Print the full namespace name before all debug output: */
static const constexpr char* dbgPrefix = "Password::";
#endif

/* Runs with administrator privileges to create, update, or remove the hashed
 * password file: */
static const constexpr char * passwordScript = "scripts/passwordManager.sh";

/* Stores the hashed application password: */
static const constexpr char * passwordPath = "~/.pocket-home/.passwd/passwd";

/**
 * @brief  Gets the SHA256 hashed value of a string.
 * 
 * @param string  The string to encrypt.
 * 
 * @return        The hashed string value.
 */
static juce::String hashString(const juce::String& string)
{
    // Convert the string to a byte array:
    const unsigned char* stringBytes = (unsigned char*) string.toRawUTF8();

    // Get the digest:
    const int digestSize = SHA256_DIGEST_LENGTH;
    unsigned char digest[digestSize];
    SHA256(stringBytes, string.length(), digest);

    // Convert back to string and return:
    return juce::String::toHexString(digest, digestSize, 0);
}

/**
 * @brief  Checks if a saved password file exists.
 *
 * @return  Whether the password file was found.
 */
static bool passwordFileExists()
{
    return juce::File(passwordPath).existsAsFile();
}

/**
 * @brief  Checks if the saved password file is properly protected.
 *
 * @return  Whether the password file has no write access.
 */
static bool passwordFileProtected()
{
    return !juce::File(passwordPath).hasWriteAccess();
}

/**
 * @brief  Checks if pkexec is installed.
 *
 * pkexec is needed to run the password manager script as root.
 *
 * @return  Whether pkexec is installed and available.
 */
static bool pkexecInstalled()
{
    juce::ChildProcess checkCmd;
    checkCmd.start("command -v pkexec");
    return checkCmd.readAllProcessOutput().containsNonWhitespaceChars();
}

/*
 * Checks if a string matches the existing password.
 */
bool Password::checkPassword(const juce::String password)
{
    if (!isPasswordSet())
    {
        return password.isEmpty();
    }
    juce::String savedHash = juce::File(passwordPath).loadFileAsString()
            .removeCharacters("\n");
    return savedHash == hashString(password);
}

/*
 * Checks if a password has been set for the application.
 */
bool Password::isPasswordSet()
{
    juce::File pwd(passwordPath);
    return pwd.existsAsFile();
}

/*
 * Attempts to change or remove the current password.
 */
static Password::ChangeResult runPasswordScript
(const juce::String currentPass, const juce::String newPass)
{
    using namespace Password;
    if (isPasswordSet() && !checkPassword(currentPass))
    {
        return wrongPasswordError;
    }
    SystemCommands sysCommands;
    juce::String args(getlogin());
    if(newPass.isNotEmpty())
    {
        args += juce::String(" \"" + hashString(newPass) + "\"");
    }
    int result = sysCommands.runIntCommand(
            SystemCommands::IntCommand::setPassword, args);
    if (result == -1)
    {
        DBG(dbgPrefix << __func__ << ": password update command is missing!");
        return noPasswordScript;
    }
    if(newPass.isEmpty())
    {
        return passwordFileExists() ? fileDeleteFailed : passwordRemoveSuccess;
    }
    return passwordSetSuccess;
}

/*
 * Attempts to set, change, or remove the current pocket-home password.
 */
Password::ChangeResult Password::changePassword
(const juce::String currentPass, const juce::String newPass)
{
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
        juce::File(passwordPath).deleteFile();
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

/*
 * Attempts to remove the current pocket-home password.
 */
Password::ChangeResult Password::removePassword(const juce::String currentPass)
{
    ChangeResult result = runPasswordScript(currentPass, "");
    if(result != fileDeleteFailed)
    {
        return result;
    }
    return pkexecInstalled() ? result : noPKExec;  
}
