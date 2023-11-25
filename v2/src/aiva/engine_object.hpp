#pragma once
#include <aiva/object.hpp>

namespace aiva
{
	struct engine_object_t : public implements_t<engine_object_t, object_t>
	{
        // ------------------------------------------------

    public:
        engine_object_t(struct engine_t& engine);

        ~engine_object_t() override;

        // ------------------------------------------------

    public:
        auto get_engine() const -> struct engine_t&;

    private:
        struct engine_t& m_engine;

        // ------------------------------------------------
	};
}
