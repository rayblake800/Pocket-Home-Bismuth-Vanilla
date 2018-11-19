#pragma once
#include <glib-object.h>

/**
 * @file gpptest_object.h
 *
 * @brief  Defines a custom GObject type for GLib::Object threading tests.
 */
G_BEGIN_DECLS

#define GTEST_TYPE_OBJECT gtest_object_get_type()

#define GTEST_OBJECT_TEST_STRING "test-string"
#define GTEST_OBJECT_TEST_INT    "test-int"

//G_DECLARE_FINAL_TYPE(GTestObject, gtest_object, GTEST, OBJECT, GObject)

/* Debian Jesse's supported GLib version predates this macro, so for the
   PocketCHIP this has to be done the old way: */

#define GTEST_OBJECT(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            GTEST_TYPE_OBJECT, GTestObject))
#define GTEST_OBJECT_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), \
            GTEST_TYPE_OBJECT, GTestObjectClass))
#define GTEST_IS_OBJECT(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            GTEST_TYPE_OBJECT))
#define GTEST_IS_OBJECT_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            GTEST_TYPE_OBJECT))
#define GTEST_OBJECT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            (GTEST_TYPE_OBJECT, GTestObjectClass))

typedef struct _GTestObject GTestObject;
typedef struct _GTestObjectClass GTestObjectClass;

struct _GTestObjectClass
{
    GObjectClass parent_class;
};

GType gtest_object_get_type() G_GNUC_CONST;


/* end of code that G_DECLARE_FINAL_TYPE would replace */


GTestObject* gtest_object_new();

G_END_DECLS

/**
 * Gets the number of GTestObjects that currently exist.
 *
 * @return  The number of GTestObjects that have been initialized and not yet
 *          destroyed.
 */
int gtest_object_count();
