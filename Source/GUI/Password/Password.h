#pragma once
#include "JuceHeader.h"

/**
 * @file  Password.h
 *
 * @brief  Provides functions for securely setting, removing, or checking the
 *         application password.
 */

namespace Password
{

    /**
     * @brief  Checks if a string matches the existing password.
     *
     * @param password  A string value to check against the password.
     *
     * @return          Whether the string matches the application password.
     */
    bool checkPassword(const juce::String password);

    /**
     * @brief  Checks if a password has been set for the application.
     *
     * @return  Whether a password has been set.
     */
    bool isPasswordSet();

    /**
     * @brief  All possible results of an attempt to change or remove a
     *         password.
     */
    enum ChangeResult
    {
        missingNewPassword,
        wrongPasswordError,
        noPasswordScript,
        noPKExec,
        fileWriteFailed,
        fileCreateFailed,
        fileDeleteFailed,
        fileSecureFailed,
        passwordSetSuccess,
        passwordRemoveSuccess,
    };

    /**
     * @brief  Attempts to set or change the current application password.
     *
     * @param currentPassword  A string to check against the current password
     *                         before allowing the password to be changed.
     *
     * @param newPassword      The new password to set. If this is the empty
     *                         string, the operation will fail.
     *
     * @return                 The ChangeResult that best describes the result
     *                         of the password update attempt.
     */
    ChangeResult changePassword(const juce::String currentPassword,
            const juce::String newPassword);

    /**
     * @brief  Attempts to remove the current pocket-home password.
     *
     * @param currentPassword  A string to check against the current password
     *                         before allowing the password to be removed.
     *
     * @return                 The ChangeResult that best describes the result
     *                         of the password removal attempt.
     */
    ChangeResult removePassword(const juce::String currentPassword);
}
