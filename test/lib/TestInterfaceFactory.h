#pragma once

#include "heterogeneous_factory.h"
#include "TestInterface.h"

class TestInterfaceFactory : public pattern::HGSFactory<TestInterface, std::string>
{
public:
    static TestInterfaceFactory& instance();

private:
    TestInterfaceFactory();
};
