#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/implements.hpp>
#include <aiva2/object.hpp>

namespace aiva2
{
    struct d3d12_shader_bytecode_cs_t final : public implements_t<d3d12_shader_bytecode_cs_t, object_t>
    {
        // ------------------------------------------------

    public:
        d3d12_shader_bytecode_cs_t(shader_info_t const& shader_info);

        ~d3d12_shader_bytecode_cs_t() override;

        // ------------------------------------------------

    public:
        auto get_data() const->D3D12_SHADER_BYTECODE;

    private:
        void init_bytecode(shader_info_t const& shader_info);

        void shut_bytecode();

    private:
        winrt::com_ptr<ID3DBlob> m_bytecode{};

        // ------------------------------------------------
    };
}
