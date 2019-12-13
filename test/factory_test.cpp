#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "heterogeneous_factory.h"
#include "factory_static_registrator.h"

using namespace pattern;
using namespace ::testing;

#define TEST_ASSERTIONS 0

struct Interface
{
    virtual ~Interface() = default;
    virtual int getVal() const noexcept = 0;
};

struct Concrete : Interface
{
    Concrete() = default;
    Concrete(int val) : m_Val(val) {}
    static std::string factoryRegistrationName() { return "concrete"; }
    int getVal() const noexcept override { return m_Val; }

private:
    int m_Val{43};
};

struct SecondConcrete : Interface
{
    SecondConcrete(int a, int) : m_Val(a) {}
    static std::string factoryRegistrationName() { return "secondConcrete"; }
    int getVal() const noexcept override { return m_Val; }

private:
    int m_Val{0};
};

struct ThirdConcrete : SecondConcrete
{
    ThirdConcrete() : SecondConcrete(10, 20) {}
    static std::string factoryRegistrationName() { return "thirdConcrete"; }
};

struct SamefactoryRegistrationName : Interface
{
    static std::string factoryRegistrationName() { return "samefactoryRegistrationName"; }
    int getVal() const noexcept override { return 2; }
};

#if TEST_ASSERTIONS
struct SamefactoryRegistrationName2 : Interface
{
    static std::string factoryRegistrationName() { return "samefactoryRegistrationName"; }
    int getVal() const noexcept override { return 3; }
};
#endif

using InterfaceFactory = HGSFactory<Interface, int>;

REGISTER_IN_FACTORY_STATIC(InterfaceFactory, Concrete)
REGISTER_IN_FACTORY_STATIC(InterfaceFactory, SecondConcrete, int, int)
REGISTER_IN_FACTORY_STATIC(InterfaceFactory, ThirdConcrete)
REGISTER_IN_FACTORY_STATIC(InterfaceFactory, SamefactoryRegistrationName)

#if TEST_ASSERTIONS
// Execution fails with assertion in debug mode
REGISTER_IN_FACTORY_STATIC(InterfaceFactory, SamefactoryRegistrationName2)
#endif

class SingletonInterfaceFactory : public HGSFactory<Interface, int, int>
{
public:
    static SingletonInterfaceFactory& instance() {
        static SingletonInterfaceFactory factory;
        return factory;
    }
};

TEST(FactoryTest, StaticRegistrationTest)
{
    auto concrete = InterfaceFactory::create(Concrete::factoryRegistrationName());
    ASSERT_THAT(concrete, NotNull());
    EXPECT_THAT(concrete->getVal(), Eq(43));
    int creationVal = 2;
    concrete = InterfaceFactory::create(Concrete::factoryRegistrationName(), creationVal);
    ASSERT_THAT(concrete, NotNull());
    EXPECT_THAT(concrete->getVal(), Eq(creationVal));
    concrete = InterfaceFactory::create(Concrete::factoryRegistrationName(), 2, 4, 6);
    EXPECT_THAT(concrete, IsNull());
    auto secondConcrete = InterfaceFactory::create(SecondConcrete::factoryRegistrationName());
    EXPECT_THAT(secondConcrete, IsNull());
    secondConcrete = InterfaceFactory::create(SecondConcrete::factoryRegistrationName(), creationVal, 1);
    ASSERT_THAT(secondConcrete, NotNull());
    EXPECT_THAT(secondConcrete->getVal(), Eq(creationVal));
    auto thirdConcrete = InterfaceFactory::create(ThirdConcrete::factoryRegistrationName());
    ASSERT_THAT(thirdConcrete, NotNull());
    EXPECT_THAT(thirdConcrete->getVal(), Eq(10));
}

TEST(FactoryTest, SingletonFactoryTest)
{
    SingletonInterfaceFactory::registerType<Concrete>(Concrete::factoryRegistrationName());

    // We are not protected from double registring of the same type but there is an assertion
    // SingletonInterfaceFactory::registerType<Concrete>(Concrete::factoryRegistrationName());

    SingletonInterfaceFactory::registerType<SecondConcrete, int, int>(SecondConcrete::factoryRegistrationName());
    SingletonInterfaceFactory::registerType<ThirdConcrete>(ThirdConcrete::factoryRegistrationName());
    auto concrete = InterfaceFactory::create(Concrete::factoryRegistrationName(), 0);
    ASSERT_THAT(concrete, NotNull());
    auto secondConcrete = InterfaceFactory::create(SecondConcrete::factoryRegistrationName(), 1, 1);
    ASSERT_THAT(secondConcrete, NotNull());
    auto thirdConcrete = InterfaceFactory::create(ThirdConcrete::factoryRegistrationName());
    ASSERT_THAT(thirdConcrete, NotNull());
}

int main(int argc, char **argv)
{
    InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
