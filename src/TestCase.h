#ifndef TestCase_H
#define TestCase_H

#include <exception>
#include <vector>
#include <memory>

#include "TestResult.h"

template <class T>
class TestCase
{
public:
    explicit TestCase(void (T::*testName)())
    {
        this->testName = testName;
    }
    virtual ~TestCase() = default;

    virtual void setUp(){}
    virtual void tearDown(){}

    void run(std::shared_ptr<TestResult> result)
    {
        result->testStarted();
        setUp();
        try {
            ((static_cast<T*>(this))->*testName)();
            tearDown();
        } catch(std::exception)
        {
            result->testFailed();
        }
    }

private:
    void (T::*testName)();
};

#endif