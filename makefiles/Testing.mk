############################### Testing Module #################################
TEST_PREFIX := $(JUCE_OBJDIR)/Test_
TEST_ROOT = Tests

OBJECTS_TEST := \
  $(TEST_PREFIX)StressTest.o \
  $(TEST_PREFIX)TestWindow.o \
  $(TEST_PREFIX)DelayUtils.o

ifeq ($(BUILD_TESTS), 1)
    MODULES := $(MODULES) testing
    OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_TEST)
endif

testing : $(OBJECTS_TEST)
	@echo "Built Testing module"

$(TEST_PREFIX)StressTest.o: \
	$(TEST_ROOT)/StressTest.cpp
$(TEST_PREFIX)TestWindow.o: \
	$(TEST_ROOT)/TestWindow.cpp
$(TEST_PREFIX)DelayUtils.o: \
	$(TEST_ROOT)/DelayUtils.cpp
