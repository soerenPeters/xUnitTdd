#include "TestResult.h"

TestResult::TestResult()
{
    count = 0;
    errorCount = 0;
}

TestResult::~TestResult()
{

}

void TestResult::testStarted()
{
    count++;
}

void TestResult::testFailed()
{
    errorCount++;
}

std::string TestResult::summary()
{
    std::stringstream ss;
    ss << count << " run, " << errorCount << " failed";
    return ss.str();
}
