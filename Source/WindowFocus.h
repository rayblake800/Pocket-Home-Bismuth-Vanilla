/**
 * @file WindowFocus.h
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
#pragma once
#include "JuceHeader.h"
namespace WindowFocus {

    class BroadcastWindow : public DocumentWindow {
    public:
        /**
         * @param title
         * @param backgroundColour
         * @param requiredButtons
         * @see juce_DocumentWindow.h:72 for information on parameters.
         */
        BroadcastWindow(const String& title, Colour backgroundColour,
                int requiredButtons);

        virtual ~BroadcastWindow();
    private:
        /**
         * Ensure all listeners are notified when window state changes.
         */
        void activeWindowStatusChanged() override;
    };

    class Listener {
    public:
        friend class BroadcastWindow;
        friend void accessListeners(std::function
        <void(Array<WindowFocus::Listener*>&) > listenerAction);
        Listener();
        virtual ~Listener();

    private:
        /**
         * called whenever the application window gains focus.
         */
        virtual void windowFocusGained() { }

        /**
         * called whenever the application window loses focus.
         */
        virtual void windowFocusLost() { }
    };
}
