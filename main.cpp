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
        wasRun = false;
        wasSetUp = false;
    }

    void setUp()
    {
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

    void testRunning()
    {
        WasRun* test = new WasRun(&WasRun::testMethod);

        assert(!test->wasRun);
        test->run();
        assert(test->wasRun);

        delete test;
    }

    void testSetUp()
    {
        WasRun* test = new WasRun(&WasRun::testMethod);
        
        test->run();
        assert(test->wasSetUp);

        delete test;
    }
};

int main()
{
    TestCaseTest(&TestCaseTest::testRunning).run();
    TestCaseTest(&TestCaseTest::testSetUp).run();

    return 0;
}