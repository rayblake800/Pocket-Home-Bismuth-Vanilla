#include "GLib_Test_OwnedObject.h"

// Creates an OwnedObject containing a new GTestObject.
GLib::Test::OwnedObject::OwnedObject() :
GLib::Owned::Object(GTEST_TYPE_OBJECT)
{
    GObject* object = G_OBJECT(gtest_object_new());
    setGObject(object);
}


// Creates an OwnedObject sharing data with another OwnedObject.
GLib::Test::OwnedObject::OwnedObject(const OwnedObject& toCopy) :
GLib::Owned::Object(toCopy, GTEST_TYPE_OBJECT) { }


// Creates an OwnedObject holding a GTestObject.
GLib::Test::OwnedObject::OwnedObject(const GTestObject* toAssign) :
GLib::Owned::Object(G_OBJECT(toAssign), GTEST_TYPE_OBJECT) { }


// Gets the object's testString value.
juce::String GLib::Test::OwnedObject::getTestString() const
{
    juce::String value;
    if (!isNull())
    {
        gchar* property = getProperty<char*> (GTEST_OBJECT_TEST_STRING);
        if (property != nullptr)
        {
            value = juce::CharPointer_UTF8(property);
            //DBG("Found testString value " << value);
            g_free(property);
        }
    }
    return value;
}


// Gets the object's testInt value.
int GLib::Test::OwnedObject::getTestInt() const
{
    int value = 0;
    if (!isNull())
    {
        value = getProperty<gint>(GTEST_OBJECT_TEST_INT);
        //DBG("Found testInt value " << value);
    }
    return value;
}


// Changes the object's testString value, or does nothing if this object is
// null.
void GLib::Test::OwnedObject::setTestString(juce::String newString)
{
    //DBG("Setting testString value " << newString);
    setProperty<const char*> (GTEST_OBJECT_TEST_STRING, newString.toRawUTF8());
}


// Changes the object's testInt value, or does nothing if this object is null.
void GLib::Test::OwnedObject::setTestInt(int newInt)
{
    //DBG("Setting testInt value " << newInt);
    setProperty<int>(GTEST_OBJECT_TEST_INT, newInt);
}

GLib::Test::OwnedObject::Listener::Listener() { }

GLib::Test::OwnedObject::Listener::~Listener() { }


// Connects to property change signals from a GTestObject.
void GLib::Test::OwnedObject::Listener::connectAllSignals
(const OwnedObject source)
{
    if (!source.isNull())
    {
        createPropertyConnection(GTEST_OBJECT_TEST_STRING, source);
        createPropertyConnection(GTEST_OBJECT_TEST_INT, source);
        ObjectPtr sourcePtr(source);
        trackedStrings[sourcePtr] = juce::String();
        trackedInts[sourcePtr] = 0;
    }
}


// Detects property change signals, and checks that the signals are valid.
void GLib::Test::OwnedObject::Listener::propertyChanged
(const OwnedObject source, juce::String property)
{
    using namespace juce;
    ObjectPtr sourcePtr(source);
    if (property == GTEST_OBJECT_TEST_STRING)
    {
        String newStr = source.getTestString();
        jassert(newStr.length() > trackedStrings[sourcePtr].length());
        trackedStrings[sourcePtr] = newStr;
    }
    else if (property == GTEST_OBJECT_TEST_INT)
    {
        int newInt = source.getTestInt();
        jassert(newInt > trackedInts[sourcePtr]);
        trackedInts[sourcePtr] = newInt;
    }
    else
    {
        jassertfalse;
    }
}


// Adds a new listener to this OwnedObject's property changes.
void GLib::Test::OwnedObject::addListener(Listener& listener)
{
    listener.connectAllSignals(*this);
}


// Disconnects a listener from this OwnedObject's property changes.
bool GLib::Test::OwnedObject::removeListener(Listener& listener)
{
    return listener.disconnectSignals(*this);
}
