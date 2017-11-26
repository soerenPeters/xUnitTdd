#include <iostream>
#include <string>

class WasRun
{
public:
    WasRun(std::string name)
    {
        wasRun = false;
    }
    void run()
    {
        this->testMethod();
    }
    void testMethod()
    {
        wasRun = true;
    }

    bool wasRun;
};

int main()
{
    WasRun* test = new WasRun("testMethod");
    std::cout << test->wasRun << std::endl;
    test->run();
    std::cout << test->wasRun << std::endl;

    delete test;
    return 0;
}