############################# GLib Module ######################################
GLIB_PREFIX := $(JUCE_OBJDIR)/GLib_
GLIB_ROOT = Source/GLib
GLIB_TEST_ROOT = Tests/GLib

OBJECTS_GLIB := \
  $(GLIB_PREFIX)Object.o \
  $(GLIB_PREFIX)WeakRef.o \
  $(GLIB_PREFIX)Thread.o \
  $(GLIB_PREFIX)SignalThread.o \
  $(GLIB_PREFIX)SignalHandler.o \
  $(GLIB_PREFIX)VariantConverter.o \
  $(GLIB_PREFIX)DBusProxy.o  

OBJECTS_GLIB_TEST := \
  $(GLIB_PREFIX)Test_Object.o \
  $(GLIB_PREFIX)GPPTestObject.o \
  $(GLIB_PREFIX)gtest_object.o

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_GLIB := $(OBJECTS_GLIB) $(OBJECTS_GLIB_TEST)
endif

MODULES := $(MODULES) glib

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_GLIB)

glib : $(OBJECTS_GLIB)
	@echo "Built GLib module"

$(GLIB_PREFIX)Object.o : \
    $(GLIB_ROOT)/GPPObject.cpp
$(GLIB_PREFIX)WeakRef.o : \
    $(GLIB_ROOT)/GPPWeakRef.cpp
$(GLIB_PREFIX)Thread.o : \
    $(GLIB_ROOT)/GLibThread.cpp
$(GLIB_PREFIX)SignalThread.o : \
    $(GLIB_ROOT)/GLibSignalThread.cpp
$(GLIB_PREFIX)SignalHandler.o : \
    $(GLIB_ROOT)/GSignalHandler.cpp
$(GLIB_PREFIX)VariantConverter.o : \
    $(GLIB_ROOT)/DBus/GVariantConverter.cpp
$(GLIB_PREFIX)DBusProxy.o : \
    $(GLIB_ROOT)/DBus/GPPDBusProxy.cpp

# Tests
$(GLIB_PREFIX)Test_Object.o : \
    $(GLIB_TEST_ROOT)/GPPObject.cpp
$(GLIB_PREFIX)GPPTestObject.o : \
    $(GLIB_TEST_ROOT)/GPPTestObject.cpp
$(GLIB_PREFIX)gtest_object.o : \
    $(GLIB_TEST_ROOT)/gtest_object.cpp
