###################### SharedResource Module ################################### 
SHARED_RESOURCE_PREFIX := $(JUCE_OBJDIR)/SharedResource_
SHARED_RESOURCE_ROOT = Source/Util/SharedResource
SHARED_RESOURCE_THREAD_ROOT = $(SHARED_RESOURCE_ROOT)/Thread
SHARED_RESOURCE_IMPL_ROOT = $(SHARED_RESOURCE_ROOT)/Implementation
SHARED_RESOURCE_TEST_ROOT = Tests/Util/SharedResource

OBJECTS_SHARED_RESOURCE_IMPL := \
  $(SHARED_RESOURCE_PREFIX)Holder.o \
  $(SHARED_RESOURCE_PREFIX)ReferenceInterface.o \
  $(SHARED_RESOURCE_PREFIX)Instance.o \
  $(SHARED_RESOURCE_PREFIX)Reference.o \
  $(SHARED_RESOURCE_PREFIX)LockedInstancePtr.o \

SHARED_RESOURCE_THREAD_PREFIX := $(SHARED_RESOURCE_PREFIX)Thread_
OBJECTS_SHARED_RESOURCE_THREAD := \
  $(SHARED_RESOURCE_THREAD_PREFIX)Lock.o \
  $(SHARED_RESOURCE_THREAD_PREFIX)ScopedReadLock.o \
  $(SHARED_RESOURCE_THREAD_PREFIX)ScopedWriteLock.o \
  $(SHARED_RESOURCE_THREAD_PREFIX)Thread.o \
  $(SHARED_RESOURCE_THREAD_PREFIX)Resource.o

OBJECTS_SHARED_RESOURCE := \
  $(OBJECTS_SHARED_RESOURCE_IMPL) \
  $(OBJECTS_SHARED_RESOURCE_THREAD) \
  $(SHARED_RESOURCE_PREFIX)Resource.o

OBJECTS_SHARED_RESOURCE_TEST := \
  $(SHARED_RESOURCE_PREFIX)Test_SharedResource_ModuleTest.o \
  $(SHARED_RESOURCE_PREFIX)Test_SharedResource_Modular_TestClasses.o

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_SHARED_RESOURCE := $(OBJECTS_SHARED_RESOURCE) \
                               $(OBJECTS_SHARED_RESOURCE_TEST)
endif

MODULES := $(MODULES) sharedResource

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_SHARED_RESOURCE)

sharedResource : $(OBJECTS_SHARED_RESOURCE)
	@echo "Built SharedResource module"

$(SHARED_RESOURCE_PREFIX)Holder.o : \
    $(SHARED_RESOURCE_IMPL_ROOT)/SharedResource_Holder.cpp
$(SHARED_RESOURCE_PREFIX)ReferenceInterface.o : \
    $(SHARED_RESOURCE_IMPL_ROOT)/SharedResource_ReferenceInterface.cpp
$(SHARED_RESOURCE_PREFIX)Instance.o : \
    $(SHARED_RESOURCE_IMPL_ROOT)/SharedResource_Instance.cpp
$(SHARED_RESOURCE_PREFIX)Reference.o : \
    $(SHARED_RESOURCE_IMPL_ROOT)/SharedResource_Reference.cpp
$(SHARED_RESOURCE_PREFIX)LockedInstancePtr.o : \
    $(SHARED_RESOURCE_IMPL_ROOT)/SharedResource_LockedInstancePtr.cpp
$(SHARED_RESOURCE_PREFIX)Resource.o : \
    $(SHARED_RESOURCE_ROOT)/SharedResource_Resource.cpp

$(SHARED_RESOURCE_THREAD_PREFIX)Lock.o : \
    $(SHARED_RESOURCE_THREAD_ROOT)/SharedResource_Thread_Lock.cpp
$(SHARED_RESOURCE_THREAD_PREFIX)ScopedReadLock.o : \
    $(SHARED_RESOURCE_THREAD_ROOT)/SharedResource_Thread_ScopedReadLock.cpp
$(SHARED_RESOURCE_THREAD_PREFIX)ScopedWriteLock.o : \
    $(SHARED_RESOURCE_THREAD_ROOT)/SharedResource_Thread_ScopedWriteLock.cpp
$(SHARED_RESOURCE_THREAD_PREFIX)Thread.o : \
    $(SHARED_RESOURCE_THREAD_ROOT)/SharedResource_Thread_Thread.cpp
$(SHARED_RESOURCE_THREAD_PREFIX)Resource.o : \
    $(SHARED_RESOURCE_THREAD_ROOT)/SharedResource_Thread_Resource.cpp

$(SHARED_RESOURCE_PREFIX)Test_SharedResource_ModuleTest.o : \
    $(SHARED_RESOURCE_TEST_ROOT)/Test_SharedResource_ModuleTest.cpp
$(SHARED_RESOURCE_PREFIX)Test_SharedResource_Modular_TestClasses.o : \
    $(SHARED_RESOURCE_TEST_ROOT)/Test_SharedResource_Modular_TestClasses.cpp

