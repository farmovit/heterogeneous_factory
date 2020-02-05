#pragma once

#include "TestInterface.h"
#include <string>

struct TestObject: TestInterface
{
    TestObject(int a, int b);
    int mA;
    int mB;

    static std::string factoryRegistrationKey();
};
