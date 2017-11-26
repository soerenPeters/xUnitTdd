#ifndef TestSuite_H
#define TestSuite_H

#include <vector>
#include <memory>

#include "TestCase.h"
#include "TestResult.h"


template <class T>
class TestSuite
{
public:
    void add(std::shared_ptr<TestCase<T> > test)
    {
        tests.push_back(test);
    }

    void run(std::shared_ptr<TestResult> result)
    {
        for (const std::shared_ptr<TestCase<T> > test : tests)
            test->run(result);
    }

private:
    std::vector<std::shared_ptr<TestCase<T> > > tests;
};

#endif
