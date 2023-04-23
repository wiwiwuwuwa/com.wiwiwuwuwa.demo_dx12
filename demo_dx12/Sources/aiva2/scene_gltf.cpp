#include <pch.h>
#include <aiva2/scene_gltf.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/logger.hpp>

namespace aiva2
{
    scene_gltf_t::scene_gltf_t(engine_t& engine, boost::span<std::byte const> const& binary)
        : impl_type{ engine }
    {
        init_model(binary);
    }

    scene_gltf_t::~scene_gltf_t()
    {
        shut_model();
    }

    auto scene_gltf_t::get_model() const -> tinygltf::Model const&
    {
        return m_model;
    }

    void scene_gltf_t::init_model(boost::span<std::byte const> const& binary)
    {
        assert_t::check_bool(!std::empty(binary), "(binary) is not valid");

        auto warnings = std::string{};
        auto errors = std::string{};

        auto const is_loaded = tinygltf::TinyGLTF{}.LoadASCIIFromString
        (
            /*model*/ &m_model,
            /*err*/ &errors,
            /*warn*/ &warnings,
            /*str*/ reinterpret_cast<const char*>(std::data(binary)),
            /*length*/ std::size(binary),
            /*baseDir*/ {}
        );

        if (!std::empty(warnings))
        {
            logger_t::log(warnings);
        }

        if (!std::empty(errors))
        {
            logger_t::log(errors);
        }

        assert_t::check_bool(is_loaded, "failed to load gltf scene from binary data");
    }

    void scene_gltf_t::shut_model()
    {
        m_model = {};
    }
}
