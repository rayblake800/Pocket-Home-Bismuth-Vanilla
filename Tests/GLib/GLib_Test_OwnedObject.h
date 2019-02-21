#pragma once
/**
 * @file  GLib_Test_OwnedObject.h
 *
 * @brief  A minimal GLib::Owned::Object test class to use for thread safety 
 *         testing.
 */

#include "gtest_object.h"
#include "GLib_Owned_Object.h"
#include "GLib_Signal_Handler.h"
#include <map>

namespace GLib { namespace Test { class OwnedObject; } }

class GLib::Test::OwnedObject : public GLib::Owned::Object
{
public:
    /**
     * @brief  Creates an OwnedObject containing a new GTestObject.
     */
    OwnedObject();

    /**
     * @brief  Creates an OwnedObject sharing data with another OwnedObject.
     *
     * @param toCopy  An existing OwnedObject instance.
     */
    OwnedObject(const OwnedObject& toCopy);
    
    /**
     * @brief  Creates an OwnedObject holding a GTestObject. 
     *
     * @param toAssign  The GTestObject instance to assign.
     */
    OwnedObject(const GTestObject* toAssign);

    virtual ~OwnedObject() { }

    /**
     * @brief  Gets the object's testString value.
     *
     * @return  The testString value, or the empty string if this object is
     *          null.
     */
    juce::String getTestString() const;
    
    /**
     * @brief  Gets the object's testInt value. 
     *
     * @return  The testInt value, or -1 if this object is null.
     */
    int getTestInt() const;

    /**
     * @brief  Changes the object's testString value, or does nothing if this 
     *         object is null.
     *
     * @param newString  The new value to store as the test string.
     */
    void setTestString(juce::String newString);
    
    /**
     * @brief  Changes the object's testInt value, or does nothing if this 
     *         object is null.
     *
     * @param newInt  The new value to store as the test int.
     */
    void setTestInt(int newInt);

    /**
     * @brief  Receives property change notifications from OwnedObjects.
     */
    class Listener : private GLib::Signal::Handler<OwnedObject>
    {
    public:
        friend class OwnedObject;

        Listener();

        virtual ~Listener();

        using GLib::Signal::Handler<OwnedObject>::isConnected;
        
    protected:
        /**
         * @brief  Connects to property change signals from an OwnedObject.
         *
         * @param source  An OwnedObject to connect.
         */
        virtual void connectAllSignals(const OwnedObject source) override;

    private:
        /**
         * @brief  Handles property change signals, and checks that the signals 
         *         are valid.
         *
         * @param source    A connected OwnedObject.
         *
         * @param property  The updated GTestObject property.
         */
        virtual void propertyChanged(const OwnedObject source, 
                juce::String property) override;

        std::map<GObject*, juce::String> trackedStrings;
        std::map<GObject*, int> trackedInts;
    };

    /**
     * Adds a new listener to this OwnedObject's property updates.
     *
     * @param listener  The listener object to connect.
     */
    void addListener(Listener& listener);

    /**
     * Disconnects a listener from this OwnedObject's property updates.
     *
     * @param listener  The listener object to disconnect.
     *
     * @return          True if the listener was removed from this object, 
     *                  false if the listener was never listening to this 
     *                  object.
     */
    bool removeListener(Listener& listener);
};

