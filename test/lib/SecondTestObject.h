#pragma once

#include "TestInterface.h"
#include <string>

struct SecondTestObject: TestInterface
{
    SecondTestObject(int c);
    int mC;

    static std::string factoryRegistrationKey();
};

