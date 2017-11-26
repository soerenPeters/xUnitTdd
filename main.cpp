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

template <class T>
class TestSuite
{
public:
    void add(TestCase<T>* test)
    {
        tests.push_back(test);
    }

    void run(TestResult *result)
    {
        for (TestCase<T>* test : tests)
            test->run(result);
    }

private:
    std::vector<TestCase<T>*> tests;

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
        TestSuite<WasRun>* suite = new TestSuite<WasRun>();

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
    TestSuite<TestCaseTest> suite;
    suite.add(new TestCaseTest(&TestCaseTest::testTemplateMethod));
    suite.add(new TestCaseTest(&TestCaseTest::testResult));
    suite.add(new TestCaseTest(&TestCaseTest::testBrokenTest));
    suite.add(new TestCaseTest(&TestCaseTest::testSuite));

    TestResult* result = new TestResult();
    suite.run(result);
    std::cout << result->summary() << std::endl;


    return 0;
}