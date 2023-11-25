#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>

namespace aiva2
{
    struct scene_gltf_t final : public implements_t<scene_gltf_t, engine_object_t>
    {
        // ------------------------------------------------

    public:
        scene_gltf_t(engine_t& engine, boost::span<std::byte const> const& binary);

        ~scene_gltf_t() override;

        // ------------------------------------------------

    public:
        auto get_model() const -> tinygltf::Model const&;

    private:
        void init_model(boost::span<std::byte const> const& binary);

        void shut_model();

    private:
        tinygltf::Model m_model{};

        // ------------------------------------------------
    };
}
