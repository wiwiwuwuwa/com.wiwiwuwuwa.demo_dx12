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
