#pragma once
#include <glib-object.h>

/**
 * @file gpptest_object.h
 *
 * @brief  Defines a custom GObject type for GPPObject threading tests.
 */
G_BEGIN_DECLS

#define GTEST_TYPE_OBJECT gtest_object_get_type()

#define GTEST_OBJECT_TEST_STRING "test-string"
#define GTEST_OBJECT_TEST_INT    "test-int"

G_DECLARE_FINAL_TYPE(GTestObject, gtest_object, GTEST, OBJECT, GObject)

GTestObject* gtest_object_new();

G_END_DECLS

int gtestInstances();
