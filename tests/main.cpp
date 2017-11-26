#include <iostream>
#include <memory>

#include "TestCaseTest.h"
#include "../src/TestResult.h"
#include "../src/TestSuite.h"

int main()
{
    TestSuite<TestCaseTest> suite;
    suite.add(std::shared_ptr<TestCaseTest>(new TestCaseTest(&TestCaseTest::testTemplateMethod)));
    suite.add(std::shared_ptr<TestCaseTest>(new TestCaseTest(&TestCaseTest::testResult)));
    suite.add(std::shared_ptr<TestCaseTest>(new TestCaseTest(&TestCaseTest::testBrokenTest)));
    suite.add(std::shared_ptr<TestCaseTest>(new TestCaseTest(&TestCaseTest::testSuite)));

    std::shared_ptr<TestResult> result = std::make_shared<TestResult>();
    suite.run(result);
    std::cout << result->summary() << std::endl;

    return 0;
}
