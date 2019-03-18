############################# GLib Module ######################################
GLIB_DIR = Source/Framework/GLib
GLIB_PTR_DIR := $(GLIB_DIR)/SmartPointers
GLIB_OWNED_DIR := $(GLIB_DIR)/Owned
GLIB_BORROWED_DIR := $(GLIB_DIR)/Borrowed
GLIB_THREAD_DIR := $(GLIB_DIR)/Thread
GLIB_DBUS_DIR := $(GLIB_DIR)/DBus
GLIB_VAR_DIR := $(GLIB_DBUS_DIR)/Variant
GLIB_TEST_DIR = Tests/Framework/GLib

GLIB_PREFIX = GLib_
GLIB_OBJ := $(JUCE_OBJDIR)/$(GLIB_PREFIX)

OBJECTS_GLIB_SMART_PTRS := \
  $(GLIB_OBJ)ObjectPtr.o \
  $(GLIB_OBJ)ErrorPtr.o \
  $(GLIB_OBJ)SharedContextPtr.o

GLIB_OWNED_PREFIX := $(GLIB_PREFIX)Owned_
GLIB_OWNED_OBJ := $(GLIB_OBJ)Owned_
OBJECTS_GLIB_OWNED := \
  $(GLIB_OWNED_OBJ)Object.o 

GLIB_BORROWED_PREFIX := $(GLIB_PREFIX)Borrowed_
GLIB_BORROWED_OBJ := $(GLIB_OBJ)Borrowed_
OBJECTS_GLIB_BORROWED := \
  $(GLIB_BORROWED_OBJ)Object.o \
  $(GLIB_BORROWED_OBJ)SharedContainer.o

GLIB_THREAD_OBJ := $(GLIB_OBJ)Thread_
OBJECTS_GLIB_THREAD := \
  $(GLIB_THREAD_OBJ)EventLoop.o \
  $(GLIB_THREAD_OBJ)ContextCaller.o \
  $(GLIB_THREAD_OBJ)SharedThread.o \

GLIB_DBUS_PREFIX := $(GLIB_PREFIX)DBus_
GLIB_DBUS_OBJ := $(GLIB_OBJ)DBus_
OBJECTS_GLIB_DBUS := \
  $(GLIB_DBUS_OBJ)Proxy.o

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

FRAMEWORK_MODULES := $(FRAMEWORK_MODULES) glib

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_GLIB)

glib : $(OBJECTS_GLIB)
	@echo "    Built GLib module"

$(GLIB_OBJ)Object.o : \
    $(GLIB_DIR)/$(GLIB_PREFIX)Object.cpp
$(GLIB_OBJ)WeakRef.o : \
    $(GLIB_DIR)/$(GLIB_PREFIX)WeakRef.cpp

$(GLIB_OWNED_OBJ)Object.o : \
    $(GLIB_OWNED_DIR)/$(GLIB_OWNED_PREFIX)Object.cpp

$(GLIB_BORROWED_OBJ)Object.o : \
    $(GLIB_BORROWED_DIR)/$(GLIB_BORROWED_PREFIX)Object.cpp
$(GLIB_BORROWED_OBJ)SharedContainer.o : \
    $(GLIB_BORROWED_DIR)/$(GLIB_BORROWED_PREFIX)SharedContainer.cpp

$(GLIB_THREAD_OBJ)EventLoop.o : \
    $(GLIB_THREAD_DIR)/$(GLIB_PREFIX)EventLoop.cpp
$(GLIB_THREAD_OBJ)ContextCaller.o : \
    $(GLIB_THREAD_DIR)/$(GLIB_PREFIX)ContextCaller.cpp
$(GLIB_THREAD_OBJ)SharedThread.o : \
    $(GLIB_THREAD_DIR)/$(GLIB_PREFIX)SharedThread.cpp

$(GLIB_OBJ)ObjectPtr.o : \
    $(GLIB_PTR_DIR)/$(GLIB_PREFIX)ObjectPtr.cpp
$(GLIB_OBJ)ErrorPtr.o : \
    $(GLIB_PTR_DIR)/$(GLIB_PREFIX)ErrorPtr.cpp
$(GLIB_OBJ)SharedContextPtr.o : \
    $(GLIB_PTR_DIR)/$(GLIB_PREFIX)SharedContextPtr.cpp

$(GLIB_DBUS_OBJ)Proxy.o : \
    $(GLIB_DBUS_DIR)/$(GLIB_DBUS_PREFIX)Proxy.cpp

$(GLIB_OBJ)VariantConverter.o : \
    $(GLIB_VAR_DIR)/$(GLIB_PREFIX)VariantConverter.cpp
$(GLIB_OBJ)TypeHelper.o : \
    $(GLIB_VAR_DIR)/$(GLIB_PREFIX)TypeHelper.cpp

# Tests
$(GLIB_TEST_OBJ)OwnedObject.o : \
    $(GLIB_TEST_DIR)/$(GLIB_TEST_PREFIX)OwnedObject.cpp
$(GLIB_TEST_OBJ)OwnedObjectTest.o : \
    $(GLIB_TEST_DIR)/$(GLIB_TEST_PREFIX)OwnedObjectTest.cpp
$(GLIB_TEST_OBJ)gtest_object.o : \
    $(GLIB_TEST_DIR)/gtest_object.cpp
$(GLIB_TEST_OBJ)TypeHelper.o : \
    $(GLIB_TEST_DIR)/TypeHelper.cpp
