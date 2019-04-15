#pragma once
/**
 * @file  Page_PasswordRemover.h
 *
 * @brief  Allows the user to remove the application password.
 */

#include "Page_Component.h"
#include "Password_RemovalController.h"
#include "Locale_TextUser.h"
#include "Widgets_BoundedLabel.h"
#include "JuceHeader.h"

namespace Page { class PasswordRemover; }

/**
 * @brief  A Page class that shows controls that allow the user to remove the
 *         application's password.
 */
class Page::PasswordRemover : public Page::Component, public Locale::TextUser
{
public:
    /**
     * @brief  Sets the page layout and initializes password removal
     *         functionality on construction.
     */
    PasswordRemover();

    virtual ~PasswordRemover() { }

private:
    // Title of the page:
    Widgets::BoundedLabel titleLabel;

    // Text field for the current password:
    Widgets::BoundedLabel currentPasswordLabel;
    juce::TextEditor currentPasswordField;

    // Triggers the attempted password removal:
    juce::TextButton removerButton;

    // Provides password removal functionality:
    Password::RemovalController removalController;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PasswordRemover)
};
