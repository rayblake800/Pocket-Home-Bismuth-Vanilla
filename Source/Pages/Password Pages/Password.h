/**
 * @file Password.h
 * 
 * Password provides functions for hashing password strings and checking
 * hash values against the current application password.
 */
#pragma once
#include "JuceHeader.h"

namespace Password
{
    /**
     * Gets the SHA1 hashed value of a string.
     * 
     * @param string   The string to encrypt.
     * 
     * @return         the hashed string value.
     */
    String hashString(const String& string);
    
    /**
     * Checks if a string matches the existing password.
     * 
     * @param password  A string value to check against the password.
     * 
     * @return        True iff the hashed string matches the saved password
     *                 hash value, or if there is no password set.
     */
    bool checkPassword(const String& password);
    
    /**
     * Checks if a password has been set for the application.
     * 
     * @return  true iff a password has been set.
     */
    bool isPasswordSet();
    
    /**
     * Attempts to set, change, or remove the current pocket-home password, if
     * possible.
     * 
     * @param currentPass  If a password is set, this must match the current
     *                      password, or the operation will fail.
     * 
     * @param newPass      The new password to set.  If this is the empty
     *                      string, the password will be removed entirely.
     * 
     * @return  true iff the requested password change succeeded.
     */
    bool changePassword(const String& currentPass, const String& newPass);
    
    const constexpr char * passwordPath = "~/.pocket-home/.passwd/passwd";
}
