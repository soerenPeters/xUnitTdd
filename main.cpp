#include <iostream>
#include <assert.h>

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

    void run()
    {
        setUp();
        ((static_cast<T*>(this))->*name)();
        tearDown();
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

    void setUp()
    {
        log += "setUp ";
    }

    void testMethod()
    {
        log += "wasRun ";
    }

    void tearDown()
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

    void testTemplateMethod()
    {
        test = new WasRun(&WasRun::testMethod);
        test->run();
        assert(test->log  == "setUp wasRun tearDown ");

        delete test;
    }

    WasRun* test;
};

int main()
{
    TestCaseTest(&TestCaseTest::testTemplateMethod).run();

    return 0;
}