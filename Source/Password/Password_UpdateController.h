#pragma once
/**
 * @file  Password_UpdateController.h
 *
 * @brief  Controls the set of UI components that allow the user to set or
 *         change the application password.
 */

#include "Locale_TextUser.h"
#include "Widgets_BoundedLabel.h"
#include "JuceHeader.h"

namespace Password { class UpdateController; }

/**
 * @brief  Controls the set of UI components used to set the application 
 *         password.
 * 
 *  When the password update button is clicked, the UpdateController checks the
 * contents of the text fields it manages. It ensures that the new password 
 * field matches the confirm password field, and that the value in the current 
 * password field is correct if a password is currently set. 
 *
 *  If these conditions are met, the controller attempts to change the password,
 * and shows an alert window describing if the password could be changed. If the
 * field values are not correct, it will show an alert window explaining what 
 * was wrong. In all cases, all input fields will be cleared whenever the update
 * button is clicked. Finally, a callback function may be provided on 
 * construction that will run when the password changes successfully.
 */
class Password::UpdateController : public Locale::TextUser,
        private juce::Button::Listener
{
public:
    /**
     * @brief  Starts controlling all editor components, and sets an action to
     *         perform when the password is successfully updated.
     *
     * @param currentPasswordField  The text field where the user will re-enter
     *                              the current application password.
     *
     * @param newPasswordField      The text field where the user will enter
     *                              the new application password.
     *
     * @param confirmPasswordField  The text field where the user will enter the
     *                              new password a second time to confirm it is
     *                              correct.
     *
     * @param updateButton          The button the user will click to trigger
     *                              the update attempt.
     *
     * @param onUpdate              An optional callback function to run when
     *                              the controller successfully updates the
     *                              application password.
     */
    UpdateController(juce::TextEditor& currentPasswordField,
            juce::TextEditor& newPasswordField,
            juce::TextEditor& confirmPasswordField,
            juce::Button& updateButton,
            std::function<void()> onUpdate = std::function<void()>());

    virtual ~UpdateController() { }

private:
    /**
     * @brief  Attempts to set a new password when the update button is clicked.
     *
     * @param updateButton  The password update button.
     */
    virtual void buttonClicked(juce::Button* updateButton) override;

    /**
     * @brief  Opens a message box to display an error message, and clears all 
     *         text entry fields.
     * 
     * @param title  The title to print on the message box.
     * 
     * @param error  The error message to print in the message box.
     */
    void showErrorMessage(const juce::String title, const juce::String error);

    /**
     * @brief  Clears the text in all text entry fields on the page.
     */
    void clearAllFields();

    /* Used to enter in the existing password, if one exists: */
    juce::TextEditor& currentPasswordField;

    /* Used to enter in a new password: */
    juce::TextEditor& newPasswordField;

    /* Used to confirm the new password: */
    juce::TextEditor& confirmPasswordField;

    /* Sets the password when clicked: */
    juce::Button& updateButton;

    /* An action to perform after setting the password correctly: */
    std::function<void()> passwordChangeCallback;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UpdateController)
};
