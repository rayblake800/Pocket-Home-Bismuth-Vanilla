#pragma once
#include "JuceHeader.h"

/**
 * @file WindowFocus.h
 * 
 * @brief  Defines an abstract base object that is notified whenever the program
 *         gains or loses window focus.
 * 
 * Defines WindowFocus::Listener and WindowFocus::BroadcastWindow.
 * 
 * Whenever a BroadcastWindow gains or loses focus, it will notify all Listeners
 * of the focus change event.
 * 
 * Since all background tasks absolutely must shut down when the window loses 
 * focus and restore themselves when it regains focus, Listener objects will 
 * automatically register themselves on creation and unregister themselves on 
 * destruction.
 */
namespace WindowFocus {

    /**
     * Checks if the main application window is currently focused.
     *
     * @return   True iff the application window exists and is currently
     *           focused.
     */
    bool isFocused();

    class BroadcastWindow : public DocumentWindow {
    public:
        /**
         * @param title              Sets the window's title as printed on the 
         *                            title bar.
         * 
         * @param backgroundColour   The background color to draw behind window
         *                            components.
         * 
         * @param requiredButtons    Sets which title bar buttons will be
         *                            shown.
         * 
         * @see juce_DocumentWindow.h
         */
        BroadcastWindow(const String& title, Colour backgroundColour,
                int requiredButtons);

        virtual ~BroadcastWindow() { }
        
    private:
        /**
         * Ensure all listeners are notified when window state changes.
         */
        void activeWindowStatusChanged() override;
    };

    class Listener {
    public:
        friend class BroadcastWindow;
        friend void accessListeners
        (std::function<void(Array<WindowFocus::Listener*>&) > listenerAction);
        
        /**
         * Adds itself to the Listener list on creation.
         */
        Listener();
        
        /**
         * Removes itself from the listener list on destruction.
         */
        virtual ~Listener();

    private:
        /**
         * Called whenever the application window gains focus, override to
         * trigger actions when window focus is gained.
         */
        virtual void windowFocusGained() { }

        /**
         * Called whenever the application window loses focus, override to
         * trigger actions when window focus is lost.
         */
        virtual void windowFocusLost() { }
    };
}
