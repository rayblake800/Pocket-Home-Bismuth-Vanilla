#pragma once
#include <glib-object.h>

/**
 * @file gpptest_object.h
 *
 * @brief  Defines a custom GObject type for GPPObject threading tests.
 */
G_BEGIN_DECLS

#define GPPTEST_TYPE_OBJECT            (gpptest_object_get_type ())
#define GPPTEST_OBJECT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GPPTEST_TYPE_OBJECT, GPPTestObject))
#define GPPTEST_OBJECT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GPPTEST_TYPE_OBJECT, GPPTestObjectClass))
#define GPPTEST_IS_OBJECT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GPPTEST_TYPE_OBJECT))
#define GPPTEST_IS_OBJECT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GPPTEST_TYPE_OBJECT))
#define GPPTEST_OBJECT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GPPTEST_TYPE_OBJECT, GPPTestObjectClass))

//Properties;
#define GPPTEST_OBJECT_STRING_TEST "string-test"
#define GPPTEST_OBJECT_INT_TEST    "int-test"

//Signals:
#define GPPTEST_OBJECT_TEST_SIGNAL  "test-signal"

typedef struct {
    GObject parent;
} GPPTestObject;

typedef struct {
    GObjectClass parent;
} GPPTestObjectClass;

GType gpptest_object_get_type();

G_END_DECLS

int testObjectsExisting;
