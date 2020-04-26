#pragma once

#include <type_traits>
#include <functional>
#include <memory>

namespace impl_pattern
{
namespace traits
{

template <typename T, typename = std::void_t<>>
struct is_std_hashable : std::false_type {};

template <typename T>
struct is_std_hashable<T, std::void_t<
        decltype(
            std::declval<std::hash<T>>()(std::declval<T>())
        )
>> : std::true_type {};

template <typename T>
constexpr bool is_std_hashable_v = is_std_hashable<T>::value;

template<typename T, typename ReturnT, typename = std::void_t<>>
struct has_static_member_function_factoryRegistrationKey : std::false_type {};

template<typename T, typename ReturnT>
struct has_static_member_function_factoryRegistrationKey<T, ReturnT, std::void_t<
    std::enable_if_t<
        std::is_same_v<
            ReturnT,
            decltype(T::factoryRegistrationKey())
        >
    >
>> : std::true_type {};

template<typename T, typename ReturnT>
constexpr bool has_static_member_function_factoryRegistrationKey_v =
    has_static_member_function_factoryRegistrationKey<T, ReturnT>::value;

template <template <typename...> typename BaseT, typename DerivedT>
struct is_base_of_template
{
    using NCVDerivedT = typename std::remove_cv_t<DerivedT>;

    template <typename... Args>
    static std::true_type test(BaseT<Args...>*);

    static std::false_type test(std::void_t<>*);

    using type = decltype(test(std::declval<NCVDerivedT*>()));
};

template <template <typename...> class Base, typename Derived>
constexpr bool is_base_of_template_v = is_base_of_template<Base, Derived>::type::value;

template <typename T, typename = std::void_t<>>
struct is_smart_pointer : std::false_type {};

template <typename T>
struct is_smart_pointer<T, std::void_t<
    std::enable_if_t<
        std::is_same_v<
            typename std::remove_cv_t<T>, std::shared_ptr<typename T::element_type>
        >
        ||
        std::is_same_v<
            typename std::remove_cv_t<T>, std::unique_ptr<typename T::element_type>
        >
    >
>> : std::true_type {};

template <typename T>
constexpr bool is_smart_pointer_v = is_smart_pointer<T>::value;

} // namespace traits
} // namespace impl_pattern
