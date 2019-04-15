#include "gtest_object.h"
#include "JuceHeader.h"
#include <limits>

static int objectCount = 0;
static juce::CriticalSection countLock;

struct _GTestObject
{
    GObject parent_instance;
    gchar* testString;
    gint testInt;
};

G_DEFINE_TYPE(GTestObject, gtest_object, G_TYPE_OBJECT);

static void gtest_object_init(GTestObject* object)
{
    juce::ScopedLock lock(countLock);
    objectCount++;
}

static void finalize(GObject* object)
{
    GTestObject* testObject = GTEST_OBJECT(object);
    if (testObject->testString != NULL)
    {
        g_free(testObject->testString);
        testObject->testString = NULL;
    }
    juce::ScopedLock lock(countLock);
    objectCount--;
}

enum Properties
{
    testStringProp = 1,
    testIntProp,
    numProperties
};

static GParamSpec* objectProperties[numProperties] = { NULL, };

static void set_property(GObject* object, guint prop_id,
        const GValue* value, GParamSpec* pspec)
{
    GTestObject* testObject = GTEST_OBJECT(object);
    switch(prop_id)
    {
        case testStringProp:
            testObject->testString = g_value_dup_string(value);
            break;
        case testIntProp:
            testObject->testInt = g_value_get_int(value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void get_property(GObject* object, guint prop_id,
        GValue* value, GParamSpec* pspec)
{
    GTestObject* testObject = GTEST_OBJECT(object);
    switch(prop_id)
    {
        case testStringProp:
            g_value_set_string(value, testObject->testString);
            break;
        case testIntProp:
            g_value_set_int(value, testObject->testInt);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

void gtest_object_class_init(GTestObjectClass* klass)
{
    GObjectClass* objectClass = G_OBJECT_CLASS(klass);
    objectClass->set_property = set_property;
    objectClass->get_property = get_property;
    objectClass->finalize = finalize;

    objectProperties[testStringProp] = g_param_spec_string(
            GTEST_OBJECT_TEST_STRING,
            "testString",
            "Testing thread safety with string editing",
            NULL,
            G_PARAM_READWRITE);
    objectProperties[testIntProp] = g_param_spec_int(
            GTEST_OBJECT_TEST_INT,
            "testInt",
            "Testing thread safety with int editing",
            std::numeric_limits<int>::min(),
            std::numeric_limits<int>::max(),
            0,
            G_PARAM_READWRITE);
    g_object_class_install_properties(objectClass, numProperties,
            objectProperties);
}

GTestObject* gtest_object_new()
{
    return GTEST_OBJECT(g_type_create_instance(GTEST_TYPE_OBJECT));
}

int gtest_object_count()
{
    juce::ScopedLock lock(countLock);
    return objectCount;
}
