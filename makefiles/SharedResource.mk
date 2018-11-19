###################### SharedResource Module ################################### 
SHARED_RESOURCE_PREFIX := $(JUCE_OBJDIR)/SharedResource_
SHARED_RESOURCE_ROOT = Source/Util/SharedResource
SHARED_RESOURCE_TEST_ROOT = Tests/Util/SharedResource

OBJECTS_SHARED_RESOURCE := \
  $(SHARED_RESOURCE_PREFIX)Holder.o \
  $(SHARED_RESOURCE_PREFIX)Instance.o \
  $(SHARED_RESOURCE_PREFIX)Reference.o \
  $(SHARED_RESOURCE_PREFIX)LockedInstancePtr.o \
  $(SHARED_RESOURCE_PREFIX)Resource.o \
  $(SHARED_RESOURCE_PREFIX)ThreadResource.o

OBJECTS_SHARED_RESOURCE_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_SHARED_RESOURCE := $(OBJECTS_SHARED_RESOURCE) \
                               $(OBJECTS_SHARED_RESOURCE_TEST)
endif

MODULES := $(MODULES) sharedResource

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_SHARED_RESOURCE)

sharedResource : $(OBJECTS_SHARED_RESOURCE)
	@echo "Built SharedResource module"

$(SHARED_RESOURCE_PREFIX)Holder.o : \
    $(SHARED_RESOURCE_ROOT)/Implementation/Holder.cpp
$(SHARED_RESOURCE_PREFIX)Instance.o : \
    $(SHARED_RESOURCE_ROOT)/Implementation/Instance.cpp
$(SHARED_RESOURCE_PREFIX)Reference.o : \
    $(SHARED_RESOURCE_ROOT)/Implementation/Reference.cpp
$(SHARED_RESOURCE_PREFIX)LockedInstancePtr.o : \
    $(SHARED_RESOURCE_ROOT)/Implementation/LockedInstancePtr.cpp
$(SHARED_RESOURCE_PREFIX)Resource.o : \
    $(SHARED_RESOURCE_ROOT)/Resource.cpp
$(SHARED_RESOURCE_PREFIX)ThreadResource.o : \
    $(SHARED_RESOURCE_ROOT)/ThreadResource.cpp

