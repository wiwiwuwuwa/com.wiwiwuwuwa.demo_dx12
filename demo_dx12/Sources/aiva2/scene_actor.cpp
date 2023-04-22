#include <pch.h>
#include <aiva2/scene_actor.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/scene_component.hpp>
#include <aiva2/scene_graph.hpp>

namespace aiva2
{
    scene_actor_t::scene_actor_t(scene_graph_t& scene_graph)
        : m_graph{ scene_graph }
    {

    }

    scene_actor_t::~scene_actor_t()
    {

    }

    auto scene_actor_t::get_graph() const -> scene_graph_t&
    {
        return m_graph;
    }

    auto scene_actor_t::get_engine() const -> engine_t&
    {
        return get_graph().get_engine();
    }

    auto scene_actor_t::get_parent_ptr() const -> std::shared_ptr<scene_actor_t>
    {
        return m_parent.lock();
    }

    auto scene_actor_t::get_parent_ref() const -> scene_actor_t&
    {
        auto const parent = get_parent_ptr();
        assert_t::check_bool(parent, "(parent) is not valid");

        return (*parent);
    }

    auto scene_actor_t::has_parent() const -> bool
    {
        return get_parent_ptr() != nullptr;
    }

    auto scene_actor_t::set_parent(std::shared_ptr<scene_actor_t> const& parent) -> void
    {
        if (auto const current_parent = get_parent_ptr())
        {
            std::remove(std::begin((*current_parent).m_children), std::end((*current_parent).m_children), shared_from_this());
        }

        m_parent = parent;

        if (auto const desired_parent = get_parent_ptr())
        {
            (*desired_parent).m_children.push_back(shared_from_this());
        }
    }

    auto scene_actor_t::set_parent(scene_actor_t& parent) -> void
    {
        auto const parent_ptr = parent.shared_from_this();
        assert_t::check_bool(parent_ptr, "(parent_ptr) is not valid");

        set_parent(parent_ptr);
    }

    auto scene_actor_t::get_child_ptr(size_t const index) const -> std::shared_ptr<scene_actor_t>
    {
        assert_t::check_bool(index >= decltype(index){}, "(index) is out of range");
        assert_t::check_bool(index < std::size(m_children), "(index) is out of range");

        return m_children[index];
    }

    auto scene_actor_t::get_child_ref(size_t const index) const -> scene_actor_t&
    {
        auto const child = get_child_ptr(index);
        assert_t::check_bool(child, "(child) is not valid");

        return (*child);
    }

    auto scene_actor_t::num_child() const -> size_t
    {
        return std::size(m_children);
    }

    auto scene_actor_t::get_component_ptr(size_t const index) -> std::shared_ptr<scene_component_t>
    {
        assert_t::check_bool(index >= decltype(index){}, "(index) is out of range");
        assert_t::check_bool(index < std::size(m_components), "(index) is out of range");

        return m_components[index];
    }

    auto scene_actor_t::get_component_ref(size_t const index) -> scene_component_t&
    {
        auto const component = get_component_ptr(index);
        assert_t::check_bool(component, "(component) is not valid");

        return (*component);
    }

    auto scene_actor_t::num_component() const -> size_t
    {
        return std::size(m_components);
    }

    auto scene_actor_t::rem_component(size_t const index) -> void
    {
        assert_t::check_bool(index >= decltype(index){}, "(index) is out of range");
        assert_t::check_bool(index < std::size(m_components), "(index) is out of range");

        m_components.erase(std::next(std::begin(m_components), index));
    }
}
