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
    
    const constexpr char * passwordPath = "~/.pocket-home/.passwd/passwd";
}
