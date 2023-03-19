#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/buffer_format.hpp>
#include <aiva2/implements.hpp>
#include <aiva2/struct.hpp>

namespace aiva2
{
	struct uav_res_info_t : public implements_t<uav_res_info_t, struct_t>
	{
		// ------------------------------------------------

	protected:
		uav_res_info_t() = default;

		~uav_res_info_t() override = default;

		// ------------------------------------------------

	public:
		auto get_format() const->buffer_format_t;
		
		void set_format(buffer_format_t const format);

	private:
		buffer_format_t m_format{ buffer_format_t::UNKNOWN };

		// ------------------------------------------------
	};
}
