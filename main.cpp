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
    }

    void testStarted()
    {
        count++;
    }

    std::string summary()
    {
        std::stringstream ss;
        ss << count;
        ss <<  " run, 0 failed";
        return ss.str();
    }

private:
    int count;
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
        ((static_cast<T*>(this))->*name)();
        tearDown();

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
        test = new WasRun(&WasRun::testMethod);
    }

    void tearDown() override
    {
        delete test;
    }

    void testTemplateMethod()
    {
        test->run();
        assert(test->log  == "setUp wasRun tearDown ");
    }

    void testResult()
    {
        TestResult result = test->run();
        assert(result.summary()  == "1 run, 0 failed");
    }

    void testBrokenTest()
    {
        WasRun* test = new WasRun(&WasRun::brokenTestMethod);

        try {
            TestResult result = test->run();
            assert(result.summary()  == "1 run, 1 failed");
        } catch(std::exception){
            std::cout << "TODO later\n";
        }

        delete test;
    }

private:
    WasRun* test;
};

int main()
{
    TestCaseTest(&TestCaseTest::testTemplateMethod).run();
    TestCaseTest(&TestCaseTest::testResult).run();
    TestCaseTest(&TestCaseTest::testBrokenTest).run();

    return 0;
}