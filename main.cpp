#include <iostream>

class WasRun
{
public:
    WasRun(void (WasRun::*name)())
    {
        this->name = name;
        wasRun = false;
    }
    void run()
    {
        (this->*name)();
    }
    void testMethod()
    {
        wasRun = true;
    }

    void (WasRun::*name)();
    bool wasRun;
};

int main()
{
    WasRun* test = new WasRun(&WasRun::testMethod);
    std::cout << test->wasRun << std::endl;
    test->run();
    std::cout << test->wasRun << std::endl;

    delete test;
    return 0;
}