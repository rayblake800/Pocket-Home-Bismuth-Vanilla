#include "Password.h"
#include "Util_Commands.h"
#include "Assets.h"
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <unistd.h>
#include <pwd.h>

#ifdef JUCE_DEBUG
// Print the full namespace name before all debug output:
static const constexpr char* dbgPrefix = "Password::";
#endif

// Runs with administrator privileges to create, update, or remove the hashed
// password file:
static const constexpr char* passwordScript = "scripts/passwordManager.sh";

// Application data directory, relative to the user home directory:
// This ignores XDG_DATA_HOME and always uses the default data directory so the
// password can't be bypassed or changed by redefining $XDG_DATA_HOME.
static const constexpr char* dataDir = "/.local/share/pocket-home/";

// Password file path, relative to the application data directory:
static const constexpr char* passwordFile = ".passwd/passwd";

// Password salt length in bytes:
static const constexpr int saltLength = 16;

/**
 * @brief  Gets the application password file.
 *
 *  If the application data directory does not exist, this will also create
 * that directory.
 *
 * @return  The application password file, or a File object pointing to where
 *          the password file would be if it existed.
 */
juce::File getPasswordFile()
{
    juce::File dataDirFile(juce::String(getenv("HOME")) + dataDir);
    if (!dataDirFile.isDirectory())
    {
        dataDirFile.createDirectory();
    }
    return dataDirFile.getChildFile(passwordFile);
}


/**
 * @brief  Holds the two pieces of data stored in the password file.
 */
struct FileData
{
    juce::String hashedPassword;
    juce::String salt;
};

/**
 * @brief  Loads data from the user's application password file.
 *
 * @return  The user's hashed password and salt value, or a FileData containing
 *          empty strings if the password file is invalid.
 */
FileData loadPasswordFileData()
{
    FileData fileData;
    using juce::String;
    juce::File passwordFile = getPasswordFile();
    String saltString;
    if (passwordFile.existsAsFile() && !passwordFile.hasWriteAccess())
    {
        juce::StringArray passwordLines;
        passwordFile.readLines(passwordLines);
        if (passwordLines.size() == 2)
        {
            fileData.hashedPassword = passwordLines[0];
            fileData.salt = passwordLines[1];
        }
    }
    return fileData;
}

/**
 * @brief  Converts a string of hex values into a byte array.
 *
 * @param hexString  A string of hexadecimal byte values.
 *
 * @return           The byte array represented by the string of hex values.
 */
juce::Array<juce::uint8> toByteArray(const juce::String& hexString)
{
    juce::Array<juce::uint8> byteArray;
    for (int i = 0; i < (hexString.length() - 1); i += 2)
    {
        juce::String byteString = hexString.substring(i, i + 2);
        byteArray.add(byteString.getHexValue32());
    }
    return byteArray;
}

/**
 * @brief  Generates a random password salt value.
 *
 * @return  An array of random bytes to use as a password salt.
 */
juce::Array<juce::uint8> generateSalt()
{
    unsigned char salt [saltLength] = {0};
    RAND_poll();
    RAND_bytes(salt, saltLength);
    return juce::Array<juce::uint8>(salt, saltLength);
}

/**
 * @brief  Gets the SHA256 hashed value of a string.
 *
 * @param string     The string to encrypt.
 *
 * @param saltValue  An array of random bytes to use as a password salt value.
 *
 * @return           The hashed string value.
 */
static juce::String getHashString(const juce::String& string,
        const juce::Array<juce::uint8>& saltValue)
{
    // Convert the string to a byte array and append it to the salt:
    juce::Array<juce::uint8> data = saltValue;
    data.addArray(reinterpret_cast<const unsigned char*> (string.toRawUTF8()),
            string.length());

    // Get the digest:
    const int digestSize = SHA256_DIGEST_LENGTH;
    unsigned char digest[digestSize];
    SHA256(data.getRawDataPointer(), data.size(), digest);

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
    return getPasswordFile().existsAsFile();
}

/**
 * @brief  Checks if the saved password file is properly protected.
 *
 * @return  Whether the password file has no write access.
 */
