#pragma once

#include "private/heterogeneous_factory_impl.h"

namespace pattern
{

/**
 * @brief HGSFactory implements factory design pattern for objects with the same or different ctors
 * @tparam BaseT base type of all registred elements
 * @tparam KeyT key type to find and create objects
 * @tparam DefaultCreationArgsT default argument types for registred in factory types construction
 * @details HGSFactory can be used both as an independent object via 'using' or inheritance
 * and via static registrator provided by macro REGISTER_IN_FACTORY_STATIC.
 * @example
 * using InterfaceFactory = HGSFactory<Interface, std::string, int>;
 * class InterfaceFactory : public HGSFactory<Interface, Hashable, int, double, std::string> {}
 * @sa REGISTER_IN_FACTORY_STATIC
 */
template<typename BaseT, typename KeyT, typename... DefaultCreationArgsT>
class HGSFactory
{
    static_assert(impl_pattern::traits::is_std_hashable_v<KeyT>, "Type 'KeyT' must have std::hash<KeyT> specialization");

    using BasePtrT = std::unique_ptr<BaseT>;
    using ImplFactory = impl_pattern::ImplHGSFactory<BaseT, KeyT, DefaultCreationArgsT...>;

public:
    /**
     * @tparam CreationArgsT argument types passed to an object's ctor
     * @param factoryRegistrationKey key of registred type
     * @param args... parameters passed to an object's ctor
     * @details creates an object by its register name with arguments
     * @returns std::unique_ptr<BasePtrT> if object successfully constructed, nullptr otherwise
     * @throws std::bad_alloc if fails to construct object
     * @example
     * auto object = InterfaceFactory::create("concreteName", 1, 2);
     */
    template<typename... CreationArgsT>
    static BasePtrT create(const KeyT& factoryRegistrationKey, CreationArgsT... args)
    {
        return ImplFactory::create(factoryRegistrationKey, args...);
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
        ImplFactory::template registerType<T, RegisterArgsT...>();
    }

    HGSFactory() = default;
    virtual ~HGSFactory() = default;
    HGSFactory(const HGSFactory&) = delete;
    HGSFactory& operator=(const HGSFactory&) = delete;
    HGSFactory(HGSFactory&&) = delete;
    HGSFactory& operator=(HGSFactory&&) = delete;

public:
    using KeyType = KeyT;
};

} // namespace pattern
