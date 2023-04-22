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

    auto scene_actor_t::get_local_position() const -> glm::vec3
    {
        return m_local_position;
    }

    auto scene_actor_t::get_local_rotation() const -> glm::quat
    {
        return m_local_rotation;
    }

    auto scene_actor_t::get_local_scale() const -> glm::vec3
    {
        return m_local_scale;
    }

    auto scene_actor_t::get_local_transform() const -> glm::mat4
    {
        auto transform = glm::identity<glm::mat4>();

        transform = glm::scale(transform, m_local_scale);
        transform = transform * glm::mat4_cast(m_local_rotation);
        transform = glm::translate(transform, m_local_position);

        return transform;
    }

    auto scene_actor_t::set_local_position(glm::vec3 const& position) -> void
    {
        m_local_position = position;
    }

    auto scene_actor_t::set_local_rotation(glm::quat const& rotation) -> void
    {
        m_local_rotation = rotation;
    }

    auto scene_actor_t::set_local_scale(glm::vec3 const& scale) -> void
    {
        m_local_scale = scale;
    }

    auto scene_actor_t::set_local_transform(glm::mat4 const& transform) -> void
    {
        m_local_position = transform[3];
        m_local_rotation = glm::quat_cast(transform);
        m_local_scale = glm::vec3(glm::length(transform[0]), glm::length(transform[1]), glm::length(transform[2]));
    }

    auto scene_actor_t::get_world_position() const -> glm::vec3
    {
        if (auto const parent = get_parent_ptr())
        {
            auto const local_to_world = (*parent).get_world_transform();
            return glm::vec3(local_to_world * glm::vec4(m_local_position, 1.0f));
        }
        else
        {
            return m_local_position;
        }
    }

    auto scene_actor_t::get_world_rotation() const -> glm::quat
    {
        if (auto const parent = get_parent_ptr())
        {
            auto const local_to_world = (*parent).get_world_rotation();
            return local_to_world * m_local_rotation;
        }
        else
        {
            return m_local_rotation;
        }
    }

    auto scene_actor_t::get_world_scale() const -> glm::vec3
    {
        if (auto const parent = get_parent_ptr())
        {
            auto const local_to_world = (*parent).get_world_scale();
            return local_to_world * m_local_scale;
        }
        else
        {
            return m_local_scale;
        }
    }

    auto scene_actor_t::get_world_transform() const -> glm::mat4
    {
        if (auto const parent = get_parent_ptr())
        {
            auto const local_to_world = (*parent).get_world_transform();
            return local_to_world * get_local_transform();
        }
        else
        {
            return get_local_transform();
        }
    }

    auto scene_actor_t::set_world_position(glm::vec3 const& position) -> void
    {
        if (auto const parent = get_parent_ptr())
        {
            auto const local_to_world = (*parent).get_world_transform();
            auto const world_to_local = glm::inverse(local_to_world);
            m_local_position = world_to_local * glm::vec4(position, 1.0f);
        }
        else
        {
            m_local_position = position;
        }
    }

    auto scene_actor_t::set_world_rotation(glm::quat const& rotation) -> void
    {
        if (auto const parent = get_parent_ptr())
        {
            auto const local_to_world = (*parent).get_world_rotation();
            auto const world_to_local = glm::inverse(local_to_world);
            m_local_rotation = world_to_local * rotation;
        }
        else
        {
            m_local_rotation = rotation;
        }
    }

    auto scene_actor_t::set_world_scale(glm::vec3 const& scale) -> void
    {
        if (auto const parent = get_parent_ptr())
        {
            auto const local_to_world = (*parent).get_world_scale();

            if (glm::all(glm::epsilonNotEqual(local_to_world, glm::zero<glm::vec3>(), glm::epsilon<float>())))
            {
                auto const world_to_local = glm::one<glm::vec3>() / local_to_world;
                m_local_scale = world_to_local * scale;
            }
            else
            {
                m_local_scale = glm::one<glm::vec3>();
            }
        }
        else
        {
            m_local_scale = scale;
        }
    }

    auto scene_actor_t::set_world_transform(glm::mat4 const& transform) -> void
    {
        if (auto const parent = get_parent_ptr())
        {
            auto const local_to_world = (*parent).get_world_transform();
            auto const world_to_local = glm::inverse(local_to_world);
            set_local_transform(world_to_local * transform);
        }
        else
        {
            set_local_transform(transform);
        }
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
