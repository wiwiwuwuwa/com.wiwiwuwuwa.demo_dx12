#include <pch.h>
#include <aiva2/graphic_hardware.hpp>

#include <aiva2/asserts.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/window_system.hpp>

namespace aiva2
{
	graphic_hardware_t::graphic_hardware_t(engine_t& engine)
		: impl_type{ engine }
	{
		init_components();
	}

	graphic_hardware_t::~graphic_hardware_t()
	{
		shut_components();
	}

	void graphic_hardware_t::init_components()
	{
#if defined(_DEBUG)
		init_debug();
#endif
		init_factory();
		init_adapter();
		init_device();
#if defined(_DEBUG)
		init_info_queue();
#endif
		init_command_queue();
		init_is_tearing_allowed();
		init_swap_chain();
		init_command_allocator();
		init_command_list();
		init_fence();
	}

	void graphic_hardware_t::shut_components()
	{
		shut_fence();
		shut_command_list();
		shut_command_allocator();
		shut_swap_chain();
		shut_is_tearing_allowed();
		shut_command_queue();
#if defined(_DEBUG)
		shut_info_queue();
#endif
		shut_device();
		shut_adapter();
		shut_factory();
#if defined(_DEBUG)
		shut_debug();
#endif
	}

#if defined(_DEBUG)
	auto graphic_hardware_t::get_debug() const -> ID3D12Debug6&
	{
		asserts_t::check_true(m_debug, "m_debug is not valid");
		return *m_debug;
	}

	void graphic_hardware_t::init_debug()
	{
		auto basic_debug = winrt::com_ptr<ID3D12Debug>{};
		winrt::check_hresult(D3D12GetDebugInterface(IID_PPV_ARGS(&basic_debug)));
		asserts_t::check_true(basic_debug, "basic_debug is not valid");

		auto specific_debug = winrt::com_ptr<ID3D12Debug6>{};
		basic_debug.as(specific_debug);
		asserts_t::check_true(specific_debug, "specific_debug is not valid");

		specific_debug->EnableDebugLayer();
		m_debug = specific_debug;
	}

	void graphic_hardware_t::shut_debug()
	{
		asserts_t::check_true(m_debug, "m_debug is not valid");
		m_debug = {};
	}
#endif

	auto graphic_hardware_t::get_factory() const -> IDXGIFactory7&
	{
		asserts_t::check_true(m_factory, "m_factory is not valid");
		return *m_factory;
	}

	void graphic_hardware_t::init_factory()
	{
#if defined(_DEBUG)
		auto const flags = UINT{ DXGI_CREATE_FACTORY_DEBUG };
#else
		auto const flags = UINT{};
#endif

		auto basic_factory = winrt::com_ptr<IDXGIFactory2>{};
		winrt::check_hresult(CreateDXGIFactory2(flags, IID_PPV_ARGS(&basic_factory)));
		asserts_t::check_true(basic_factory, "basic_factory is not valid");

		auto specific_factory = winrt::com_ptr<IDXGIFactory7>{};
		basic_factory.as(specific_factory);
		asserts_t::check_true(specific_factory, "specific_factory is not valid");

		m_factory = specific_factory;
	}

	void graphic_hardware_t::shut_factory()
	{
		asserts_t::check_true(m_factory, "m_factory is not valid");
		m_factory = {};
	}

	auto graphic_hardware_t::get_adapter() const -> IDXGIAdapter4&
	{
		asserts_t::check_true(m_adapter, "m_adapter is not valid");
		return *m_adapter;
	}

