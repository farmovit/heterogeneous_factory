#pragma once

#include "private/factory_static_registrator_impl.h"

/**
 * @macro registers type in static factory
 * @tparam FactoryType factory where RegistredType should be registred
 * @tparam RegistredType type that should be registred
 * @tparam ... Argument types for RegistredType construction @sa StaticHGSFactory::registerType
 * @requirements static member function 'KeyT factoryRegistrationKey()'
 * @attention MUST BE used only in cpp file in global namespace otherwise no registration will be occured
 * @brief use it if the reasons of usage StaticHGSFactory is acceptable for you.
 * @example
 * // Interface.h
 * struct Interface { ... };

 * // InterfaceFactory.h
 * using InterfaceFactory = StaticHGSFactory<Interface, std::string>;

 * // ConcreteInterface.h
 * struct Concrete : public Interface {
 *      Concrete(int, int) {}
 *      static std::string factoryRegistrationKey() { return "concrete"; }
 * };
 *
 * // ConcreteInterface.cpp
 * REGISTER_IN_STATIC_FACTORY(InterfaceFactory, Concrete, int, int)
 *
 * // logic.cpp
 * auto concrete = InterfaceFactory::create("concrete", 2, 3);
 */

#define REGISTER_IN_STATIC_FACTORY( FactoryType, RegistredType, ... ) \
    IMPL_REGISTER_IN_STATIC_FACTORY( FactoryType, RegistredType, ## __VA_ARGS__)

/**
 * @macro registers type in factory instance statically
 * @tparam FactoryInstance static factory instance where RegistredType should be registred
 * @tparam RegistredType type that should be registred
 * @tparam ... Argument types for RegistredType construction @sa HGSFactory::registerType
 * @requirements static member function 'KeyT factoryRegistrationKey()'
 * @attention MUST BE used only in cpp file in global namespace otherwise no registration will be occured
 * @brief use it
 * 1) WHEN there is no risk to meet the static initialization order fiasco,
 * 2) IF you have static instance of your factory, e.g. singleton
 * @example
 * // Interface.h
 * struct Interface { ... };

 * // InterfaceFactory.h
 * class InterfaceFactory : public HGSFactory<Interface, std::string>
 * {
 * public:
 *     static InterfaceFactory& instance()
 *     {
 *         static InterfaceFactory factory;
 *         return factory;
 *     }
 * };
 *
 * // ConcreteInterface.h
 * struct Concrete : public Interface {
 *      Concrete(int, int) {}
 *      static std::string factoryRegistrationKey() { return "concrete"; }
 * };
 *
 * // ConcreteInterface.cpp
 * REGISTER_IN_FACTORY_INSTANCE_STATICALLY(InterfaceFactory::instance(), Concrete, int, int)
 *
 * // logic.cpp
 * auto concrete = InterfaceFactory::instance().create("concrete", 2, 3);
 */

#define REGISTER_IN_FACTORY_INSTANCE_STATICALLY( FactoryInstance, RegistredType, ... ) \
    IMPL_REGISTER_IN_FACTORY_INSTANCE_STATICALLY( FactoryInstance, RegistredType, ## __VA_ARGS__)
