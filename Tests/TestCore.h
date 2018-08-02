/**
 * @file TestCore.h
 * 
 * @brief The testing framework used to run all application tests.
 */
#pragma once
namespace TestCore
{
    /**
     * Runs all application tests.
     * 
     * @param verbose  Sets if additional debug output should be printed.
     * 
     * @return  True if all tests pass, false if at least one test failed.
     */
    bool runAllTests(bool verbose);
    
    /**
     * Holds the result of a single test.
     */
    struct TestResult
    {
        bool success;
        juce::String message;
    };
    
    class TestGroup
    {
        
    };
    
};
