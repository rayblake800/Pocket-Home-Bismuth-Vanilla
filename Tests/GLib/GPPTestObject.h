#pragma once
#include "gtest_object.h"
#include "GPPObject.h"
#include "GSignalHandler.h"
/**
 * @file GPPTestObject.h
 *
 * @brief A minimal GPPObject test class to use for thread safety testing.
 */
class GPPTestObject : public GPPObject
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
    class Listener : private GSignalHandler
    {
    public:
        friend class GPPTestObject;
        Listener();
        
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
     */
    void removeListener(Listener& listener);
};

