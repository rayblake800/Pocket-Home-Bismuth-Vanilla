############################# Password Module ##################################
PASSWORD_DIR = Source/GUI/Password

PASSWORD_PREFIX = Password_
PASSWORD_OBJ := $(JUCE_OBJDIR)/$(PASSWORD_PREFIX)

OBJECTS_PASSWORD := \
  $(PASSWORD_OBJ)Password.o \
  $(PASSWORD_OBJ)UpdateController.o \
  $(PASSWORD_OBJ)RemovalController.o \
  $(PASSWORD_OBJ)LoginScreen.o

OBJECTS_PASSWORD_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_PASSWORD := $(OBJECTS_PASSWORD) $(OBJECTS_PASSWORD_TEST)
endif

GUI_MODULES := $(GUI_MODULES) password

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_PASSWORD)

password : $(OBJECTS_PASSWORD)
	@echo "    Built Password module"

$(PASSWORD_OBJ)Password.o : \
    $(PASSWORD_DIR)/Password.cpp
$(PASSWORD_OBJ)UpdateController.o : \
    $(PASSWORD_DIR)/$(PASSWORD_PREFIX)UpdateController.cpp
$(PASSWORD_OBJ)RemovalController.o : \
    $(PASSWORD_DIR)/$(PASSWORD_PREFIX)RemovalController.cpp
$(PASSWORD_OBJ)LoginScreen.o : \
    $(PASSWORD_DIR)/$(PASSWORD_PREFIX)LoginScreen.cpp
