#pragma once

#include "private/heterogeneous_static_factory_impl.h"

namespace pattern
{

/**
 * @brief StaticHGSFactory implements static factory design pattern for objects with the same or different constructors.
 * @tparam BaseT base type of all registred elements
 * @tparam KeyT key type to find and create objects
 * @tparam DefaultCreationArgsT default argument types for registred in factory types construction
 * @details StaticHGSFactory can be used both as an independent object via 'using' or inheritance
 * and via static registrator provided by macro REGISTER_IN_STATIC_FACTORY.
 * This version of factory is very similar to the HGSFactory, but it makes all operations in static context.
 * Use it
 * 1) WHEN you don't have separate library where your factory is supposed to be located,
 * 2) WHEN there is no risk to meet the static initialization order fiasco,
 * 3) FOR handy automatic static types registration via REGISTER_IN_STATIC_FACTORY macro without adding any extra code.
 * @example
 * using InterfaceFactory = StaticHGSFactory<Interface, std::string, int>;
 * class InterfaceFactory : public StaticHGSFactory<Interface, Hashable, int, double, std::string> {}
 * @sa REGISTER_IN_STATIC_FACTORY
 */
template<typename BaseT, typename KeyT, typename... DefaultCreationArgsT>
class StaticHGSFactory
{
    static_assert(impl_pattern::traits::is_std_hashable_v<KeyT>, "Type 'KeyT' must have std::hash<KeyT> specialization");

    using BasePtrT = std::unique_ptr<BaseT>;
    using ImplStaticFactory = impl_pattern::StaticHGSFactoryImpl<BaseT, KeyT, DefaultCreationArgsT...>;

public:
    /**
     * @tparam CreationArgsT argument types passed to an object's ctor
     * @param factoryRegistrationKey key of registred type
     * @param args... parameters passed to an object's ctor
     * @details creates an object by its register name with arguments
     * @returns std::unique_ptr<BasePtrT> if object is successfully constructed, nullptr otherwise
     * @throws std::bad_alloc if fails to construct object
     * @example
     * auto object = InterfaceFactory::create("concreteName", 1, 2);
     */
    template<typename... CreationArgsT>
    static BasePtrT create(const KeyT& factoryRegistrationKey, CreationArgsT... args)
    {
        return ImplStaticFactory::create(factoryRegistrationKey, args...);
    }

    /**
     * @tparam T type to register in factory
     * @tparam RegisterArgsT argument types for registred types construction
     * @details register type in factory.
     * Produces assertion if type with factoryRegistrationKey is already registred
     * @example
     * InterfaceFactory::registerType<ConcreteIface, int>("concreteIface");
     */
    template<class T, typename... RegisterArgsT>
    static void registerType() noexcept
    {
        ImplStaticFactory::template registerType<T, RegisterArgsT...>();
    }

    StaticHGSFactory() = delete;
    virtual ~StaticHGSFactory() = default;
    StaticHGSFactory(const StaticHGSFactory&) = delete;
    StaticHGSFactory& operator=(const StaticHGSFactory&) = delete;
    StaticHGSFactory(StaticHGSFactory&&) = delete;
    StaticHGSFactory& operator=(StaticHGSFactory&&) = delete;

public:
    using KeyType = KeyT;
};

} // namespace pattern
