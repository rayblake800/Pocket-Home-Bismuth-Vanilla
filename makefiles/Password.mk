############################# Password Module ##################################
PASSWORD_PREFIX := $(JUCE_OBJDIR)/Password_
PASSWORD_ROOT = Source/Password

OBJECTS_PASSWORD := \
  $(PASSWORD_PREFIX)Password.o \
  $(PASSWORD_PREFIX)RemovePage.o \
  $(PASSWORD_PREFIX)SetPage.o \
  $(PASSWORD_PREFIX)LoginPage.o 

OBJECTS_PASSWORD_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_PASSWORD := $(OBJECTS_PASSWORD) $(OBJECTS_PASSWORD_TEST)
endif

MODULES := $(MODULES) password

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_PASSWORD)

password : $(OBJECTS_PASSWORD)
	@echo "Built Password module"

$(PASSWORD_PREFIX)Password.o : \
    $(PASSWORD_ROOT)/Password.cpp
$(PASSWORD_PREFIX)RemovePage.o : \
    $(PASSWORD_ROOT)/PasswordPages/RemovePasswordPage.cpp
$(PASSWORD_PREFIX)SetPage.o : \
    $(PASSWORD_ROOT)/PasswordPages/SetPasswordPage.cpp
$(PASSWORD_PREFIX)LoginPage.o : \
    $(PASSWORD_ROOT)/LoginPage/LoginPage.cpp
