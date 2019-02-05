############################# GLib Module ######################################
GLIB_ROOT = Source/GLib
GLIB_PREFIX = GLib_
GLIB_OBJ := $(JUCE_OBJDIR)/$(GLIB_PREFIX)

GLIB_PTR_ROOT := $(GLIB_ROOT)/SmartPointers
OBJECTS_GLIB_SMART_PTRS := \
  $(GLIB_OBJ)ObjectPtr.o \
  $(GLIB_OBJ)ErrorPtr.o \
  $(GLIB_OBJ)SharedContextPtr.o

GLIB_OWNED_ROOT := $(GLIB_ROOT)/Owned
GLIB_OWNED_PREFIX := $(GLIB_PREFIX)Owned_
GLIB_OWNED_OBJ := $(GLIB_OBJ)Owned_
OBJECTS_GLIB_OWNED := \
  $(GLIB_OWNED_OBJ)Object.o 

GLIB_BORROWED_ROOT := $(GLIB_ROOT)/Borrowed
GLIB_BORROWED_PREFIX := $(GLIB_PREFIX)Borrowed_
GLIB_BORROWED_OBJ := $(GLIB_OBJ)Borrowed_
OBJECTS_GLIB_BORROWED := \
  $(GLIB_BORROWED_OBJ)Object.o \
  $(GLIB_BORROWED_OBJ)SharedContainer.o

GLIB_THREAD_ROOT := $(GLIB_ROOT)/Thread
GLIB_THREAD_OBJ := $(GLIB_OBJ)Thread_
OBJECTS_GLIB_THREAD := \
  $(GLIB_THREAD_OBJ)EventLoop.o \
  $(GLIB_THREAD_OBJ)ContextCaller.o \
  $(GLIB_THREAD_OBJ)SharedThread.o \

GLIB_DBUS_ROOT := $(GLIB_ROOT)/DBus
GLIB_DBUS_PREFIX := $(GLIB_PREFIX)DBus_
GLIB_DBUS_OBJ := $(GLIB_OBJ)DBus_
OBJECTS_GLIB_DBUS := \
  $(GLIB_DBUS_OBJ)Proxy.o \
  $(GLIB_DBUS_OBJ)ThreadResource.o 

GLIB_VAR_ROOT := $(GLIB_DBUS_ROOT)/Variant
OBJECTS_GLIB_VAR := \
  $(GLIB_OBJ)VariantConverter.o \
  $(GLIB_OBJ)TypeHelper.o

OBJECTS_GLIB := \
  $(GLIB_OBJ)WeakRef.o \
  $(GLIB_OBJ)Object.o \
  $(OBJECTS_GLIB_OWNED) \
  $(OBJECTS_GLIB_BORROWED) \
  $(OBJECTS_GLIB_THREAD) \
  $(OBJECTS_GLIB_SMART_PTRS) \
  $(OBJECTS_GLIB_DBUS) \
  $(OBJECTS_GLIB_VAR) 

GLIB_TEST_ROOT = Tests/GLib
GLIB_TEST_PREFIX := $(GLIB_PREFIX)Test_
GLIB_TEST_OBJ := $(GLIB_OBJ)Test_

OBJECTS_GLIB_TEST := \
  $(GLIB_TEST_OBJ)OwnedObjectTest.o \
  $(GLIB_TEST_OBJ)OwnedObject.o \
  $(GLIB_TEST_OBJ)gtest_object.o \
  $(GLIB_TEST_OBJ)TypeHelper.o

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_GLIB := $(OBJECTS_GLIB) $(OBJECTS_GLIB_TEST)
endif

MODULES := $(MODULES) glib

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_GLIB)

glib : $(OBJECTS_GLIB)
	@echo "Built GLib module"

$(GLIB_OBJ)Object.o : \
    $(GLIB_ROOT)/$(GLIB_PREFIX)Object.cpp
$(GLIB_OBJ)WeakRef.o : \
    $(GLIB_ROOT)/$(GLIB_PREFIX)WeakRef.cpp

$(GLIB_OWNED_OBJ)Object.o : \
    $(GLIB_OWNED_ROOT)/$(GLIB_OWNED_PREFIX)Object.cpp

$(GLIB_BORROWED_OBJ)Object.o : \
    $(GLIB_BORROWED_ROOT)/$(GLIB_BORROWED_PREFIX)Object.cpp
$(GLIB_BORROWED_OBJ)SharedContainer.o : \
    $(GLIB_BORROWED_ROOT)/$(GLIB_BORROWED_PREFIX)SharedContainer.cpp

$(GLIB_THREAD_OBJ)EventLoop.o : \
    $(GLIB_THREAD_ROOT)/$(GLIB_PREFIX)EventLoop.cpp
$(GLIB_THREAD_OBJ)ContextCaller.o : \
    $(GLIB_THREAD_ROOT)/$(GLIB_PREFIX)ContextCaller.cpp
$(GLIB_THREAD_OBJ)SharedThread.o : \
    $(GLIB_THREAD_ROOT)/$(GLIB_PREFIX)SharedThread.cpp

$(GLIB_OBJ)ObjectPtr.o : \
    $(GLIB_PTR_ROOT)/$(GLIB_PREFIX)ObjectPtr.cpp
$(GLIB_OBJ)ErrorPtr.o : \
    $(GLIB_PTR_ROOT)/$(GLIB_PREFIX)ErrorPtr.cpp
$(GLIB_OBJ)SharedContextPtr.o : \
    $(GLIB_PTR_ROOT)/$(GLIB_PREFIX)SharedContextPtr.cpp

$(GLIB_DBUS_OBJ)ThreadResource.o : \
    $(GLIB_DBUS_ROOT)/$(GLIB_DBUS_PREFIX)ThreadResource.cpp
$(GLIB_DBUS_OBJ)Proxy.o : \
    $(GLIB_DBUS_ROOT)/$(GLIB_DBUS_PREFIX)Proxy.cpp

$(GLIB_OBJ)VariantConverter.o : \
    $(GLIB_VAR_ROOT)/$(GLIB_PREFIX)VariantConverter.cpp
$(GLIB_OBJ)TypeHelper.o : \
    $(GLIB_VAR_ROOT)/$(GLIB_PREFIX)TypeHelper.cpp

# Tests
$(GLIB_TEST_OBJ)OwnedObject.o : \
    $(GLIB_TEST_ROOT)/$(GLIB_TEST_PREFIX)OwnedObject.cpp
$(GLIB_TEST_OBJ)OwnedObjectTest.o : \
    $(GLIB_TEST_ROOT)/$(GLIB_TEST_PREFIX)OwnedObjectTest.cpp
$(GLIB_TEST_OBJ)gtest_object.o : \
    $(GLIB_TEST_ROOT)/gtest_object.cpp
$(GLIB_TEST_OBJ)TypeHelper.o : \
    $(GLIB_TEST_ROOT)/TypeHelper.cpp
