#pragma once

//https://developer.gnome.org/gobject/stable/howto-gobject.html


#include <glib-object.h>

G_BEGIN_DECLS

#define TEST_OBJECT_TYPE gpp_test_object_get_type()
G_DECLARE_FINAL_TYPE(TestObject, test_object, TEST, OBJECT, GObject)

TestObject* test_object_new();

G_END_DECLS
