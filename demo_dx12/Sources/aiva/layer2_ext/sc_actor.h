#pragma once
#include <pch.h>

#include <aiva/layer2/world_fwd.h>
#include <aiva/layer2_ext/sc_actor_fwd.h>
#include <aiva/layer2_ext/sc_component_fwd.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer2_ext
{
	struct ScActor final : public aiva::utils_ext::TObject<ScActor>
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(ScActor);

	protected:
		ScActor(aiva::layer2::World& world);

	public:
		~ScActor() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer2::World, World);

	// ----------------------------------------------------
	// Hierarchy Fields

	public:
		std::shared_ptr<ScActor> Parent() const;

		void Parent(std::shared_ptr<ScActor> const& parent);

	private:
		std::weak_ptr<ScActor> mParent{};

	// --------------------------------

	public:
		std::shared_ptr<ScActor> GetChild(std::size_t const index) const;

		std::size_t NumChild() const;

	private:
		std::vector<std::shared_ptr<ScActor>> mChildren{};

	// --------------------------------

	private:
		void InitHierarchyFields();

		void ShutHierarchyFields();

	// ----------------------------------------------------
	// Transformations Fields

	public:
		M_OBJECT_FIELD_VAR_3(public, glm::vec3, LocalPosition);

		M_OBJECT_FIELD_VAR_3(public, glm::quat, LocalRotation);

		M_OBJECT_FIELD_VAR_3(public, glm::vec3, LocalScale);

	// ----------------------------------------------------
	// Transformations Shortcut Fields

	public:
		glm::mat4 LocalTransform() const;

		glm::mat4 WorldTransform() const;

		glm::mat4 WorldView() const;

	// ----------------------------------------------------
	// Components Fields

	public:
		template <typename T>
		std::shared_ptr<T> CreateComponent();

	private:
		std::vector<std::shared_ptr<ScComponent>> mComponents{};
	};
}

// --------------------------------------------------------

#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer2_ext
{
	template <typename T>
	std::shared_ptr<T> ScActor::CreateComponent()
	{
		auto const component = aiva::utils_ext::NewObject<T>(*this);
		aiva::utils::Asserts::CheckBool(component, "Component is not valid");

		mComponents.emplace_back(component);
		return component;
	}
}
