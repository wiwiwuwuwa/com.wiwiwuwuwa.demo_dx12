#pragma once
#include <aiva2/base.hpp>

#include <aiva2/core/implements.hpp>

namespace aiva2::native
{
	struct graphic_hardware_t : public core::implements_t<graphic_hardware_t>
	{
		// ------------------------------------------------
		// graphic hardware

	public:
		graphic_hardware_t(engine_t& engine);

		~graphic_hardware_t() override;

		// ------------------------------------------------

	public:
		auto get_engine() const->engine_t&;

	private:
		engine_t& m_engine;

		// ------------------------------------------------
		// components

	public:
#if defined(_DEBUG)
		auto get_debug() const->ID3D12Debug6&;
#endif

		auto get_factory() const->IDXGIFactory7&;

		auto get_adapter() const->IDXGIAdapter4&;

		auto get_device() const->ID3D12Device9&;

#if defined(_DEBUG)
		auto get_info_queue() const->ID3D12InfoQueue1&;
#endif

		auto get_command_queue() const->ID3D12CommandQueue&;

		auto get_swap_chain() const->IDXGISwapChain4&;

		auto get_command_allocator() const->ID3D12CommandAllocator&;

		auto get_command_list() const->ID3D12GraphicsCommandList6&;

		auto get_fence() const->ID3D12Fence1&;

		auto get_is_tearing_allowed() const -> bool;

	private:
		void init_components();

		void shut_components();

#if defined(_DEBUG)
		void init_debug();

		void shut_debug();
#endif

		void init_factory();

		void shut_factory();

		void init_adapter();

		void shut_adapter();

		void init_device();

		void shut_device();

#if defined(_DEBUG)
		void init_info_queue();

		void shut_info_queue();
#endif

		void init_command_queue();

		void shut_command_queue();

		void init_is_tearing_allowed();

		void shut_is_tearing_allowed();

		void init_swap_chain();

		void shut_swap_chain();

		void init_command_allocator();

		void shut_command_allocator();

		void init_command_list();

		void shut_command_list();

		void init_fence();

		void shut_fence();

	private:
#if defined(_DEBUG)
		winrt::com_ptr<ID3D12Debug6> m_debug{};
#endif

		winrt::com_ptr<IDXGIFactory7> m_factory{};

		winrt::com_ptr<IDXGIAdapter4> m_adapter{};

		winrt::com_ptr<ID3D12Device9> m_device{};

#if defined(_DEBUG)
		winrt::com_ptr<ID3D12InfoQueue1> m_info_queue{};
#endif

		winrt::com_ptr<ID3D12CommandQueue> m_command_queue{};

		std::optional<bool> m_is_tearing_allowed{};

		winrt::com_ptr<IDXGISwapChain4> m_swap_chain{};

		winrt::com_ptr<ID3D12CommandAllocator> m_command_allocator{};

		winrt::com_ptr<ID3D12GraphicsCommandList6> m_command_list{};

		winrt::com_ptr<ID3D12Fence1> m_fence{};

		// ------------------------------------------------
	};
}
