#include <iostream>


template <class T>
class TestCase
{
public:
    TestCase(void (T::*name)())
    {
        this->name = name;
    }

    void run()
    {
        ((static_cast<T*>(this))->*name)();
    }

    void (T::*name)();
};

class WasRun
        : public TestCase<WasRun>
{
public:
    WasRun(void (WasRun::*name)()) : TestCase(name)
    {
        wasRun = false;
    }

    void testMethod()
    {
        wasRun = true;
    }

    bool wasRun;
};

class TestCaseTest
        : public TestCase<TestCaseTest>
{
public:
    TestCaseTest(void (TestCaseTest::*name)()) : TestCase(name) {}

    void testRunning()
    {
        WasRun* test = new WasRun(&WasRun::testMethod);

        std::cout << test->wasRun << std::endl;
        test->run();
        std::cout << test->wasRun << std::endl;

        delete test;
    }
};

int main()
{
    TestCaseTest(&TestCaseTest::testRunning).run();

    return 0;
}