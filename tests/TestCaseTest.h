#ifndef TestCaseTest_H
#define TestCaseTest_H

#include <memory>

#include "TestCaseSpy.h"
#include "../src/TestCase.h"
#include "../src/TestSuite.h"
#include "../src/TestResult.h"

class TestCaseTest
        : public TestCase<TestCaseTest>
{
public:
    explicit TestCaseTest(void (TestCaseTest::*name)()) : TestCase(name) {}

    void setUp() override
    {
        passingTest = new TestCaseSpy(&TestCaseSpy::testMethod);
        result = std::make_shared<TestResult>();
    }

    void tearDown() override
    {
        delete passingTest;
    }

    void testTemplateMethod()
    {
        passingTest->run(result);
        if(passingTest->getLog()  != "setUp wasRun tearDown ")
            throw std::exception();
    }

    void testResult()
    {
        passingTest->run(result);
        if(result->summary()  != "1 run, 0 failed")
            throw std::exception();
    }

    void testBrokenTest()
    {
        auto failingTest = new TestCaseSpy(&TestCaseSpy::brokenTestMethod);
        failingTest->run(result);
        if(result->summary()  != "1 run, 1 failed")
            throw std::exception();

        delete failingTest;
    }

    void testSuite()
    {
        auto suite = new TestSuite<TestCaseSpy>();

        suite->add(std::shared_ptr<TestCaseSpy>(new TestCaseSpy(&TestCaseSpy::testMethod)));
        suite->add(std::shared_ptr<TestCaseSpy>(new TestCaseSpy(&TestCaseSpy::brokenTestMethod)));
        suite->run(result);

        if(result->summary()  != "2 run, 1 failed")
            throw std::exception();

        delete suite;
    }

private:
    TestCaseSpy* passingTest;
    std::shared_ptr<TestResult> result;
};

#endif
