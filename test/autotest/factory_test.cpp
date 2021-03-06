#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "heterogeneous_factory.h"
#include "heterogeneous_static_factory.h"
#include "factory_static_registrator.h"
#include "TestInterfaceFactory.h"
#include "TestObject.h"
#include "SecondTestObject.h"

using namespace pattern;
using namespace ::testing;

#define TEST_ASSERTIONS 0

struct InterfaceString
{
    virtual ~InterfaceString() = default;
    virtual int getVal() const noexcept = 0;
};

struct ConcreteString : InterfaceString
{
    ConcreteString() = default;
    ConcreteString(int val) : m_Val(val) {}
    static std::string factoryRegistrationKey() { return "concrete"; }
    int getVal() const noexcept override { return m_Val; }

private:
    int m_Val{43};
};

struct SecondConcreteString : InterfaceString
{
    SecondConcreteString(int a, int) : m_Val(a) {}
    static std::string factoryRegistrationKey() { return "secondConcrete"; }
    int getVal() const noexcept override { return m_Val; }

private:
    int m_Val{0};
};

struct ThirdConcreteString : SecondConcreteString
{
    ThirdConcreteString() : SecondConcreteString(10, 20) {}
    static std::string factoryRegistrationKey() { return "thirdConcrete"; }
};

struct SamefactoryRegistrationNameString : InterfaceString
{
    static std::string factoryRegistrationKey() { return "samefactoryRegistrationName"; }
    int getVal() const noexcept override { return 2; }
};

#if TEST_ASSERTIONS
struct SamefactoryRegistrationNameString2 : InterfaceString
{
    static std::string factoryRegistrationName() { return "samefactoryRegistrationName"; }
    int getVal() const noexcept override { return 3; }
};
#endif

using InterfaceStringFactory = StaticHGSFactory<InterfaceString, std::string, int>;

REGISTER_IN_STATIC_FACTORY(InterfaceStringFactory, ConcreteString);
REGISTER_IN_STATIC_FACTORY(InterfaceStringFactory, SecondConcreteString, int, int);
REGISTER_IN_STATIC_FACTORY(InterfaceStringFactory, ThirdConcreteString);
REGISTER_IN_STATIC_FACTORY(InterfaceStringFactory, SamefactoryRegistrationNameString);
#if TEST_ASSERTIONS
// Execution fails with assertion in debug mode
REGISTER_IN_FACTORY_STATIC(InterfaceFactory, SamefactoryRegistrationNameString2)
#endif

TEST(FactoryTest, StaticRegistrationTest)
{
    auto concrete = InterfaceStringFactory::create(ConcreteString::factoryRegistrationKey());
    ASSERT_THAT(concrete, NotNull());
    EXPECT_THAT(concrete->getVal(), Eq(43));
    int creationVal = 2;
    concrete = InterfaceStringFactory::create(ConcreteString::factoryRegistrationKey(), creationVal);
    ASSERT_THAT(concrete, NotNull());
    EXPECT_THAT(concrete->getVal(), Eq(creationVal));
    concrete = InterfaceStringFactory::create(ConcreteString::factoryRegistrationKey(), 2, 4, 6);
    EXPECT_THAT(concrete, IsNull());
    auto secondConcrete = InterfaceStringFactory::create(SecondConcreteString::factoryRegistrationKey());
    EXPECT_THAT(secondConcrete, IsNull());
    secondConcrete = InterfaceStringFactory::create(SecondConcreteString::factoryRegistrationKey(), creationVal, 1);
    ASSERT_THAT(secondConcrete, NotNull());
    EXPECT_THAT(secondConcrete->getVal(), Eq(creationVal));
    auto thirdConcrete = InterfaceStringFactory::create(ThirdConcreteString::factoryRegistrationKey());
    ASSERT_THAT(thirdConcrete, NotNull());
    EXPECT_THAT(thirdConcrete->getVal(), Eq(10));
}

struct InterfaceInt
{
    virtual ~InterfaceInt() = default;
    virtual int getVal() const noexcept = 0;
};

struct ConcreteInt : InterfaceInt
{
    ConcreteInt() = default;
    ConcreteInt(int val) : m_Val(val) {}
    static int factoryRegistrationKey() { return 0; }
    int getVal() const noexcept override { return m_Val; }

private:
    int m_Val{43};
};

struct SecondConcreteInt : InterfaceInt
{
    SecondConcreteInt(int a, int) : m_Val(a) {}
    int getVal() const noexcept override { return m_Val; }

private:
    int m_Val{0};
};

class InterfaceIntInternalRegistredFactory : public HGSFactory<InterfaceInt, int>
{
public:
    void registerTypes() noexcept
    {
        registerType<ConcreteInt, int>();

        // We are not protected from double registration of the same type in compile time but there is an assertion
        // registerType<Concrete, int>(Concrete::factoryRegistrationName());

        registerType<SecondConcreteInt, int, int>(1);
    }
};

TEST(FactoryTest, InternalRegistrationTest)
{
    InterfaceIntInternalRegistredFactory factory;
    factory.registerTypes();
    auto concrete = factory.create(ConcreteInt::factoryRegistrationKey(), 20);
    ASSERT_THAT(concrete, NotNull());
    auto secondConcrete = factory.create(1, 1, 1);
    ASSERT_THAT(secondConcrete, NotNull());
}

TEST(FactoryTest, SharedLibraryRegistrationTest)
{
    auto testObject = TestInterfaceFactory::instance().create(TestObject::factoryRegistrationKey(), 20, 30);
    EXPECT_THAT(testObject, NotNull());
    auto secondTestObject = TestInterfaceFactory::instance().create(SecondTestObject::factoryRegistrationKey(), 40);
    EXPECT_THAT(secondTestObject, NotNull());
}

int main(int argc, char **argv)
{
    InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
