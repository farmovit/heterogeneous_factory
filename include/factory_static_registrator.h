#pragma once

#include "private/factory_static_registrator_impl.h"

/**
 * @macro registers type in factory statically
 * @tparam FactoryType factory where RegistredType should be registred
 * @tparam RegistredType type should be registred
 * @tparam ... Argument types for RegistredType construction @sa HGSFactory::registerType
 * @requires static member function 'std::string factoryRegistrationName()'
 * @attention MUST BE used only in cpp file in global namespace otherwise no registration will occured
 * @example
 * // Interface.h
 * struct Interface { ... };

 * // InterfaceFactory.h
 * using InterfaceFactory = HGSFactory<Interface>;

 * // ConcreteInterface.h
 * struct Concrete : public Interface {
 *      Concrete(int, int) {}
 *      static std::string factoryRegistrationName() { return "concrete"; }
 * };
 *
 * // ConcreteInterface.cpp
 * REGISTER_IN_FACTORY_STATIC(InterfaceFactory, Concrete, int, int)
 *
 * // logic.cpp
 * auto concrete = InterfaceFactory::create("concrete", 2, 3);
 */

#define REGISTER_IN_FACTORY_STATIC( FactoryType, RegistredType, ... ) \
    IMPL_REGISTER_IN_FACTORY_STATIC( FactoryType, RegistredType, ## __VA_ARGS__)
