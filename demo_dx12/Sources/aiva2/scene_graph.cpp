#include <pch.h>
#include <aiva2/scene_graph.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/scene_actor.hpp>

namespace aiva2
{
    scene_graph_t::scene_graph_t(engine_t& engine)
        : impl_type{ engine }
    {

    }

    scene_graph_t::~scene_graph_t()
    {

    }

    auto scene_graph_t::add_actor_ptr() -> std::shared_ptr<scene_actor_t>
    {
        auto const actor = new scene_actor_t{ *this };
        assert_t::check_bool(actor, "(actor) is not valid");

        return m_actors.emplace_back(actor);
    }

    auto scene_graph_t::add_actor_ref() -> scene_actor_t&
    {
        auto const actor = add_actor_ptr();
        assert_t::check_bool(actor, "(actor) is not valid");

        return (*actor);
    }

    auto scene_graph_t::get_actor_ptr(size_t const index) -> std::shared_ptr<scene_actor_t>
    {
        assert_t::check_bool(index >= decltype(index){}, "(index) is not valid");
        assert_t::check_bool(index < std::size(m_actors), "(index) is not valid");

        return m_actors[index];
    }

    auto scene_graph_t::get_actor_ref(size_t const index) -> scene_actor_t&
    {
        auto const actor = get_actor_ptr(index);
        assert_t::check_bool(actor, "(actor) is not valid");

        return (*actor);
    }

    auto scene_graph_t::num_actor() const -> size_t
    {
        return std::size(m_actors);
    }

    auto scene_graph_t::rem_actor(size_t const index) -> void
    {
        assert_t::check_bool(index >= decltype(index){}, "(index) is not valid");
        assert_t::check_bool(index < std::size(m_actors), "(index) is not valid");

        m_actors.erase(std::next(std::begin(m_actors), index));
    }
}
