#include "TestInterfaceFactory.h"
#include "SecondTestObject.h"

TestInterfaceFactory &TestInterfaceFactory::instance()
{
    static TestInterfaceFactory factory;
    return factory;
}

TestInterfaceFactory::TestInterfaceFactory()
{
    registerType<SecondTestObject, int>();
}
