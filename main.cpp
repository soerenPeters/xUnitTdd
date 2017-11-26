#include <iostream>
#include <string>
#include <sstream>
#include <assert.h>
#include <exception>
#include <vector>

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

    void run(TestResult* result)
    {
        result->testStarted();
        setUp();
        try {
            ((static_cast<T*>(this))->*name)();
            tearDown();
        } catch(std::exception)
        {
            result->testFailed();
        }
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

class TestSuite
{
public:
    void add(WasRun* test)
    {
        tests.push_back(test);
    }

    void run(TestResult *result)
    {
        for (WasRun* test : tests)
            test->run(result);
    }

private:
    std::vector<WasRun*> tests;

};

class TestCaseTest
        : public TestCase<TestCaseTest>
{
public:
    TestCaseTest(void (TestCaseTest::*name)()) : TestCase(name) {}

    void setUp() override
    {
        passingTest = new WasRun(&WasRun::testMethod);
        result = new TestResult();
    }

    void tearDown() override
    {
        delete passingTest;
        delete result;
    }

    void testTemplateMethod()
    {
        passingTest->run(result);
        assert(passingTest->log  == "setUp wasRun tearDown ");
    }

    void testResult()
    {
        passingTest->run(result);
        assert(result->summary()  == "1 run, 0 failed");
    }

    void testBrokenTest()
    {
        WasRun* failingTest = new WasRun(&WasRun::brokenTestMethod);
        failingTest->run(result);
        assert(result->summary()  == "1 run, 1 failed");
        delete failingTest;
    }

    void testSuite()
    {
        TestSuite* suite = new TestSuite();

        suite->add(new WasRun(&WasRun::testMethod));
        suite->add(new WasRun(&WasRun::brokenTestMethod));
        suite->run(result);

        assert(result->summary()  == "2 run, 1 failed");
        delete suite;
    }

private:
    WasRun* passingTest;
    TestResult* result;
};

int main()
{
    TestResult* result = new TestResult();
    TestSuite suite;
    TestCaseTest(&TestCaseTest::testTemplateMethod).run(result);
    TestCaseTest(&TestCaseTest::testResult).run(result);
    TestCaseTest(&TestCaseTest::testBrokenTest).run(result);
    TestCaseTest(&TestCaseTest::testSuite).run(result);

    std::cout << result->summary() << std::endl;


    return 0;
}