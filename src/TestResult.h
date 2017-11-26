#ifndef TestResult_H
#define TestResult_H

#include <string>
#include <sstream>


class TestResult
{
public:
    TestResult();
    ~TestResult();

    void testStarted();
    void testFailed();

    std::string summary();

private:
    int count;
    int errorCount;
};

#endif
