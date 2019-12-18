#pragma once

#include "traits.h"

namespace pattern
{
template<typename TForward, typename UForward, typename... ArgsForward>
class HGSFactory;
} // namespace pattern

namespace impl_pattern
{
template<typename T>
class StaticTypeRegistrator {};
} // namespace impl_pattern

#define IMPL_REGISTER_IN_FACTORY_STATIC( FactoryType, RegistredType, ... ) \
template<> \
class impl_pattern::StaticTypeRegistrator<RegistredType> { \
static_assert(traits::is_base_of_template_v<pattern::HGSFactory, FactoryType>, \
    "Type 'FactoryType' must inherit from pattern::HGSFactory"); \
private: \
    StaticTypeRegistrator() noexcept { \
        FactoryType::registerType<RegistredType, ## __VA_ARGS__>(); \
    } \
    static impl_pattern::StaticTypeRegistrator<RegistredType> instance; \
}; \
impl_pattern::StaticTypeRegistrator<RegistredType> \
    impl_pattern::StaticTypeRegistrator<RegistredType>::instance
