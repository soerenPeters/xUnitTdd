#ifndef WasRun_H
#define WasRun_H

#include <string>

#include "../src/TestCase.h"

class TestCaseSpy
        : public TestCase<TestCaseSpy>
{
public:
    explicit TestCaseSpy(void (TestCaseSpy::*name)()) : TestCase(name)
    {

    }

    void setUp() override
    {
        log += "setUp ";
    }

    void testMethod()
    {
        log += "wasRun ";
    }

    void brokenTestMethod()
    {
        throw std::exception();
    }

    void tearDown() override
    {
        log += "tearDown ";
    }

    std::string getLog()
    {
        return log;
    }
private:
    std::string log;
};

#endif
