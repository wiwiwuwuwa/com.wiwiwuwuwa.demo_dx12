#include <pch.h>
#include <aiva/layer2_ext/sc_actor.h>

#include <aiva/layer2/world.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer2_ext
{
	using namespace aiva::utils;

	ScActor::ScActor(aiva::layer2::World& world) : mWorld{ world }
	{
		InitHierarchyFields();
	}

	ScActor::~ScActor()
	{
		ShutHierarchyFields();
	}

	std::shared_ptr<ScActor> ScActor::Parent() const
	{
		return mParent.lock();
	}

	void ScActor::Parent(std::shared_ptr<ScActor> const& parent)
	{
		auto const currentParent = mParent.lock();
		if (currentParent)
		{
			std::remove(std::begin(currentParent->mChildren), std::end(currentParent->mChildren), shared_from_this());
		}

		mParent = parent;

		auto const desiredParent = mParent.lock();
		if (desiredParent)
		{
			desiredParent->mChildren.emplace_back(shared_from_this());
		}
	}

	std::shared_ptr<ScActor> ScActor::GetChild(std::size_t const index) const
	{
		Asserts::CheckBool(index >= decltype(index){} && index < std::size(mChildren), "Index is out of range");
		return mChildren.at(index);
	}

	std::size_t ScActor::NumChild() const
	{
		return std::size(mChildren);
	}

	void ScActor::InitHierarchyFields()
	{
		Parent({});
	}

	void ScActor::ShutHierarchyFields()
	{
		Parent({});
	}

	glm::mat4 ScActor::LocalTransform() const
	{
		auto const position = glm::translate(glm::identity<glm::mat4>(), mLocalPosition);
		auto const rotation = glm::mat4_cast(mLocalRotation);
		auto const scale = glm::scale(glm::identity<glm::mat4>(), mLocalScale);

		return position * rotation * scale;
	}

	glm::mat4 ScActor::WorldTransform() const
	{
		auto const parent = Parent();
		if (parent)
		{
			return parent->WorldTransform() * LocalTransform();
		}
		else
		{
			return LocalTransform();
		}
	}

	glm::mat4 ScActor::WorldView() const
	{
		auto const transform = WorldTransform();

		auto const origin = glm::vec3(transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		auto const forward = glm::vec3(transform * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
		auto const upwards = glm::vec3(transform * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));

		return glm::lookAt(origin, origin + forward, upwards);
	}
}
