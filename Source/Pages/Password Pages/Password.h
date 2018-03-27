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
     * @return         True iff the hashed string matches the saved password
     *                 hash value.
     */
    bool checkPassword(const String& password);
    
    /**
     * Checks if a password has been set for the application.
     * 
     * @return  true iff a password has been set.
     */
    bool isPasswordSet();

    /**
     * Checks if the password file and folder exist and are secure.
     *
     * @return  true iff the password directory and file exists, and both
     *          are locked so that only root can modify them.
     */
    bool securePasswordFileExists();

    /**
     * This covers all possible results of an attempt to change/remove
     * a password.
     */
    enum ChangeResult
    {
	    wrongPasswordError,
	    missingNewPassword,
	    noPasswordScript,
	    noPKExec,
	    noPolkitAgent,
            agentPromptClosed,
	    wrongAdminPass,
	    noRootAccess,
	    appDirNotFound,
	    fileWriteFailed,
	    fileCreateFailed,
	    fileDeleteFailed,
	    fileSecureFailed,
	    paswordSetSuccess,
	    passwordRemoveSuccess,
    };
    
    /**
     * Attempts to set or change the current pocket-home password, if possible.
     * 
     * @param currentPass  If a password is set, this must match the current
     *                      password, or the operation will fail.
     * 
     * @param newPass      The new password to set.  If this is the empty
     *                     string, the operation will fail.
     * 
     * @return  the ChangeResult that best describes the outcome of this
     *          operation.
     */
    ChangeResult changePassword
	    (const String& currentPass, const String& newPass);
    
    /**
     * Attempts to remove the current pocket-home password.
     *
     * 
     * @param currentPass  If a password is set, this must match the current
     *                      password, or the operation will fail.
     *  
     * @return  the ChangeResult that best describes the outcome of this
     *          operation.
     *
     */
    ChangeResult removePassword(const String& currentPass);

    const constexpr char * passwordPath = "~/.pocket-home/.passwd/passwd";
}
