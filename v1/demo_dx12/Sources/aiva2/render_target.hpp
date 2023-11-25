#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/implements.hpp>

namespace aiva2
{
	struct render_target_t final : public implements_t<render_target_t, engine_object_t>
	{
		// ------------------------------------------------

	public:
		render_target_t(engine_t& engine);
		
		~render_target_t() override;

	public:
		auto init_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

		auto shut_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

		// ------------------------------------------------
		// RENDER TARGET API
		// ------------------------------------------------

	public:
		void add_color_resource(std::shared_ptr<gpu_res_t> const& resource, std::shared_ptr<gpu_eye_info_t> const& info);

		auto get_color_resource(size_t const index) const->std::shared_ptr<gpu_res_t>;

		auto num_color_resource() const->size_t;

		void rem_color_resource(size_t const index);

		void set_color_resource(size_t const index, std::shared_ptr<gpu_res_t> const& resource, std::shared_ptr<gpu_eye_info_t> const& info);

	public:
		auto get_color_handle(size_t const index = {}) const->std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>;

		auto num_color_handle() const->size_t;

	private:
		auto init_color_resources_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

		auto shut_color_resources_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

	private:
		void upd_color_resource();

	private:
		std::vector<std::shared_ptr<gpu_eye_t>> m_color_views{};

		winrt::com_ptr<ID3D12DescriptorHeap> m_color_heap{};

		// ------------------------------------------------
		// DEPTH STENCIL API
		// ------------------------------------------------
		
	public:
		auto get_depth_resource() const->std::shared_ptr<gpu_res_t>;

		auto has_depth_resource() const->bool;

		void set_depth_resource(std::shared_ptr<gpu_res_t> const& resource, std::shared_ptr<gpu_eye_info_t> const& info);

	public:
		auto get_depth_handle() const->std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>;

	private:
		auto init_depth_resource_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

		auto shut_depth_resource_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

	private:
		void upd_depth_resource();

	private:
		std::shared_ptr<gpu_eye_t> m_depth_view{};

		winrt::com_ptr<ID3D12DescriptorHeap> m_depth_heap{};

		// ------------------------------------------------
		// UTILS
		// ------------------------------------------------

	public:
		auto get_viewport_size(size_t const index) const->glm::size2;

		auto num_viewport_size() const->size_t;
		
		// ------------------------------------------------
	};
}