static bool passwordFileProtected()
{
    return !getPasswordFile().hasWriteAccess();
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
    static const constexpr char* testCommand = "pkexec";
    Util::Commands systemCommands;
    return systemCommands.runIntCommand(Util::CommandTypes::Int::commandCheck,
            testCommand) == 0;
}


// Checks if a string matches the existing password.
bool Password::checkPassword(const juce::String password)
{
    if (!isPasswordSet())
    {
        return password.isEmpty();
    }
    FileData passwordData = loadPasswordFileData();
    jassert(passwordData.hashedPassword.isNotEmpty());
    jassert(passwordData.salt.isNotEmpty());
    juce::Array<juce::uint8> salt = toByteArray(passwordData.salt);
    juce::String testHash = getHashString(password, salt);
    return testHash == passwordData.hashedPassword;
}


// Checks if a password has been set for the application.
bool Password::isPasswordSet()
{
    juce::File passwordFile = getPasswordFile();
    return passwordFile.existsAsFile() && !passwordFile.hasWriteAccess()
            && passwordFile.getSize() > 0;
}


// Attempts to change or remove the current password.
static Password::ChangeResult runPasswordScript
(const juce::String currentPass, const juce::String newPass)
{
    using namespace Password;
    if (isPasswordSet() && ! checkPassword(currentPass))
    {
        return wrongPasswordError;
    }

    // Read and check username argument:
    char* username = getlogin();
    if (username == nullptr)
    {
        struct passwd* pwEntry = getpwuid(getuid());
        if (pwEntry != nullptr)
        {
            username = pwEntry->pw_name;
        }
        if (username == nullptr)
        {
            username = getenv("LOGNAME");
        }
    }
    if (username == nullptr)
    {
        DBG(dbgPrefix << __func__ << ": Failed to get username.");
        jassertfalse;
        return ChangeResult::noPasswordScript;
    }

    juce::String args(username);
    if (args.isEmpty())
    {
        DBG(dbgPrefix << __func__ << ": Found empty username!");
        jassertfalse;
        return ChangeResult::noPasswordScript;
    }

    // If setting a password, generate hash and salt arguments:
    if (newPass.isNotEmpty())
    {
        juce::Array<juce::uint8> salt = generateSalt();
        juce::String hashedPassword = getHashString(newPass, salt);
        juce::String saltString = juce::String::toHexString(
                salt.getRawDataPointer(), salt.size(), 0);
        args += juce::String(" \"" + hashedPassword + "\"");
        args += juce::String(" \"" + saltString + "\"");
    }

    // Have Util::Commands locate and run the script:
    Util::Commands sysCommands;
    int result = sysCommands.runIntCommand(Util::CommandTypes::Int::setPassword,
            args);
    if (result == -1)
    {
        DBG(dbgPrefix << __func__ << ": password update command is missing!");
        return noPasswordScript;
    }
    if (newPass.isEmpty())
    {
        return passwordFileExists() ? fileDeleteFailed : passwordRemoveSuccess;
    }
    return passwordSetSuccess;
}


// Attempts to set, change, or remove the current pocket-home password.
Password::ChangeResult Password::changePassword
(const juce::String currentPass, const juce::String newPass)
{
    if (!newPass.containsNonWhitespaceChars() || currentPass == newPass)
    {
        return missingNewPassword;
    }
    ChangeResult result = runPasswordScript(currentPass, newPass);
    if (result != passwordSetSuccess)
    {
        return result;
    }
    if (!passwordFileExists())
    {
        result = fileCreateFailed;
    }
    if (!passwordFileProtected())
    {
        getPasswordFile().deleteFile();
        result = fileSecureFailed;
    }
    if (!checkPassword(newPass)){
        result = fileWriteFailed;
    }
    if (result == passwordSetSuccess)
    {
        return result;
    }
    return pkexecInstalled() ? result : noPKExec;
}


// Attempts to remove the current pocket-home password.
Password::ChangeResult Password::removePassword
(const juce::String currentPass)
{
    ChangeResult result = runPasswordScript(currentPass, "");
    if (result != fileDeleteFailed)
    {
        return result;
    }
    return pkexecInstalled() ? result : noPKExec;
}
