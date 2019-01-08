#pragma once
#include "gtest_object.h"
#include "GLib_Owned_Object.h"
#include "GLib_SignalHandler.h"
#include <map>

/**
 * @file GPPTestObject.h
 *
 * @brief A minimal GLib::Object test class to use for thread safety testing.
 */
class GPPTestObject : public GLib::Owned::Object
{
public:
    /**
     * Creates a GPPTestObject containing a new GTestObject.
     */
    GPPTestObject();

    /**
     * Creates a GPPTestObject sharing data with another GPPTestObject.
     *
     * @param toCopy  An existing GPPTestObject instance.
     */
    GPPTestObject(const GPPTestObject& toCopy);
    
    /**
     * Creates a GPPTestObject holding a GTestObject. 
     *
     * @param toAssign  The GTestObject instance to assign.
     */
    GPPTestObject(const GTestObject* toAssign);
    virtual ~GPPTestObject() { }

    /**
     * Gets the GTestObject testString value.
     *
     * @return  The testString value, or the empty string if this object is
     *          null.
     */
    juce::String getTestString();
    
    /**
     * Gets the GTestObject testInt value. 
     *
     * @return  The testInt value, or -1 if this object is null.
     */
    int getTestInt();

    /**
     * Changes the GTestObject testString value, or does nothing if this object
     * is null.
     *
     * @param newString  The new value to store as the test string.
     */
    void setTestString(juce::String newString);
    
    /**
     * Changes the GTestObject testInt value, or does nothing if this object is
     * null.
     *
     * @param newInt  The new value to store as the test int.
     */
    void setTestInt(int newInt);

    /**
     * Receives property change notifications from GTestObjects.
     */
    class Listener : private GLib::SignalHandler
    {
    public:
        friend class GPPTestObject;
        Listener();
    
        /**
         * Checks if this object is listening to a particular GPPTestObject.
         *
         * @param source  A test object to search for in this object's
         *                subscribed signal sources.
         *
         * @return  True iff this Listener is listening to the source object.
         */
        bool isListening(const GPPTestObject& source) const;
        
    protected:
        /**
         * Connects to property change signals from a GTestObject.
         *
         * @param source A valid GTestObject instance.
         */
        virtual void connectAllSignals(GObject* source) override;

    private:
        /**
         * Detects property change signals, and checks that the signals are
         * valid.
         *
         * @param source    A connected GTestObject.
         *
         * @param property  One of the GTestObject properties.
         */
        virtual void propertyChanged(GObject* source, juce::String property)
            override;

        std::map<GObject*, juce::String> trackedStrings;
        std::map<GObject*, int> trackedInts;
    };

    /**
     * Adds a new listener to this GPPTestObject's property changes.
     *
     * @param listener  The listener object to connect.
     */
    void addListener(Listener& listener);

    /**
     * Disconnects a listener from this GPPTestObject's property changes.
     *
     * @param listener The listener object to disconnect.
     *
     * @return  True if the listener was removed from this object, false if the
     *          listener was never listening to this object.
     */
    bool removeListener(Listener& listener);
    
    
    /**
     * Checks if this GPPTestObject is being listened to by a particular
     * listener.
     *
     * @param listener   The listener object to test.
     *
     * @return  True iff the listener is listening to this GPPTestObject's
     *          signals.
     */
    bool isConnected(const Listener& listener) const;
};