	void graphic_hardware_t::init_adapter()
	{
		auto basic_adapter = winrt::com_ptr<IDXGIAdapter>{};
		winrt::check_hresult(get_factory().EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&basic_adapter)));
		asserts_t::check_true(basic_adapter, "basic_adapter is not valid");

		auto specific_adapter = winrt::com_ptr<IDXGIAdapter4>{};
		basic_adapter.as(specific_adapter);
		asserts_t::check_true(specific_adapter, "specific_adapter is not valid");

		m_adapter = specific_adapter;
	}
	
	void graphic_hardware_t::shut_adapter()
	{
		asserts_t::check_true(m_adapter, "m_adapter is not valid");
		m_adapter = {};
	}

	auto graphic_hardware_t::get_device() const -> ID3D12Device9&
	{
		asserts_t::check_true(m_device, "m_device is not valid");
		return *m_device;
	}

	void graphic_hardware_t::init_device()
	{
		auto basic_device = winrt::com_ptr<ID3D12Device>{};
		winrt::check_hresult(D3D12CreateDevice(&get_adapter(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&basic_device)));
		asserts_t::check_true(basic_device, "basic_device is not valid");

		auto specific_device = winrt::com_ptr<ID3D12Device9>{};
		basic_device.as(specific_device);
		asserts_t::check_true(specific_device, "specific_device is not valid");

		m_device = specific_device;
	}

	void graphic_hardware_t::shut_device()
	{
		asserts_t::check_true(m_device, "m_device is not valid");
		m_device = {};
	}

#if defined(_DEBUG)
	auto graphic_hardware_t::get_info_queue() const -> ID3D12InfoQueue1&
	{
		asserts_t::check_true(m_info_queue, "m_info_queue is not valid");
		return *m_info_queue;
	}

	void graphic_hardware_t::init_info_queue()
	{
		auto const& device = m_device;
		asserts_t::check_true(device, "device is not valid");

		auto info_queue = winrt::com_ptr<ID3D12InfoQueue1>{};
		device.as(info_queue);
		asserts_t::check_true(info_queue, "info_queue is not valid");

		info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

		{ // ignore empty index buffer warnings
			auto filter = D3D12_INFO_QUEUE_FILTER{};

			auto severities = std::vector<D3D12_MESSAGE_SEVERITY>{ D3D12_MESSAGE_SEVERITY_WARNING };
			filter.DenyList.NumSeverities = static_cast<UINT>(std::size(severities));
			filter.DenyList.pSeverityList = std::data(severities);

			auto message_ids = std::vector<D3D12_MESSAGE_ID>{ D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_INDEX_BUFFER_NOT_SET };
			filter.DenyList.NumIDs = static_cast<UINT>(std::size(message_ids));
			filter.DenyList.pIDList = std::data(message_ids);

			info_queue->PushStorageFilter(&filter);
		}

		m_info_queue = info_queue;
	}

	void graphic_hardware_t::shut_info_queue()
	{
		asserts_t::check_true(m_info_queue, "info_queue is not valid");
		m_info_queue = {};
	}	
