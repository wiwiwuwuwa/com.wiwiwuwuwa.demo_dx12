#include <pch.h>
#include <aiva/layer1_ext/nr_sampler.h>

#include <aiva/layer1/engine.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;

	NrSampler::NrSampler(aiva::layer1::Engine& engine) : mEngine{ engine }
	{

	}

	NrSampler::~NrSampler()
	{

	}
}
