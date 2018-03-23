#include <openssl/sha.h>
#include "Password.h"

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
 * Checks if a hashed string value matches the save password.
 */
bool Password::checkPassword(const String& hashStr)
{

    File passFile(passwordPath);
    if (!isPasswordSet())
    {
        return true;
    }
    String savedHash = File(passwordPath).loadFileAsString()
            .removeCharacters("\n");
    DBG("Checking \""<<hashStr<<"\" against \"" << savedHash << "\"");
    return savedHash == hashStr;
}

/**
 * Checks if a password has been set for the application.
 */
bool Password::isPasswordSet()
{
    File pwd(passwordPath);
    return pwd.existsAsFile() && pwd.loadFileAsString() != "none\n";
}