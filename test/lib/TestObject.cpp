#include "TestObject.h"
#include "TestInterfaceFactory.h"
#include "factory_static_registrator.h"

REGISTER_IN_FACTORY_INSTANCE_STATICALLY(TestInterfaceFactory::instance(), TestObject, int, int);

TestObject::TestObject(int a, int b)
    : mA(a), mB(b)
{
}

std::string TestObject::factoryRegistrationKey()
{
    return "TestObject";
}
