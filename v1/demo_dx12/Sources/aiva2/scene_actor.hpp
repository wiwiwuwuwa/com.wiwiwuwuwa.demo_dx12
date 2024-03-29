#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/implements.hpp>
#include <aiva2/object.hpp>

namespace aiva2
{
    struct scene_actor_t final : public implements_t<scene_actor_t, object_t>, public std::enable_shared_from_this<scene_actor_t>
    {
        // ------------------------------------------------

    public:
        friend scene_graph_t;

    protected:
        scene_actor_t(scene_graph_t& scene_graph);

    public:
        ~scene_actor_t() override;

        // ------------------------------------------------

    public:
        auto get_graph() const -> scene_graph_t&;

        auto get_engine() const -> engine_t&;

    private:
        scene_graph_t& m_graph;

        // ------------------------------------------------

    public:
        auto get_parent_ptr() const -> std::shared_ptr<scene_actor_t>;

        auto get_parent_ref() const -> scene_actor_t&;

        auto has_parent() const -> bool;

        auto set_parent(std::shared_ptr<scene_actor_t> const& parent) -> void;

        auto set_parent(scene_actor_t& parent) -> void;

    public:
        auto get_child_ptr(size_t const index) const -> std::shared_ptr<scene_actor_t>;

        auto get_child_ref(size_t const index) const -> scene_actor_t&;

        auto num_child() const -> size_t;

    private:
        std::weak_ptr<scene_actor_t> m_parent{};

        std::vector<std::shared_ptr<scene_actor_t>> m_children{};

        // ------------------------------------------------

    public:
        auto get_local_position() const -> glm::vec3;

        auto get_local_rotation() const -> glm::quat;

        auto get_local_scale() const -> glm::vec3;

        auto get_local_transform() const -> glm::mat4;

        auto set_local_position(glm::vec3 const& position) -> void;

        auto set_local_rotation(glm::quat const& rotation) -> void;

        auto set_local_scale(glm::vec3 const& scale) -> void;

        auto set_local_transform(glm::mat4 const& transform) -> void;

    public:
        auto get_world_position() const -> glm::vec3;

        auto get_world_rotation() const -> glm::quat;

        auto get_world_scale() const -> glm::vec3;

        auto get_world_transform() const -> glm::mat4;

        auto set_world_position(glm::vec3 const& position) -> void;

        auto set_world_rotation(glm::quat const& rotation) -> void;

        auto set_world_scale(glm::vec3 const& scale) -> void;

        auto set_world_transform(glm::mat4 const& transform) -> void;

    private:
        glm::vec3 m_local_position{};

        glm::quat m_local_rotation{ glm::identity<glm::quat>() };

        glm::vec3 m_local_scale{ glm::one<glm::vec3>() };

        // ------------------------------------------------

    public:
        template <typename t_cmp>
        auto add_component_ptr() -> std::shared_ptr<t_cmp>;

        template <typename t_cmp>
        auto add_component_ref() -> t_cmp&;

        template <typename t_cmp>
        auto get_component_ptr() -> std::shared_ptr<t_cmp>;

        template <typename t_cmp>
        auto get_component_ref() -> t_cmp&;

        template <typename t_cmp>
        auto has_component() -> bool;

        template <typename t_cmp>
        auto rem_component() -> void;

    public:
        auto get_component_ptr(size_t const index) -> std::shared_ptr<scene_component_t>;

        auto get_component_ref(size_t const index) -> scene_component_t&;

        auto num_component() const -> size_t;

        auto rem_component(size_t const index) -> void;

    private:
        std::vector<std::shared_ptr<scene_component_t>> m_components{};

        // ------------------------------------------------
    };
}

// --------------------------------------------------------

namespace aiva2
{
    template <typename t_cmp>
    auto scene_actor_t::add_component_ptr() -> std::shared_ptr<t_cmp>
    {
        auto const component = new t_cmp(*this);
        assert_t::check_bool(component, "(component) is not valid");

        return m_components.emplace_back(component);
    }

    template <typename t_cmp>
    auto scene_actor_t::add_component_ref() -> t_cmp&
    {
        auto const component = add_component_ptr<t_cmp>();
        assert_t::check_bool(component, "(component) is not valid");

        return (*component);
    }

    template <typename t_cmp>
    auto scene_actor_t::get_component_ptr() -> std::shared_ptr<t_cmp>
    {
        for (auto i = size_t{}; i < num_component(); i++)
        {
            auto const component = get_component_ptr(i);
            assert_t::check_bool(component, "(component) is not valid");

            if (auto const cmp = std::dynamic_pointer_cast<t_cmp>(component))
            {
                return cmp;
            }
        }

        return {};
    }

    template <typename t_cmp>
    auto scene_actor_t::get_component_ref() -> t_cmp&
    {
        auto const component = get_component_ptr<t_cmp>();
        assert_t::check_bool(component, "(component) is not valid");

        return (*component);
    }

    template <typename t_cmp>
    auto scene_actor_t::has_component() -> bool
    {
        return (get_component_ptr<t_cmp>() != nullptr);
    }

    template <typename t_cmp>
    auto scene_actor_t::rem_component() -> void
    {
        for (auto i = num_component() - 1; i >= 0; i--)
        {
            auto const component = get_component_ptr(i);
            assert_t::check_bool(component, "(component) is not valid");

            if (std::dynamic_pointer_cast<t_cmp>(component))
            {
                rem_component(i);
            }
        }
    }
}
