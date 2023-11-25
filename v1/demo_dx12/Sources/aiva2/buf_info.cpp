#include <pch.h>
#include <aiva2/buf_info.hpp>

namespace aiva2
{
    auto buf_info_t::get_memory() const->buffer_memory_t
    {
        return m_memory;
    }

    void buf_info_t::set_memory(buffer_memory_t const memory)
    {
        m_memory = memory;
    }

    auto buf_info_t::get_size() const->size_t
    {
        return m_size;
    }

    void buf_info_t::set_size(size_t const size)
    {
        m_size = size;
    }

    auto buf_info_t::get_support_shader_resource() const->bool
    {
        return m_support_shader_resource;
    }

    void buf_info_t::set_support_shader_resource(bool const support_shader_resource)
    {
        m_support_shader_resource = support_shader_resource;
    }

    auto buf_info_t::get_support_shader_atomics() const->bool
    {
        return m_support_shader_atomics;
    }

    void buf_info_t::set_support_shader_atomics(bool const support_shader_atomics)
    {
        m_support_shader_atomics = support_shader_atomics;
    }

    auto buf_info_t::get_support_unordered_access() const->bool
    {
        return m_support_unordered_access;
    }

    void buf_info_t::set_support_unordered_access(bool const support_unordered_access)
    {
        m_support_unordered_access = support_unordered_access;
    }
}
