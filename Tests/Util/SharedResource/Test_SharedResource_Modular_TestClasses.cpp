#include "Test_SharedResource_Modular_TestClasses.h"
    
const juce::Identifier TestResource::resourceKey = "Test_Modular_Resource";

TestModuleA::TestModuleA(TestResource& resource): 
TestModule<juce::String>(resource, defaultValueA) { }

TestModuleB::TestModuleB(TestResource& resource): 
TestModule<double>(resource, defaultValueB) { }

TestModuleC::TestModuleC(TestResource& resource): 
TestModule<int>(resource, defaultValueC) { }

TestResource::TestResource() : SharedResource::Modular::Resource(resourceKey)
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

template<> TestModuleA* 
SharedResource::Modular::Resource::getModule<TestModuleA>()
{
    TestResource* thisResource = static_cast<TestResource*>(this);
    return thisResource->moduleA;
}

template<> TestModuleB* 
SharedResource::Modular::Resource::getModule<TestModuleB>()
{
    TestResource* thisResource = static_cast<TestResource*>(this);
    return thisResource->moduleB;
}


template<> TestModuleC* 
SharedResource::Modular::Resource::getModule<TestModuleC>()
{
    TestResource* thisResource = static_cast<TestResource*>(this);
    return thisResource->moduleC;
}

template<> const TestModuleA* 
SharedResource::Modular::Resource::getModule<const TestModuleA>()
{
    TestResource* thisResource = static_cast<TestResource*>(this);
    return thisResource->moduleA;
}

template<> const TestModuleB* 
SharedResource::Modular::Resource::getModule<const TestModuleB>()
{
    TestResource* thisResource = static_cast<TestResource*>(this);
    return thisResource->moduleB;
}


template<> const TestModuleC* 
SharedResource::Modular::Resource::getModule<const TestModuleC>()
{
    TestResource* thisResource = static_cast<TestResource*>(this);
    return thisResource->moduleC;
}
