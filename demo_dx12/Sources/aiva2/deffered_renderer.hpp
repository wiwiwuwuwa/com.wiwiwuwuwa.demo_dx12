#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/implements.hpp>

namespace aiva2
{
	struct deffered_renderer_t final : public implements_t<deffered_renderer_t, engine_object_t>
	{
		// ------------------------------------------------

	public:
		deffered_renderer_t(engine_t& engine);

		~deffered_renderer_t() override;

	private:
		void init_deffered_renderer();

		void tick_deffered_renderer();

		void shut_deffered_renderer();

		// ------------------------------------------------

	private:
		void init_random_effect();

		void tick_random_effect();

		void shut_random_effect();

	private:
		std::shared_ptr<compute_material_t> m_random_effect_material{};

		std::shared_ptr<tex_2d_t> m_random_effect_output{};

		// ------------------------------------------------

	private:
		void tick_present();

		// ------------------------------------------------
	};
}
