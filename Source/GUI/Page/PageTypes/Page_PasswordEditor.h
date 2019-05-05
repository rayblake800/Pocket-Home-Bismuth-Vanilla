#pragma once
/**
 * @file  Page_PasswordEditor.h
 *
 * @brief  Allows the user to set or change a password for this application.
 */

#include "Page_Component.h"
#include "Widgets_BoundedLabel.h"
#include "Locale_TextUser.h"
#include "Password_UpdateController.h"

namespace Page { class PasswordEditor; }

/**
 * @brief  A Page class that displays controls for setting or changing the
 *         application's password.
 */
class Page::PasswordEditor : public Page::Component, public Locale::TextUser
{
public:
    /**
     * @brief  Sets the page layout and initializes password editing components
     *         on construction.
     */
    PasswordEditor();

    virtual ~PasswordEditor() { }

private:
    // Page title:
    Widgets::BoundedLabel titleLabel;

    // Used to enter in the existing password, if one exists:
    Widgets::BoundedLabel currentPasswordLabel;
    juce::TextEditor currentPasswordField;

    // Used to enter in a new password:
    Widgets::BoundedLabel newPasswordLabel;
    juce::TextEditor newPasswordField;

    // Used to confirm the new password:
    Widgets::BoundedLabel confirmPasswordLabel;
    juce::TextEditor confirmPasswordField;

    // Sets the password when clicked:
    juce::TextButton updateButton;

    // Manages the password update components:
    Password::UpdateController updateController;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PasswordEditor)
};
