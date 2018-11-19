####################### DesktopEntry Module ####################################
DESKTOP_ENTRY_PREFIX := $(JUCE_OBJDIR)/DesktopEntry_
DESKTOP_ENTRY_ROOT = Source/DesktopEntry
DESKTOP_ENTRY_TEST_ROOT = Tests/DesktopEntry

OBJECTS_DESKTOP_ENTRY := \
  $(DESKTOP_ENTRY_PREFIX)FileUtils.o \
  $(DESKTOP_ENTRY_PREFIX)EntryFile.o \
  $(DESKTOP_ENTRY_PREFIX)LoadingThread.o \
  $(DESKTOP_ENTRY_PREFIX)UpdateListener.o \
  $(DESKTOP_ENTRY_PREFIX)Loader.o

OBJECTS_DESKTOP_ENTRY_TEST := \
  $(DESKTOP_ENTRY_PREFIX)Test_FileUtils.o \
  $(DESKTOP_ENTRY_PREFIX)Test_EntryFile.o \
  $(DESKTOP_ENTRY_PREFIX)Test_LoadingThread.o \
  $(DESKTOP_ENTRY_PREFIX)Test_Loader.o

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_DESKTOP_ENTRY := $(OBJECTS_DESKTOP_ENTRY) \
                             $(OBJECTS_DESKTOP_ENTRY_TEST)
endif

MODULES := $(MODULES) desktopEntry

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_DESKTOP_ENTRY)

desktopEntry : $(OBJECTS_DESKTOP_ENTRY)
	@echo "Built DesktopEntry module"

$(DESKTOP_ENTRY_PREFIX)FileUtils.o: \
    $(DESKTOP_ENTRY_ROOT)/FileUtils.cpp
$(DESKTOP_ENTRY_PREFIX)EntryFile.o: \
	$(DESKTOP_ENTRY_ROOT)/EntryFile.cpp
$(DESKTOP_ENTRY_PREFIX)LoadingThread.o: \
	$(DESKTOP_ENTRY_ROOT)/LoadingThread.cpp
$(DESKTOP_ENTRY_PREFIX)UpdateListener.o : \
	$(DESKTOP_ENTRY_ROOT)/UpdateListener.cpp
$(DESKTOP_ENTRY_PREFIX)Loader.o: \
	$(DESKTOP_ENTRY_ROOT)/Loader.cpp

# Tests:
$(DESKTOP_ENTRY_PREFIX)Test_FileUtils.o: \
    $(DESKTOP_ENTRY_TEST_ROOT)/FileUtils.cpp
$(DESKTOP_ENTRY_PREFIX)Test_EntryFile.o: \
	$(DESKTOP_ENTRY_TEST_ROOT)/EntryFile.cpp
$(DESKTOP_ENTRY_PREFIX)Test_LoadingThread.o: \
	$(DESKTOP_ENTRY_TEST_ROOT)/LoadingThread.cpp
$(DESKTOP_ENTRY_PREFIX)Test_Loader.o: \
	$(DESKTOP_ENTRY_TEST_ROOT)/Loader.cpp
