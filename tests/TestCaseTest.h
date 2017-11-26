#ifndef TestCaseTest_H
#define TestCaseTest_H

#include <cassert>
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
        assert(passingTest->getLog()  == "setUp wasRun tearDown ");
    }

    void testResult()
    {
        passingTest->run(result);
        assert(result->summary()  == "1 run, 0 failed");
    }

    void testBrokenTest()
    {
        auto failingTest = new TestCaseSpy(&TestCaseSpy::brokenTestMethod);
        failingTest->run(result);
        assert(result->summary()  == "1 run, 1 failed");
        delete failingTest;
    }

    void testSuite()
    {
        auto suite = new TestSuite<TestCaseSpy>();

        suite->add(std::shared_ptr<TestCaseSpy>(new TestCaseSpy(&TestCaseSpy::testMethod)));
        suite->add(std::shared_ptr<TestCaseSpy>(new TestCaseSpy(&TestCaseSpy::brokenTestMethod)));
        suite->run(result);

        assert(result->summary()  == "2 run, 1 failed");
        delete suite;
    }

private:
    TestCaseSpy* passingTest;
    std::shared_ptr<TestResult> result;
};

#endif
