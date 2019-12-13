#pragma once

#include <memory>
#include <map>
#include <functional>
#include <any>
#include <cassert>

namespace pattern
{
template<typename T, typename... Args>
class HGSFactory;
} // namespace pattern

namespace impl_pattern
{

template<typename BaseT, typename... DefaultCreationArgsT>
class ImplHGSFactory final
{
private:
    using BasePtrT = std::unique_ptr<BaseT>;
    using DefaultCreatorTraitT = std::function<BasePtrT(DefaultCreationArgsT...)>;

    template<typename... Args>
    static BasePtrT create(std::string_view factoryRegistrationName, Args... args)
    {
        using CreatorTraitT = std::function<BasePtrT(Args...)>;
        try
        {
            std::string name{factoryRegistrationName};
            auto range = traitsMap().equal_range(name);
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

    template<class T, typename... Args>
    static void registerType(std::string_view factoryRegistrationName) noexcept
    {
        static_assert(!std::is_abstract_v<T>, "Type must not be abstract");
        static_assert(std::is_base_of_v<BaseT, T>, "Type must be derived from BaseT");
        static_assert(std::is_constructible_v<T, Args...> || std::is_constructible_v<T, DefaultCreationArgsT...>,
                      "Type must be constructible from Args... or from DefaultCreationArgsT...");
        try
        {
            std::string name{factoryRegistrationName};
            if (auto iter = traitsMap().find(name);
                    iter != traitsMap().end())
            {
                assert(((void)"You have the same names of register types or you are trying to register the same type twice!", false));
                return;
            }
            using CreatorTraitT = std::function<BasePtrT(Args...)>;
            if constexpr (std::is_constructible_v<T, DefaultCreationArgsT...>)
            {
                DefaultCreatorTraitT defaultTrait = [](DefaultCreationArgsT... args) { return std::make_unique<T>(args...); };
                traitsMap().emplace(name, defaultTrait);
            }
            if constexpr (!std::is_same_v<CreatorTraitT, DefaultCreatorTraitT> && std::is_constructible_v<T, Args...>)
            {
                CreatorTraitT trait = [](Args... args) { return std::make_unique<T>(args...); };
                traitsMap().emplace(name, trait);
            }
        } catch (const std::exception&) {}
    }

    ImplHGSFactory() = delete;
    ImplHGSFactory(const ImplHGSFactory&) = delete;
    ImplHGSFactory& operator=(const ImplHGSFactory&) = delete;
    ImplHGSFactory(ImplHGSFactory&&) = delete;
    ImplHGSFactory& operator=(ImplHGSFactory&&) = delete;

private:
    using TraitsMap = std::unordered_multimap<std::string, std::any>;
    static TraitsMap &traitsMap()
    {
        static TraitsMap map;
        return map;
    }
    template<typename T, typename... Args>
    friend class pattern::HGSFactory;
};
} // namespace impl_pattern
