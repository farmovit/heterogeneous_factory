#pragma once

#include "private/heterogeneous_factory_impl.h"

namespace pattern
{

/**
 * @brief HGSFactory implements factory design pattern for objects with the same or different ctors
 * @tparam BaseT base type of all registred elements
 * @tparam DefaultCreationArgsT default argument types for registred in factory types construction
 * @details HGSFactory can be used both as an independent object via 'using' or inheritance
 * and via static registrator provided by macro REGISTER_IN_FACTORY_STATIC.
 * @example
 * using InterfaceFactory = HGSFactory<Interface, int>;
 * class InterfaceFactory : public HGSFactory<Interface, int> {}
 * @sa REGISTER_IN_FACTORY_STATIC
 */
template<typename BaseT, typename... DefaultCreationArgsT>
class HGSFactory
{
    using BasePtrT = std::unique_ptr<BaseT>;
    using ImplFactory = impl_pattern::ImplHGSFactory<BaseT, DefaultCreationArgsT...>;

public:

    /**
     * @tparam CreationArgsT argument types passed to an object's ctor
     * @param factoryRegistrationName name of registred type
     * @param args... parameters passed to an object's ctor
     * @details creates an object by it's register name with arguments
     * @returns std::unique_ptr<BasePtrT> if object successfully constructed, nullptr otherwise
     * @throws std::bad_alloc if failed to construct object
     * @example
     * auto object = InterfaceFactory::create("concreteIface", 1, 2);
     */
    template<typename... CreationArgsT>
    static BasePtrT create(std::string_view factoryRegistrationName, CreationArgsT... args)
    {
        return ImplFactory::create(factoryRegistrationName, args...);
    }

    /**
     * @tparam T type to register in factory
     * @tparam RegisterArgsT argument types for registred types construction
     * @param factoryRegistrationName name of registred type
     * @details register type in factory
     * @produces assertion if type with factoryRegistrationName is already registred
     * @example
     * InterfaceFactory::registerType<ConcreteIface, int>("concreteIface");
     */
    template<class T, typename... RegisterArgsT>
    static void registerType(std::string_view factoryRegistrationName) noexcept
    {
        ImplFactory::template registerType<T, RegisterArgsT...>(factoryRegistrationName);
    }

    HGSFactory() = default;
    virtual ~HGSFactory() = default;
    HGSFactory(const HGSFactory&) = delete;
    HGSFactory& operator=(const HGSFactory&) = delete;
    HGSFactory(HGSFactory&&) = delete;
    HGSFactory& operator=(HGSFactory&&) = delete;
};

} // namespace pattern
