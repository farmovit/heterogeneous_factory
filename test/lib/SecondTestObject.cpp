#include "SecondTestObject.h"

SecondTestObject::SecondTestObject(int c)
    : mC(c)
{
}

std::string SecondTestObject::factoryRegistrationKey()
{
    return "SecondTestObject";
}