#endif

	auto graphic_hardware_t::get_command_queue() const -> ID3D12CommandQueue&
	{
		asserts_t::check_true(m_command_queue, "m_command_queue is not valid");
		return *m_command_queue;
	}
	
	void graphic_hardware_t::init_command_queue()
	{
		auto desc = D3D12_COMMAND_QUEUE_DESC{};
		desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = {};

		auto command_queue = winrt::com_ptr<ID3D12CommandQueue>{};
		winrt::check_hresult(get_device().CreateCommandQueue(&desc, IID_PPV_ARGS(&command_queue)));
		asserts_t::check_true(command_queue, "command_queue is not valid");

		m_command_queue = command_queue;
	}

	void graphic_hardware_t::shut_command_queue()
	{
		asserts_t::check_true(m_command_queue, "m_command_queue is not valid");
		m_command_queue = {};
	}

	auto graphic_hardware_t::get_is_tearing_allowed() const -> bool
	{
		asserts_t::check_true(m_is_tearing_allowed, "m_is_tearing_allowed is not valid");
		return *m_is_tearing_allowed;
	}

	void graphic_hardware_t::init_is_tearing_allowed()
	{
		auto is_tearing_allowed = BOOL{};
		winrt::check_hresult(get_factory().CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &is_tearing_allowed, sizeof(is_tearing_allowed)));

		m_is_tearing_allowed = is_tearing_allowed;
	}

	void graphic_hardware_t::shut_is_tearing_allowed()
	{
		asserts_t::check_true(m_is_tearing_allowed, "m_is_tearing_allowed is not valid");
		m_is_tearing_allowed = {};
	}

	auto graphic_hardware_t::get_swap_chain() const -> IDXGISwapChain4&
	{
		asserts_t::check_true(m_swap_chain, "m_swap_chain is not valid");
		return *m_swap_chain;
	}

	void graphic_hardware_t::init_swap_chain()
	{
		auto desc = DXGI_SWAP_CHAIN_DESC1{};
		desc.Width = {};
		desc.Height = {};
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.Stereo = FALSE;
		desc.SampleDesc = DXGI_SAMPLE_DESC{ 1, 0 };
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 2;
		desc.Scaling = DXGI_SCALING_STRETCH;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		desc.Flags = get_is_tearing_allowed() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : NULL;

		auto basic_swap_chain = winrt::com_ptr<IDXGISwapChain1>{};
		winrt::check_hresult(get_factory().CreateSwapChainForCoreWindow(&get_command_queue(), winrt::get_unknown(get_engine().get_window_system().get_core_window()), &desc, nullptr, basic_swap_chain.put()));
		asserts_t::check_true(basic_swap_chain, "basic_swap_chain is not valid");

		auto specific_swap_chain = winrt::com_ptr<IDXGISwapChain4>{};
		basic_swap_chain.as(specific_swap_chain);
		asserts_t::check_true(specific_swap_chain, "specific_swap_chain is not valid");

		m_swap_chain = specific_swap_chain;
	}

	void graphic_hardware_t::shut_swap_chain()
	{
		asserts_t::check_true(m_swap_chain, "m_swap_chain is not valid");
		m_swap_chain = {};
	}

	auto graphic_hardware_t::get_command_allocator() const -> ID3D12CommandAllocator&
	{
		asserts_t::check_true(m_command_allocator, "m_command_allocator is not valid");
		return *m_command_allocator;
	}

	void graphic_hardware_t::init_command_allocator()
	{
		auto command_allocator = winrt::com_ptr<ID3D12CommandAllocator>{};
		winrt::check_hresult(get_device().CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocator)));
		asserts_t::check_true(command_allocator, "command_allocator is not valid");

		m_command_allocator = command_allocator;
	}

	void graphic_hardware_t::shut_command_allocator()
	{
		asserts_t::check_true(m_command_allocator, "m_command_allocator is not valid");
		m_command_allocator = {};
	}

	auto graphic_hardware_t::get_command_list() const -> ID3D12GraphicsCommandList6&
	{
		asserts_t::check_true(m_command_list, "m_command_list is not valid");
		return *m_command_list;
	}
	
	void graphic_hardware_t::init_command_list()
	{
		auto basic_command_list = winrt::com_ptr<ID3D12GraphicsCommandList>{};
		winrt::check_hresult(get_device().CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, &get_command_allocator(), nullptr, IID_PPV_ARGS(&basic_command_list)));
		asserts_t::check_true(basic_command_list, "basic_command_list is not valid");

		auto specific_command_list = winrt::com_ptr<ID3D12GraphicsCommandList6>{};
		basic_command_list.as(specific_command_list);
		asserts_t::check_true(specific_command_list, "specific_command_list is not valid");

		winrt::check_hresult(specific_command_list->Close());
		m_command_list = specific_command_list;
	}

	void graphic_hardware_t::shut_command_list()
	{
		asserts_t::check_true(m_command_list, "m_command_list is not valid");
		m_command_list = {};
	}

	auto graphic_hardware_t::get_fence() const -> ID3D12Fence1&
	{
		asserts_t::check_true(m_fence, "m_fence is not valid");
		return *m_fence;
	}

	void graphic_hardware_t::init_fence()
	{
		auto basic_fence = winrt::com_ptr<ID3D12Fence>{};
		winrt::check_hresult(get_device().CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&basic_fence)));
		asserts_t::check_true(basic_fence, "basic_fence is not valid");

		auto specific_fence = winrt::com_ptr<ID3D12Fence1>{};
		basic_fence.as(specific_fence);
		asserts_t::check_true(specific_fence, "specific_fence is not valid");

		m_fence = specific_fence;
	}

	void graphic_hardware_t::shut_fence()
	{
		asserts_t::check_true(m_fence, "m_fence is not valid");
		m_fence = {};
	}
}
