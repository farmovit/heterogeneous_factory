#pragma once

#include "traits.h"
#include <memory>
#include <map>
#include <functional>
#include <any>
#include <cassert>

namespace pattern
{
template<typename TForward, typename UForward, typename... ArgsForward>
class HGSFactory;
} // namespace pattern

namespace impl_pattern
{

template<typename TForward, typename UForward, typename... ArgsForward>
class StaticHGSFactory;

template<typename BaseT, typename KeyT, typename... DefaultCreationArgsT>
class ImplHGSFactory final
{
    static_assert(traits::is_std_hashable_v<KeyT>, "Type 'KeyT' must have std::hash<KeyT> specialization");

protected:
    using BasePtrT = std::unique_ptr<BaseT>;
    using DefaultCreatorTraitT = std::function<BasePtrT(DefaultCreationArgsT...)>;

private:
    ImplHGSFactory() = default;

    template<typename... Args>
    BasePtrT create(const KeyT& factoryRegistrationKey, Args... args) noexcept
    {
        using CreatorTraitT = std::function<BasePtrT(Args...)>;
        try
        {
            auto range = mTraitsMap.equal_range(factoryRegistrationKey);
            for (auto it = range.first; it != range.second; ++it)
            {
                try
                {
                    auto creator = std::any_cast<CreatorTraitT>(it->second);
                    return creator(std::forward<Args>(args)...);
                }
                catch (const std::bad_any_cast&) {}
            }
        } catch (const std::exception&) {}
        return nullptr;
    }

    template<class RegistredT, typename... Args>
    void registerType() noexcept
    {
        static_assert(!std::is_abstract_v<RegistredT>, "Type 'RegistredT' must not be abstract");
        static_assert(std::is_base_of_v<BaseT, RegistredT>, "Type 'RegistredT' must inherit from 'BaseT'");
        static_assert(std::is_constructible_v<RegistredT, Args...>
                      || std::is_constructible_v<RegistredT, DefaultCreationArgsT...>,
                      "Type 'RegistredT' must be constructible from 'Args...' or from 'DefaultCreationArgsT...'");
        static_assert(traits::has_static_member_function_factoryRegistrationKey_v<RegistredT, KeyT>,
                      "Type 'RegistredT' must have 'static KeyT factoryRegistrationKey()' member function");
        using CreatorTraitT = std::function<BasePtrT(Args...)>;
        try
        {
            const auto registrationKey = RegistredT::factoryRegistrationKey();
            if (auto it = mTraitsMap.find(registrationKey); it != mTraitsMap.end())
            {
                assert(((void)"You have the same registration key for several types "
                              "or you are trying to register the same type twice", false));
                return;
            }
            if constexpr (std::is_constructible_v<RegistredT, DefaultCreationArgsT...>)
            {
                DefaultCreatorTraitT defaultTrait = [](DefaultCreationArgsT... args)
                {
                    return std::make_unique<RegistredT>(args...);
                };
                mTraitsMap.emplace(registrationKey, defaultTrait);
            }
            if constexpr (!std::is_same_v<CreatorTraitT, DefaultCreatorTraitT>
                          && std::is_constructible_v<RegistredT, Args...>)
            {
                CreatorTraitT trait = [](Args... args)
                {
                    return std::make_unique<RegistredT>(args...);
                };
                mTraitsMap.emplace(registrationKey, trait);
            }
        } catch (const std::exception&) {}
    }

    ImplHGSFactory(const ImplHGSFactory&) = delete;
    ImplHGSFactory& operator=(const ImplHGSFactory&) = delete;
    ImplHGSFactory(ImplHGSFactory&&) = delete;
    ImplHGSFactory& operator=(ImplHGSFactory&&) = delete;

private:
    using TraitsMap = std::unordered_multimap<KeyT, std::any>;
    TraitsMap mTraitsMap;

private:
    template<typename TForward, typename KeyTForward, typename... ArgsForward>
    friend class pattern::HGSFactory;
    template<typename TForward, typename KeyTForward, typename... ArgsForward>
    friend class StaticHGSFactoryImpl;
};
} // namespace impl_pattern
