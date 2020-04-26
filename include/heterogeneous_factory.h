#pragma once

#include "private/heterogeneous_factory_impl.h"

namespace pattern
{

/**
 * @brief HGSFactory implements factory design pattern for objects with the same or different constructors.
 * @tparam BaseT base type of all registred elements
 * @tparam KeyT key type to find and create objects
 * @tparam DefaultCreationArgsT default argument types for registred in factory types construction
 * @details HGSFactory can be used as a simple object and via inheritance.
 * Also there is a macro REGISTER_IN_FACTORY_INSTANCE_STATICALLY, for handy types registration in static context
 * if you have e.g. singleton of your factory.
 * @example
 * HGSFactory<Interface, std::string, int> factory;
 * class InterfaceFactory : public HGSFactory<Interface, Hashable, int, double, std::string> {}
 * @sa REGISTER_IN_FACTORY_INSTANCE_STATICALLY
 */
template<typename BaseT, typename KeyT, typename... DefaultCreationArgsT>
class HGSFactory
{
    static_assert(impl_pattern::traits::is_std_hashable_v<KeyT>, "Type 'KeyT' must have std::hash<KeyT> specialization");

private:
    using BasePtrT = std::unique_ptr<BaseT>;
    using ImplFactory = impl_pattern::ImplHGSFactory<BaseT, KeyT, DefaultCreationArgsT...>;

public:

    /**
     * @tparam CreationArgsT argument types passed to an object's ctor
     * @param factoryRegistrationKey key of registred type
     * @param args... parameters passed to an object's ctor
     * @details creates an object by its register name with arguments
     * @returns std::unique_ptr<BasePtrT> if object is successfully constructed, nullptr otherwise
     * @throws std::bad_alloc if fails to construct object
     * @example
     * auto object = factory.create("concreteName", 1, 2);
     */
    template<typename... CreationArgsT>
    [[ nodiscard ]] BasePtrT create(const KeyT& factoryRegistrationKey, CreationArgsT... args)
    {
        return impl.create(factoryRegistrationKey, args...);
    }

    /**
     * @tparam T type to register in factory
     * @tparam RegisterArgsT argument types for registred types construction
     * @details register type in factory.
     * Produces assertion if type with factoryRegistrationKey is already registred
     * @example
     * factory.registerType<ConcreteIface, int>("concreteIface");
     */
    template<class T, typename... RegisterArgsT>
    void registerType() noexcept
    {
        impl.template registerType<T, RegisterArgsT...>();
    }

public:
    HGSFactory() = default;
    virtual ~HGSFactory() = default;
    HGSFactory(const HGSFactory&) = default;
    HGSFactory& operator=(const HGSFactory&) = default;
    HGSFactory(HGSFactory&&) = default;
    HGSFactory& operator=(HGSFactory&&) = default;

private:
    ImplFactory impl;
};

} // namespace pattern
