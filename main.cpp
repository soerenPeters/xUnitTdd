#include <iostream>
#include <string>
#include <sstream>
#include <assert.h>
#include <exception>

class TestResult
{
public:
    TestResult()
    {
        count = 0;
        errorCount = 0;
    }

    void testStarted()
    {
        count++;
    }

    void testFailed()
    {
        errorCount++;
    }

    std::string summary()
    {
        std::stringstream ss;
        ss << count;
        ss <<  " run, " << errorCount << " failed";
        return ss.str();
    }

private:
    int count;
    int errorCount;
};

template <class T>
class TestCase
{
public:
    TestCase(void (T::*name)())
    {
        this->name = name;
    }

    virtual void setUp(){}
    virtual void tearDown(){}

    TestResult run()
    {
        TestResult result = TestResult();
        result.testStarted();
        setUp();
        try {
            ((static_cast<T*>(this))->*name)();
            tearDown();
        } catch(std::exception)
        {
            result.testFailed();
        }
        return result;
    }

private:
    void (T::*name)();
};

class WasRun
        : public TestCase<WasRun>
{
public:
    WasRun(void (WasRun::*name)()) : TestCase(name)
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

    std::string log;
};

class TestCaseTest
        : public TestCase<TestCaseTest>
{
public:
    TestCaseTest(void (TestCaseTest::*name)()) : TestCase(name) {}

    void setUp() override
    {
        passingTest = new WasRun(&WasRun::testMethod);
    }

    void tearDown() override
    {
        delete passingTest;
    }

    void testTemplateMethod()
    {
        passingTest->run();
        assert(passingTest->log  == "setUp wasRun tearDown ");
    }

    void testResult()
    {
        TestResult result = passingTest->run();
        assert(result.summary()  == "1 run, 0 failed");
    }

    void testBrokenTest()
    {
        WasRun* failingTest = new WasRun(&WasRun::brokenTestMethod);
        TestResult result = failingTest->run();
        assert(result.summary()  == "1 run, 1 failed");
        delete failingTest;
    }

private:
    WasRun* passingTest;
};

int main()
{
    std::cout << TestCaseTest(&TestCaseTest::testTemplateMethod).run().summary() << std::endl;
    std::cout << TestCaseTest(&TestCaseTest::testResult).run().summary() << std::endl;
    std::cout << TestCaseTest(&TestCaseTest::testBrokenTest).run().summary() << std::endl;

    return 0;
}