#pragma once

#include "traits.h"

namespace pattern
{
template<typename TForward, typename UForward, typename... ArgsForward>
class StaticHGSFactory;

template<typename TForward, typename UForward, typename... ArgsForward>
class HGSFactory;
} // namespace pattern

namespace impl_pattern
{
template<typename T>
class StaticTypeRegistrator {};
} // namespace impl_pattern

#define IMPL_REGISTER_IN_STATIC_FACTORY( FactoryType, RegistredType, ... ) \
template<> \
class impl_pattern::StaticTypeRegistrator<RegistredType> { \
    static_assert(traits::is_base_of_template_v<pattern::StaticHGSFactory, FactoryType>, \
    "Type 'FactoryType' must inherit from pattern::StaticHGSFactory"); \
private: \
    StaticTypeRegistrator() noexcept { \
        FactoryType::registerType<RegistredType, ## __VA_ARGS__>(); \
    } \
    static impl_pattern::StaticTypeRegistrator<RegistredType> instance; \
}; \
impl_pattern::StaticTypeRegistrator<RegistredType> \
    impl_pattern::StaticTypeRegistrator<RegistredType>::instance

#define IMPL_REGISTER_IN_FACTORY_INSTANCE_STATICALLY( FactoryInstance, RegistredType, ... ) \
template<> \
class impl_pattern::StaticTypeRegistrator<RegistredType> { \
    using FactoryInstanceT = decltype(FactoryInstance); \
    static_assert(traits::is_base_of_template_v<pattern::HGSFactory, std::remove_reference_t<FactoryInstanceT>>, \
    "Type of 'FactoryInstance' must inherit from pattern::HGSFactory"); \
private: \
    StaticTypeRegistrator() noexcept { \
        FactoryInstance.registerType<RegistredType, ## __VA_ARGS__>(); \
    } \
    static impl_pattern::StaticTypeRegistrator<RegistredType> instance; \
}; \
impl_pattern::StaticTypeRegistrator<RegistredType> \
    impl_pattern::StaticTypeRegistrator<RegistredType>::instance
