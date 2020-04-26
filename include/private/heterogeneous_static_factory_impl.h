#pragma once

#include "heterogeneous_factory_impl.h"

namespace pattern
{
template<typename TForward, typename UForward, typename... ArgsForward>
class StaticHGSFactory;
} // namespace pattern

namespace impl_pattern
{

template<typename BaseT, typename KeyT, typename... DefaultCreationArgsT>
class StaticHGSFactoryImpl final
{
    using BasePtrT = std::unique_ptr<BaseT>;
    using FactoryT = ImplHGSFactory<BaseT, KeyT, DefaultCreationArgsT...>;

private:
    template<typename... CreationArgsT>
    static BasePtrT create(const KeyT& factoryRegistrationKey, CreationArgsT... args) noexcept
    {
        return getFactory().create(factoryRegistrationKey, std::forward<CreationArgsT>(args)...);
    }

    template<class T, typename... RegisterArgsT>
    static void registerType() noexcept
    {
        getFactory().template registerType<T, RegisterArgsT...>();
    }

public:
    StaticHGSFactoryImpl() = delete;
    StaticHGSFactoryImpl(const StaticHGSFactoryImpl&) = delete;
    StaticHGSFactoryImpl& operator=(const StaticHGSFactoryImpl&) = delete;
    StaticHGSFactoryImpl(StaticHGSFactoryImpl&&) = delete;
    StaticHGSFactoryImpl& operator=(StaticHGSFactoryImpl&&) = delete;

private:
    static FactoryT& getFactory()
    {
        static FactoryT factory;
        return factory;
    }

private:
    template<typename TForward, typename KeyTForward, typename... ArgsForward>
    friend class pattern::StaticHGSFactory;
};

} // namespace impl_pattern
