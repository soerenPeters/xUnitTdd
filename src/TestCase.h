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
    TestCase(void (T::*name)())
    {
        this->name = name;
    }
    virtual ~TestCase(){}

    virtual void setUp(){}
    virtual void tearDown(){}

    void run(std::shared_ptr<TestResult> result)
    {
        result->testStarted();
        setUp();
        try {
            ((static_cast<T*>(this))->*name)();
            tearDown();
        } catch(std::exception)
        {
            result->testFailed();
        }
    }

private:
    void (T::*name)();
};

#endif