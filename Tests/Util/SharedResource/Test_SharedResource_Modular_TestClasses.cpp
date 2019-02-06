#include "Test_SharedResource_Modular_TestClasses.h"
    
const juce::Identifier TestResource::resourceKey = "Test_Modular_Resource";

TestModuleA::TestModuleA(TestResource& resource): 
TestModule<juce::String>(resource, defaultValueA) { }

TestModuleB::TestModuleB(TestResource& resource): 
TestModule<double>(resource, defaultValueB) { }

TestModuleC::TestModuleC(TestResource& resource): 
TestModule<int>(resource, defaultValueC) { }

juce::String TestModuleC::getAValue() const
{
    const TestModuleA* moduleA = getConstSiblingModule<TestModuleA>();
    return moduleA->getValue();
}

double TestModuleC::getBValue() const
{
    const TestModuleB* moduleB = getConstSiblingModule<TestModuleB>();
    return moduleB->getValue();
}

int TestModuleC::actOnAllHandlers()
{
    int count = actOnHandlers();
    count += getSiblingModule<TestModuleA>()->actOnHandlers();
    count += getSiblingModule<TestModuleB>()->actOnHandlers();
    return count;
}

juce::String TestHandlerC::getAValue() const
{
    SharedResource::Modular::LockedPtr<TestResource, const TestModuleC> testC
            = getReadLockedResource();
    return testC->getAValue();
}

double TestHandlerC::getBValue() const
{
    SharedResource::Modular::LockedPtr<TestResource, const TestModuleC> testC
            = getReadLockedResource();
    return testC->getBValue();
}

int TestHandlerC::actOnAllHandlers()
{
    SharedResource::Modular::LockedPtr<TestResource, TestModuleC> testC
            = getWriteLockedResource();
    return testC->actOnAllHandlers();
}

TestResource::TestResource() : SharedResource::Modular::Resource<>(resourceKey)
{
    moduleA = new TestModuleA(*this);
    moduleB = new TestModuleB(*this);
    moduleC = new TestModuleC(*this);
}

TestResource::~TestResource()
{
    delete moduleA;
    delete moduleB;
    delete moduleC;
}

namespace SharedResource { namespace Modular {

template<> template<> TestModuleA* 
SharedResource::Modular::Resource<>::getModule<TestModuleA>()
{
    TestResource* thisResource = static_cast<TestResource*>(this);
    return thisResource->moduleA;
}

template<> template<> TestModuleB* 
SharedResource::Modular::Resource<>::getModule<TestModuleB>()
{
    TestResource* thisResource = static_cast<TestResource*>(this);
    return thisResource->moduleB;
}


template<> template<> TestModuleC* 
SharedResource::Modular::Resource<>::getModule<TestModuleC>()
{
    TestResource* thisResource = static_cast<TestResource*>(this);
    return thisResource->moduleC;
}

template<> template<> const TestModuleA* 
SharedResource::Modular::Resource<>::getModule<const TestModuleA>()
{
    TestResource* thisResource = static_cast<TestResource*>(this);
    return thisResource->moduleA;
}

template<> template<> const TestModuleB* 
SharedResource::Modular::Resource<>::getModule<const TestModuleB>()
{
    TestResource* thisResource = static_cast<TestResource*>(this);
    return thisResource->moduleB;
}

template<> template<> const TestModuleC* 
SharedResource::Modular::Resource<>::getModule<const TestModuleC>()
{
    TestResource* thisResource = static_cast<TestResource*>(this);
    return thisResource->moduleC;
}

} }
