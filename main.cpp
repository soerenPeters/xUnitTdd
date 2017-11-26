#include <iostream>
#include <assert.h>

class TestResult
{
public:
    std::string summary()
    {
        return "";
    }
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
        setUp();
        ((static_cast<T*>(this))->*name)();
        tearDown();

        return TestResult();
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

private:
    WasRun* test;
};

int main()
{
    TestCaseTest(&TestCaseTest::testTemplateMethod).run();
    TestCaseTest(&TestCaseTest::testResult).run();

    return 0;
}