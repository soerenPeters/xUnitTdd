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
        wasSetUp = false;
    }

    void setUp()
    {
        wasRun = false;
        wasSetUp = true;
    }

    void testMethod()
    {
        wasRun = true;
    }

    bool wasRun;
    bool wasSetUp;
};

class TestCaseTest
        : public TestCase<TestCaseTest>
{
public:
    TestCaseTest(void (TestCaseTest::*name)()) : TestCase(name) {}

    void setUp()
    {
        test = new WasRun(&WasRun::testMethod);
    }

    void testRunning()
    {
        test->run();
        assert(test->wasRun);

        delete test;
    }

    void testSetUp()
    {
        test->run();
        assert(test->wasSetUp);

        delete test;
    }

    WasRun* test;
};

int main()
{
    TestCaseTest(&TestCaseTest::testRunning).run();
    TestCaseTest(&TestCaseTest::testSetUp).run();

    return 0;
}