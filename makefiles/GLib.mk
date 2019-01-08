############################# GLib Module ######################################
GLIB_PREFIX := $(JUCE_OBJDIR)/GLib_
GLIB_TEST_PREFIX := $(GLIB_PREFIX)Test_

# Module directories:
GLIB_ROOT = Source/GLib
GLIB_PTR_ROOT := $(GLIB_ROOT)/SmartPointers
GLIB_OWNED_ROOT := $(GLIB_ROOT)/Owned
GLIB_BORROWED_ROOT := $(GLIB_ROOT)/Borrowed
GLIB_THREAD_ROOT := $(GLIB_ROOT)/Thread
GLIB_DBUS_ROOT := $(GLIB_ROOT)/DBus
GLIB_VAR_ROOT := $(GLIB_DBUS_ROOT)/Variant
GLIB_TEST_ROOT = Tests/GLib

OBJECTS_GLIB_SMART_PTRS := \
  $(GLIB_PREFIX)ObjectPtr.o \
  $(GLIB_PREFIX)ErrorPtr.o \
  $(GLIB_PREFIX)SharedContextPtr.o

OBJECTS_GLIB_OWNED := \
  $(GLIB_PREFIX)Owned_Object.o \

OBJECTS_GLIB_BORROWED := \
  $(GLIB_PREFIX)Borrowed_Object.o \
  $(GLIB_PREFIX)SharedContainer.o

OBJECTS_GLIB_THREAD := \
  $(GLIB_PREFIX)EventLoop.o \
  $(GLIB_PREFIX)ContextCaller.o \
  $(GLIB_PREFIX)ThreadResource.o \
  $(GLIB_PREFIX)ThreadHandler.o

OBJECTS_GLIB_DBUS := \
  $(GLIB_PREFIX)DBusProxy.o \
  $(GLIB_PREFIX)DBusThread.o

OBJECTS_GLIB_VAR := \
  $(GLIB_PREFIX)VariantConverter.o \
  $(GLIB_PREFIX)TypeHelper.o

OBJECTS_GLIB := \
  $(GLIB_PREFIX)WeakRef.o \
  $(GLIB_PREFIX)Object.o \
  $(GLIB_PREFIX)SignalHandler.o \
  $(OBJECTS_GLIB_OWNED) \
  $(OBJECTS_GLIB_BORROWED) \
  $(OBJECTS_GLIB_THREAD) \
  $(OBJECTS_GLIB_SMART_PTRS) \
  $(OBJECTS_GLIB_DBUS) \
  $(OBJECTS_GLIB_VAR) 

OBJECTS_GLIB_TEST := \
  $(GLIB_TEST_PREFIX)Test_Object.o \
  $(GLIB_TEST_PREFIX)GPPTestObject.o \
  $(GLIB_TEST_PREFIX)gtest_object.o \
  $(GLIB_TEST_PREFIX)TypeHelper.o

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_GLIB := $(OBJECTS_GLIB) $(OBJECTS_GLIB_TEST)
endif

MODULES := $(MODULES) glib

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_GLIB)

glib : $(OBJECTS_GLIB)
	@echo "Built GLib module"

$(GLIB_PREFIX)Object.o : \
    $(GLIB_ROOT)/GLib_Object.cpp
$(GLIB_PREFIX)WeakRef.o : \
    $(GLIB_ROOT)/GLib_WeakRef.cpp
$(GLIB_PREFIX)SignalHandler.o : \
    $(GLIB_ROOT)/GLib_SignalHandler.cpp

$(GLIB_PREFIX)Owned_Object.o : \
    $(GLIB_OWNED_ROOT)/GLib_Owned_Object.cpp

$(GLIB_PREFIX)Borrowed_Object.o : \
    $(GLIB_BORROWED_ROOT)/GLib_Borrowed_Object.cpp
$(GLIB_PREFIX)SharedContainer.o : \
    $(GLIB_BORROWED_ROOT)/GLib_Borrowed_SharedContainer.cpp

$(GLIB_PREFIX)EventLoop.o : \
    $(GLIB_THREAD_ROOT)/GLib_EventLoop.cpp
$(GLIB_PREFIX)ContextCaller.o : \
    $(GLIB_THREAD_ROOT)/GLib_ContextCaller.cpp
$(GLIB_PREFIX)ThreadResource.o : \
    $(GLIB_THREAD_ROOT)/GLib_ThreadResource.cpp
$(GLIB_PREFIX)ThreadHandler.o : \
    $(GLIB_THREAD_ROOT)/GLib_ThreadHandler.cpp

$(GLIB_PREFIX)ObjectPtr.o : \
    $(GLIB_PTR_ROOT)/GLib_ObjectPtr.cpp
$(GLIB_PREFIX)ErrorPtr.o : \
    $(GLIB_PTR_ROOT)/GLib_ErrorPtr.cpp
$(GLIB_PREFIX)SharedContextPtr.o : \
    $(GLIB_PTR_ROOT)/GLib_SharedContextPtr.cpp

$(GLIB_PREFIX)DBusThread.o : \
    $(GLIB_DBUS_ROOT)/GLib_DBusThread.cpp
$(GLIB_PREFIX)DBusProxy.o : \
    $(GLIB_DBUS_ROOT)/GLib_DBusProxy.cpp

$(GLIB_PREFIX)VariantConverter.o : \
    $(GLIB_VAR_ROOT)/GLib_VariantConverter.cpp
$(GLIB_PREFIX)TypeHelper.o : \
    $(GLIB_VAR_ROOT)/GLib_TypeHelper.cpp


# Tests
$(GLIB_TEST_PREFIX)Test_Object.o : \
    $(GLIB_TEST_ROOT)/GPPObject.cpp
$(GLIB_TEST_PREFIX)GPPTestObject.o : \
    $(GLIB_TEST_ROOT)/GPPTestObject.cpp
$(GLIB_TEST_PREFIX)gtest_object.o : \
    $(GLIB_TEST_ROOT)/gtest_object.cpp
$(GLIB_TEST_PREFIX)TypeHelper.o : \
    $(GLIB_TEST_ROOT)/TypeHelper.cpp
