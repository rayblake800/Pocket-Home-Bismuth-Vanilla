############################# GLib Module ######################################
GLIB_PREFIX := $(JUCE_OBJDIR)/GLib_
GLIB_TEST_PREFIX := $(GLIB_PREFIX)Test_
GLIB_ROOT = Source/GLib
GLIB_TEST_ROOT = Tests/GLib

OBJECTS_GLIB := \
  $(GLIB_PREFIX)ErrorPtr.o \
  $(GLIB_PREFIX)Object.o \
  $(GLIB_PREFIX)WeakRef.o \
  $(GLIB_PREFIX)ThreadResource.o \
  $(GLIB_PREFIX)ThreadHandler.o \
  $(GLIB_PREFIX)SharedContextPtr.o \
  $(GLIB_PREFIX)EventLoop.o \
  $(GLIB_PREFIX)ContextCaller.o \
  $(GLIB_PREFIX)SignalHandler.o \
  $(GLIB_PREFIX)TypeHelper.o \
  $(GLIB_PREFIX)VariantConverter.o \
  $(GLIB_PREFIX)DBusThread.o \
  $(GLIB_PREFIX)DBusProxy.o  

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

$(GLIB_PREFIX)ErrorPtr.o : \
    $(GLIB_ROOT)/SmartPointers/ErrorPtr.cpp
$(GLIB_PREFIX)Object.o : \
    $(GLIB_ROOT)/Object.cpp
$(GLIB_PREFIX)WeakRef.o : \
    $(GLIB_ROOT)/WeakRef.cpp
$(GLIB_PREFIX)ThreadResource.o : \
    $(GLIB_ROOT)/ThreadResource.cpp
$(GLIB_PREFIX)ThreadHandler.o : \
    $(GLIB_ROOT)/ThreadHandler.cpp
$(GLIB_PREFIX)SharedContextPtr.o : \
    $(GLIB_ROOT)/SmartPointers/SharedContextPtr.cpp
$(GLIB_PREFIX)EventLoop.o : \
    $(GLIB_ROOT)/EventLoop.cpp
$(GLIB_PREFIX)ContextCaller.o : \
    $(GLIB_ROOT)/ContextCaller.cpp
$(GLIB_PREFIX)SignalHandler.o : \
    $(GLIB_ROOT)/SignalHandler.cpp
$(GLIB_PREFIX)TypeHelper.o : \
    $(GLIB_ROOT)/Variant/TypeHelper.cpp
$(GLIB_PREFIX)VariantConverter.o : \
    $(GLIB_ROOT)/DBus/GVariantConverter.cpp
$(GLIB_PREFIX)DBusThread.o : \
    $(GLIB_ROOT)/DBus/DBusThread.cpp
$(GLIB_PREFIX)DBusProxy.o : \
    $(GLIB_ROOT)/DBus/DBusProxy.cpp

# Tests
$(GLIB_TEST_PREFIX)Test_Object.o : \
    $(GLIB_TEST_ROOT)/GPPObject.cpp
$(GLIB_TEST_PREFIX)GPPTestObject.o : \
    $(GLIB_TEST_ROOT)/GPPTestObject.cpp
$(GLIB_TEST_PREFIX)gtest_object.o : \
    $(GLIB_TEST_ROOT)/gtest_object.cpp
$(GLIB_TEST_PREFIX)TypeHelper.o : \
    $(GLIB_TEST_ROOT)/TypeHelper.cpp
