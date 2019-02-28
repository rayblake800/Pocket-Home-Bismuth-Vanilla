#pragma once
/**
 * @file  FelPage.h
 * 
 * @brief  Assists the user in flashing the PocketCHIP.
 */

#include "Widgets_BoundedLabel.h"
#include "Locale/TextUser.h"
#include "Page_Component.h"

/**
 * @brief  Provides component controls for putting PocketCHIP systems into Fel
 *         mode.
 *
 * FelPage gives the user the option to either reboot into Fel (flashing) mode, 
 * or cancel and return to the last page. If the system is not a PocketCHIP or 
 * the i2c bus is otherwise inaccessible, nothing will happen if the user clicks
 * the confirmation button.
 */
class FelPage : public Page::Component, public Locale::TextUser
{
public:
    /**
     * @brief  Loads the page layout on construction.
     */
    FelPage();

    virtual ~FelPage() { }

private:
    /**
     * @brief  Handles all button click events for the FelPage.
     */
    class PageListener final : public juce::Button::Listener
    {
    public:
        PageListener(FelPage& felPage): felPage(felPage) { }

        virtual ~PageListener() { }

    private:
        /**
         * @brief  Handles button click events for the Fel page, either 
         *         restarting into Fel mode or closing the page.
         * 
         * @param button  This should be either yesButton or noButton.
         */
        void buttonClicked(juce::Button* button) override;

        FelPage& felPage;
    };
    PageListener pageListener;

    /* If true, the yes button was clicked already and further clicks should
       be ignored. */
    bool debounce = false;

    /* Ask for confirmation */
    Widgets::BoundedLabel infoLine1;
    /* Tell the user where to find flashing instructions. */
    Widgets::BoundedLabel infoLine2;
    /* Confirm, and reboot into Fel mode */
    juce::TextButton yesButton;
    /* Cancel, and close this page */
    juce::TextButton noButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FelPage)
};
