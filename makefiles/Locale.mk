########################## Locale Module ######################################
LOCALE_PREFIX := $(JUCE_OBJDIR)/Locale_
LOCALE_ROOT = Source/Locale

OBJECTS_LOCALE := \
  $(LOCALE_PREFIX)Time.o \
  $(LOCALE_PREFIX)TextUser.o \
  $(LOCALE_PREFIX)Locale.o 

OBJECTS_LOCALE_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_LOCALE := $(OBJECTS_LOCALE) $(OBJECTS_LOCALE_TEST)
endif

MODULES := $(MODULES) locale
OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_LOCALE)

locale : $(OBJECTS_LOCALE)
	@echo "Built Locale module"

$(LOCALE_PREFIX)Time.o: \
	$(LOCALE_ROOT)/Time.cpp
$(LOCALE_PREFIX)TextUser.o: \
	$(LOCALE_ROOT)/TextUser.cpp
$(LOCALE_PREFIX)Locale.o: \
	$(LOCALE_ROOT)/Locale.cpp
