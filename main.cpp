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

    void run()
    {
        setUp();
        ((static_cast<T*>(this))->*name)();
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

    std::string log;
};

class TestCaseTest
        : public TestCase<TestCaseTest>
{
public:
    TestCaseTest(void (TestCaseTest::*name)()) : TestCase(name) {}

    void setUp()
    {

    }

    void testTemplateMethod()
    {
        test = new WasRun(&WasRun::testMethod);
        test->run();
        assert(test->log  == "setUp wasRun ");

        delete test;
    }

    WasRun* test;
};

int main()
{
    TestCaseTest(&TestCaseTest::testTemplateMethod).run();

    return 0;
}