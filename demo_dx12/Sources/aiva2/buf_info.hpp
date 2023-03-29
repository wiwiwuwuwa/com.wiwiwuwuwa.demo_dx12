#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/buffer_memory.hpp>
#include <aiva2/implements.hpp>
#include <aiva2/struct.hpp>

namespace aiva2
{
    struct buf_info_t final : public implements_t<buf_info_t, struct_t>
    {
        // ------------------------------------------------

    public:
        auto get_memory() const->buffer_memory_t;

        void set_memory(buffer_memory_t const memory);

    private:
        buffer_memory_t m_memory{};

        // ------------------------------------------------

    public:
        auto get_size() const->size_t;

        void set_size(size_t const size);

    private:
        size_t m_size{};

        // ------------------------------------------------

    public:
        auto get_support_shader_resource() const->bool;

        void set_support_shader_resource(bool const support_shader_resource);

    private:
        bool m_support_shader_resource{};

        // ------------------------------------------------

    public:
        auto get_support_shader_atomics() const->bool;

        void set_support_shader_atomics(bool const support_shader_atomics);

    private:
        bool m_support_shader_atomics{};

        // ------------------------------------------------

    public:
        auto get_support_unordered_access() const->bool;

        void set_support_unordered_access(bool const support_unordered_access);

    private:
        bool m_support_unordered_access{};
        
        // ------------------------------------------------
    };
}
