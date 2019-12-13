#pragma once

#include <type_traits>
#include <string>

namespace impl_pattern
{
template<typename T, typename = void>
struct has_static_member_function_factoryRegistrationName : std::false_type {};

template<typename T>
struct has_static_member_function_factoryRegistrationName<T, std::void_t<
    std::enable_if_t<
        std::is_same_v<
            std::string,
            decltype(T::factoryRegistrationName())
        >
    >
>> : std::true_type {};

template<typename T>
constexpr bool has_static_member_function_factoryRegistrationName_v =
    has_static_member_function_factoryRegistrationName<T>::value;

template<typename T>
class StaticTypeRegistrator {};
} // namespace impl_pattern

#define IMPL_REGISTER_IN_FACTORY_STATIC( FactoryType, RegistredType, ... ) \
template<> class impl_pattern::StaticTypeRegistrator<RegistredType> { \
    static_assert(impl_pattern::has_static_member_function_factoryRegistrationName_v<RegistredType>, \
        "Registred type must have 'static std::string factoryRegistrationName()' class member"); \
private: \
    StaticTypeRegistrator() noexcept { \
        try { \
            FactoryType::registerType<RegistredType, ## __VA_ARGS__>(RegistredType::factoryRegistrationName()); \
        } \
        catch (const std::exception&) {} \
    } \
    static impl_pattern::StaticTypeRegistrator<RegistredType> instance; \
}; \
impl_pattern::StaticTypeRegistrator<RegistredType> impl_pattern::StaticTypeRegistrator<RegistredType>::instance;
