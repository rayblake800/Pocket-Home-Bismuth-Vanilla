#pragma once
/**
 * @file  Password_RemovalController.h
 *
 * @brief  Configures a text field and button so they can be used to remove the
 *         application password.
 */

#include "Locale_TextUser.h"
#include "Widgets_BoundedLabel.h"
#include "JuceHeader.h"

namespace Password { class RemovalController; }

/**
 * @brief  Manages the behavior of the text field and button used to remove the
 *         application password.
 *
 *  When the user clicks its password remover button, the controller checks the
 * password entered into its password field against the current application
 * password. If they match, it attempts to remove the current application
 * password.
 */
class Password::RemovalController : public Locale::TextUser,
        private juce::Button::Listener
{
public:
    /**
     * @brief  Connects the removal controller to the UI components it will
     *         control, and sets the action it should take after the password is
     *         removed.
     *
     * @param passwordField  The text field used to enter the user's current
     *                       password.
     *
     * @param removerButton  The button the user clicks to trigger the password
     *                       removal attempt.
     *
     * @param onRemove       A callback function to run when the password is
     *                       removed successfully.
     */
    RemovalController(juce::TextEditor& passwordField,
            juce::Button& removerButton, const std::function<void()> onRemove);

    virtual ~RemovalController() { }

private:
    /**
     * @brief  Attempts to delete the application password when the remove
     *         button is pressed.
     *
     * @param button  The password remover button.
     */
    void buttonClicked(juce::Button* button) override;

    // Text field for entering the current application password:
    juce::TextEditor& passwordField;

    // Triggers the attempted password removal:
    juce::Button& removerButton;

    // A callback function to run when the password is removed successfully.
    std::function<void()> onRemoveCallback;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RemovalController)
};
