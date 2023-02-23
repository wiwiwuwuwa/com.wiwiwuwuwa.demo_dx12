//#pragma once
//#include <aiva2/_init.hpp>
//#include <aiva2/implements.hpp>
//#include <aiva2/engine_object.hpp>
//
//namespace aiva2::native
//{
//	struct graphic_hardware_t : public implements_t<graphic_hardware_t, engine_object_t>
//	{
//		// ------------------------------------------------
//
//	public:
//		graphic_hardware_t(engine_t& engine);
//
//		~graphic_hardware_t() override;
//
//		// ------------------------------------------------
//
//	private:
//		void init_components();
//
//		void shut_components();
//
//		// ------------------------------------------------
//
//#if defined(_DEBUG)
//	public:
//		auto get_debug() const->ID3D12Debug6&;
//
//	private:
//		void init_debug();
//
//		void shut_debug();
//
//	private:
//		winrt::com_ptr<ID3D12Debug6> m_debug{};
//#endif
//
//		// ------------------------------------------------
//
//	public:
//		auto get_factory() const->IDXGIFactory7&;
//
//	private:
//		void init_factory();
//
//		void shut_factory();
//
//	private:
//		winrt::com_ptr<IDXGIFactory7> m_factory{};
//
//		// ------------------------------------------------
//
//	public:
//		auto get_adapter() const->IDXGIAdapter4&;
//
//	private:
//		void init_adapter();
//
//		void shut_adapter();
//
//	private:
//		winrt::com_ptr<IDXGIAdapter4> m_adapter{};
//
//		// ------------------------------------------------
//
//	public:
//		auto get_device() const->ID3D12Device9&;
//
//	private:
//		void init_device();
//
//		void shut_device();
//
//	private:
//		winrt::com_ptr<ID3D12Device9> m_device{};
//
//		// ------------------------------------------------
//
//#if defined(_DEBUG)
//	public:
//		auto get_info_queue() const->ID3D12InfoQueue1&;
//
//	private:
//		void init_info_queue();
//
//		void shut_info_queue();
//
//	private:
//		winrt::com_ptr<ID3D12InfoQueue1> m_info_queue{};
//#endif
//
//		// ------------------------------------------------
//
//	public:
//		auto get_command_queue() const->ID3D12CommandQueue&;
//
//	private:
//		void init_command_queue();
//
//		void shut_command_queue();
//
//	private:
//		winrt::com_ptr<ID3D12CommandQueue> m_command_queue{};
//
//		// ------------------------------------------------
//
//	public:
//		auto get_is_tearing_allowed() const -> bool;
//		
//	private:
//		void init_is_tearing_allowed();
//		
//		void shut_is_tearing_allowed();
//
//	private:
//		std::optional<bool> m_is_tearing_allowed{};
//
//		// ------------------------------------------------
//
//	public:
//		auto get_swap_chain() const->IDXGISwapChain4&;
//		
//	private:
//		void init_swap_chain();
//
//		void shut_swap_chain();
//
//	private:
//		winrt::com_ptr<IDXGISwapChain4> m_swap_chain{};
//		
//		// ------------------------------------------------
//
//	public:
//		auto get_command_allocator() const->ID3D12CommandAllocator&;
//		
//	private:
//		void init_command_allocator();
//
//		void shut_command_allocator();
//
//	private:
//		winrt::com_ptr<ID3D12CommandAllocator> m_command_allocator{};
//
//		// ------------------------------------------------
//
//	public:
//		auto get_command_list() const->ID3D12GraphicsCommandList6&;
//
//	private:
//		void init_command_list();
//
//		void shut_command_list();
//
//	private:
//		winrt::com_ptr<ID3D12GraphicsCommandList6> m_command_list{};
//
//		// ------------------------------------------------
//
//	public:
//		auto get_fence() const->ID3D12Fence1&;
//
//	private:
//		void init_fence();
//		
//		void shut_fence();
//
//	private:
//		winrt::com_ptr<ID3D12Fence1> m_fence{};
//
//		// ------------------------------------------------
//	};
//}
