############################# Password Module ##################################
PASSWORD_ROOT = Source/Password
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

MODULES := $(MODULES) password

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_PASSWORD)

password : $(OBJECTS_PASSWORD)
	@echo "Built Password module"

$(PASSWORD_OBJ)Password.o : \
    $(PASSWORD_ROOT)/Password.cpp
$(PASSWORD_OBJ)UpdateController.o : \
    $(PASSWORD_ROOT)/$(PASSWORD_PREFIX)UpdateController.cpp
$(PASSWORD_OBJ)RemovalController.o : \
    $(PASSWORD_ROOT)/$(PASSWORD_PREFIX)RemovalController.cpp
$(PASSWORD_OBJ)LoginScreen.o : \
    $(PASSWORD_ROOT)/$(PASSWORD_PREFIX)LoginScreen.cpp
