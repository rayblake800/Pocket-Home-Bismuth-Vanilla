####################### DesktopEntry Module ####################################
DESKTOP_ENTRY_DIR = Source/Files/DesktopEntry
DESKTOP_ENTRY_TEST_DIR = Tests/Files/DesktopEntry

DESKTOP_ENTRY_PREFIX = DesktopEntry_
DESKTOP_ENTRY_OBJ := $(JUCE_OBJDIR)/$(DESKTOP_ENTRY_PREFIX)

OBJECTS_DESKTOP_ENTRY := \
  $(DESKTOP_ENTRY_OBJ)FileUtils.o \
  $(DESKTOP_ENTRY_OBJ)EntryFile.o \
  $(DESKTOP_ENTRY_OBJ)LoadingThread.o \
  $(DESKTOP_ENTRY_OBJ)UpdateListener.o \
  $(DESKTOP_ENTRY_OBJ)Loader.o

OBJECTS_DESKTOP_ENTRY_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_DESKTOP_ENTRY := $(OBJECTS_DESKTOP_ENTRY) \
                             $(OBJECTS_DESKTOP_ENTRY_TEST)
endif

FILE_MODULES := $(FILE_MODULES) desktopEntry

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_DESKTOP_ENTRY)

desktopEntry : $(OBJECTS_DESKTOP_ENTRY)
	@echo "    Built DesktopEntry module"

$(DESKTOP_ENTRY_OBJ)FileUtils.o: \
    $(DESKTOP_ENTRY_DIR)/$(DESKTOP_ENTRY_PREFIX)FileUtils.cpp
$(DESKTOP_ENTRY_OBJ)EntryFile.o: \
	$(DESKTOP_ENTRY_DIR)/$(DESKTOP_ENTRY_PREFIX)EntryFile.cpp
$(DESKTOP_ENTRY_OBJ)LoadingThread.o: \
	$(DESKTOP_ENTRY_DIR)/$(DESKTOP_ENTRY_PREFIX)LoadingThread.cpp
$(DESKTOP_ENTRY_OBJ)UpdateListener.o : \
	$(DESKTOP_ENTRY_DIR)/$(DESKTOP_ENTRY_PREFIX)UpdateListener.cpp
$(DESKTOP_ENTRY_OBJ)Loader.o: \
	$(DESKTOP_ENTRY_DIR)/$(DESKTOP_ENTRY_PREFIX)Loader.cpp
