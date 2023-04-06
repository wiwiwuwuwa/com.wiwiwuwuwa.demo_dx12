#pragma once
#include <aiva2/_core.hpp>
#include <aiva2/resource_loader_library.hpp>

#define REGISTER_RESOURCE_LOADER(loader_type) \
    namespace \
    { \
        static struct register_resource_loader_t \
        { \
            template<typename T, typename = void> \
            struct has_load_data : std::false_type {}; \
            \
            template<typename T> \
            struct has_load_data<T, std::void_t<decltype(T::load(std::declval<engine_t&>(), std::declval<boost::span<std::byte const> const&>()))>> : std::true_type {}; \
            \
            template<typename T, typename = void> \
            struct has_load_json : std::false_type {}; \
            \
            template<typename T> \
            struct has_load_json<T, std::void_t<decltype(T::load(std::declval<engine_t&>(), std::declval<nlohmann::json const&>()))>> : std::true_type {}; \
            \
            register_resource_loader_t() \
            { \
                if constexpr (has_load_data<loader_type>::value) \
                { \
                    using function_type = resource_loader_library_t::load_from_data_func; \
                    using resource_type = decltype(loader_type::load(std::declval<engine_t&>(), std::declval<boost::span<std::byte const> const&>()))::element_type; \
                    resource_loader_library_t::register_loader(typeid(resource_type), function_type{ &loader_type::load }); \
                } \
                \
                if constexpr (has_load_json<loader_type>::value) \
                { \
                    using function_type = resource_loader_library_t::load_from_json_func; \
                    using resource_type = decltype(loader_type::load(std::declval<engine_t&>(), std::declval<nlohmann::json const&>()))::element_type; \
                    resource_loader_library_t::register_loader(typeid(resource_type), function_type{ &loader_type::load }); \
                } \
            } \
        } register_resource_loader_instance{}; \
    } \
