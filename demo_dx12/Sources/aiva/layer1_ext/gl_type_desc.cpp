#include <pch.h>
#include <aiva/layer1_ext/gl_type_desc.h>

namespace aiva::layer1_ext
{
	GlTypeDesc::GlTypeDesc() : GlTypeDesc{ {}, {} }
	{

	}

	GlTypeDesc::GlTypeDesc(std::size_t const typeOf, std::size_t const typeIn) : mTypeOf{ typeOf }, mTypeIn{ typeIn }
	{

	}

	bool operator==(GlTypeDesc const& a, GlTypeDesc const& b)
	{
		auto equals = true;
		equals &= (a.TypeOf() == b.TypeOf());
		equals &= (a.TypeIn() == b.TypeIn());
		return equals;
	}

	std::size_t hash_value(GlTypeDesc const& v)
	{
		auto hash = std::size_t{};
		boost::hash_combine(hash, v.TypeOf());
		boost::hash_combine(hash, v.TypeIn());
		return hash;
	}
}
