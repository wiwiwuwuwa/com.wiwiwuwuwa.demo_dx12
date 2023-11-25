#include <aiva/graphic_hardware.hpp>

#include <aiva/assert.hpp>
#include <aiva/engine.hpp>
#include <aiva/hardware_system.hpp>

namespace aiva
{
    graphic_hardware_t::graphic_hardware_t(engine_t& engine)
        : impl_type{ engine }
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

    graphic_hardware_t::~graphic_hardware_t()
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
        assert_t::check_bool(m_debug, "(m_debug) is not valid");
        return (*m_debug.Get());
    }

    void graphic_hardware_t::init_debug()
    {
        auto basic_debug = Microsoft::WRL::ComPtr<ID3D12Debug>{};
        assert_t::check_hresult(D3D12GetDebugInterface(IID_PPV_ARGS(&basic_debug)));
        assert_t::check_bool(basic_debug, "(basic_debug) is not valid");

        auto specific_debug = Microsoft::WRL::ComPtr<ID3D12Debug6>{};
        basic_debug.As(&specific_debug);
        assert_t::check_bool(specific_debug, "(specific_debug) is not valid");

        (*specific_debug.Get()).EnableDebugLayer();
        m_debug = specific_debug;
    }

    void graphic_hardware_t::shut_debug()
    {
        assert_t::check_bool(m_debug, "(m_debug) is not valid");
        m_debug.Reset();
    }
#endif

    auto graphic_hardware_t::get_factory() const -> IDXGIFactory7&
    {
        assert_t::check_bool(m_factory, "(m_factory) is not valid");
        return (*m_factory.Get());
    }

    void graphic_hardware_t::init_factory()
    {
#if defined(_DEBUG)
        auto const flags = UINT{ DXGI_CREATE_FACTORY_DEBUG };
#else
        auto const flags = UINT{};
#endif

        auto basic_factory = Microsoft::WRL::ComPtr<IDXGIFactory2>{};
        assert_t::check_hresult(CreateDXGIFactory2(flags, IID_PPV_ARGS(&basic_factory)));
        assert_t::check_bool(basic_factory, "(basic_factory) is not valid");

        auto specific_factory = Microsoft::WRL::ComPtr<IDXGIFactory7>{};
        basic_factory.As(&specific_factory);
        assert_t::check_bool(specific_factory, "(specific_factory) is not valid");

        m_factory = specific_factory;
    }

    void graphic_hardware_t::shut_factory()
    {
        assert_t::check_bool(m_factory, "(m_factory) is not valid");
        m_factory.Reset();
    }

    auto graphic_hardware_t::get_adapter() const -> IDXGIAdapter4&
    {
        assert_t::check_bool(m_adapter, "(m_adapter) is not valid");
        return (*m_adapter.Get());
    }

    void graphic_hardware_t::init_adapter()
    {
        auto basic_adapter = Microsoft::WRL::ComPtr<IDXGIAdapter>{};
        assert_t::check_hresult(get_factory().EnumAdapterByGpuPreference({}, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&basic_adapter)));
        assert_t::check_bool(basic_adapter, "(basic_adapter) is not valid");

        auto specific_adapter = Microsoft::WRL::ComPtr<IDXGIAdapter4>{};
        basic_adapter.As(&specific_adapter);
        assert_t::check_bool(specific_adapter, "(specific_adapter) is not valid");

        m_adapter = specific_adapter;
    }

    void graphic_hardware_t::shut_adapter()
    {
        assert_t::check_bool(m_adapter, "(m_adapter) is not valid");
        m_adapter.Reset();
    }

    auto graphic_hardware_t::get_device() const -> ID3D12Device9&
    {
        assert_t::check_bool(m_device, "(m_device) is not valid");
        return (*m_device.Get());
    }

    void graphic_hardware_t::init_device()
    {
        auto basic_device = Microsoft::WRL::ComPtr<ID3D12Device>{};
        assert_t::check_hresult(D3D12CreateDevice(&get_adapter(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&basic_device)));
        assert_t::check_bool(basic_device, "(basic_device) is not valid");

        auto specific_device = Microsoft::WRL::ComPtr<ID3D12Device9>{};
        basic_device.As(&specific_device);
        assert_t::check_bool(specific_device, "(specific_device) is not valid");

        m_device = specific_device;
    }

    void graphic_hardware_t::shut_device()
    {
        assert_t::check_bool(m_device, "(m_device) is not valid");
        m_device.Reset();
    }

#if defined(_DEBUG)
    auto graphic_hardware_t::get_info_queue() const -> ID3D12InfoQueue1&
    {
        assert_t::check_bool(m_info_queue, "(m_info_queue) is not valid");
        return (*m_info_queue.Get());
    }

    void graphic_hardware_t::init_info_queue()
    {
        auto const& device = m_device;
        assert_t::check_bool(device, "(device) is not valid");

        auto info_queue = Microsoft::WRL::ComPtr<ID3D12InfoQueue1>{};
        device.As(&info_queue);
        assert_t::check_bool(info_queue, "(info_queue) is not valid");

        (*info_queue.Get()).SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
        (*info_queue.Get()).SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
        (*info_queue.Get()).SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

        { // ignore empty index buffer warnings
            auto filter = D3D12_INFO_QUEUE_FILTER{};

            auto severities = std::vector<D3D12_MESSAGE_SEVERITY>{ D3D12_MESSAGE_SEVERITY_WARNING };
            filter.DenyList.NumSeverities = static_cast<UINT>(std::size(severities));
            filter.DenyList.pSeverityList = std::data(severities);

            auto message_ids = std::vector<D3D12_MESSAGE_ID>{ D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_INDEX_BUFFER_NOT_SET };
            filter.DenyList.NumIDs = static_cast<UINT>(std::size(message_ids));
            filter.DenyList.pIDList = std::data(message_ids);

            (*info_queue.Get()).PushStorageFilter(&filter);
        }

        m_info_queue = info_queue;
    }

    void graphic_hardware_t::shut_info_queue()
    {
        assert_t::check_bool(m_info_queue, "(m_info_queue) is not valid");
        m_info_queue.Reset();
    }
#endif

    auto graphic_hardware_t::get_command_queue() const -> ID3D12CommandQueue&
    {
        assert_t::check_bool(m_command_queue, "(m_command_queue) is not valid");
        return (*m_command_queue.Get());
    }

    void graphic_hardware_t::init_command_queue()
    {
        auto desc = D3D12_COMMAND_QUEUE_DESC{};
        desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        desc.NodeMask = {};

        auto command_queue = Microsoft::WRL::ComPtr<ID3D12CommandQueue>{};
        assert_t::check_hresult(get_device().CreateCommandQueue(&desc, IID_PPV_ARGS(&command_queue)));
        assert_t::check_bool(command_queue, "(command_queue) is not valid");

        m_command_queue = command_queue;
    }

    void graphic_hardware_t::shut_command_queue()
    {
        assert_t::check_bool(m_command_queue, "(m_command_queue) is not valid");
        m_command_queue.Reset();
    }

    auto graphic_hardware_t::get_is_tearing_allowed() const -> bool
    {
        assert_t::check_bool(m_is_tearing_allowed, "(m_is_tearing_allowed) is not valid");
        return (*m_is_tearing_allowed);
    }

    void graphic_hardware_t::init_is_tearing_allowed()
    {
        auto is_tearing_allowed = BOOL{};
        assert_t::check_hresult(get_factory().CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &is_tearing_allowed, sizeof(is_tearing_allowed)));

        m_is_tearing_allowed = is_tearing_allowed;
    }

    void graphic_hardware_t::shut_is_tearing_allowed()
    {
        assert_t::check_bool(m_is_tearing_allowed, "(m_is_tearing_allowed) is not valid");
        m_is_tearing_allowed.reset();
    }

    auto graphic_hardware_t::get_swap_chain() const -> IDXGISwapChain4&
    {
        assert_t::check_bool(m_swap_chain, "(m_swap_chain) is not valid");
        return (*m_swap_chain.Get());
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
        desc.Flags = get_is_tearing_allowed() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : UINT{};

        auto basic_swap_chain = Microsoft::WRL::ComPtr<IDXGISwapChain1>{};
        assert_t::check_hresult(get_factory().CreateSwapChainForHwnd(&get_command_queue(), get_engine().get_hardware_system().get_hwnd(), &desc, {}, {}, &basic_swap_chain));
        assert_t::check_bool(basic_swap_chain, "(basic_swap_chain) is not valid");

        auto specific_swap_chain = Microsoft::WRL::ComPtr<IDXGISwapChain4>{};
        basic_swap_chain.As(&specific_swap_chain);
        assert_t::check_bool(specific_swap_chain, "(specific_swap_chain) is not valid");

        m_swap_chain = specific_swap_chain;
    }

    void graphic_hardware_t::shut_swap_chain()
    {
        assert_t::check_bool(m_swap_chain, "(m_swap_chain) is not valid");
        m_swap_chain.Reset();
    }

    auto graphic_hardware_t::get_command_allocator() const -> ID3D12CommandAllocator&
    {
        assert_t::check_bool(m_command_allocator, "(m_command_allocator) is not valid");
        return (*m_command_allocator.Get());
    }

    void graphic_hardware_t::init_command_allocator()
    {
        auto command_allocator = Microsoft::WRL::ComPtr<ID3D12CommandAllocator>{};
        assert_t::check_hresult(get_device().CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocator)));
        assert_t::check_bool(command_allocator, "(command_allocator) is not valid");

        m_command_allocator = command_allocator;
    }

    void graphic_hardware_t::shut_command_allocator()
    {
        assert_t::check_bool(m_command_allocator, "(m_command_allocator) is not valid");
        m_command_allocator.Reset();
    }

    auto graphic_hardware_t::get_command_list() const -> ID3D12GraphicsCommandList6&
    {
        assert_t::check_bool(m_command_list, "(m_command_list) is not valid");
        return (*m_command_list.Get());
    }

    void graphic_hardware_t::init_command_list()
    {
        auto basic_command_list = Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>{};
        assert_t::check_hresult(get_device().CreateCommandList({}, D3D12_COMMAND_LIST_TYPE_DIRECT, &get_command_allocator(), {}, IID_PPV_ARGS(&basic_command_list)));
        assert_t::check_bool(basic_command_list, "(basic_command_list) is not valid");

        auto specific_command_list = Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6>{};
        basic_command_list.As(&specific_command_list);
        assert_t::check_bool(specific_command_list, "(specific_command_list) is not valid");

        assert_t::check_hresult((*specific_command_list.Get()).Close());
        m_command_list = specific_command_list;
    }

    void graphic_hardware_t::shut_command_list()
    {
        assert_t::check_bool(m_command_list, "(m_command_list) is not valid");
        m_command_list.Reset();
    }

    auto graphic_hardware_t::get_fence() const -> ID3D12Fence1&
    {
        assert_t::check_bool(m_fence, "(m_fence) is not valid");
        return (*m_fence.Get());
    }

    void graphic_hardware_t::init_fence()
    {
        auto basic_fence = Microsoft::WRL::ComPtr<ID3D12Fence>{};
        assert_t::check_hresult(get_device().CreateFence({}, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&basic_fence)));
        assert_t::check_bool(basic_fence, "(basic_fence) is not valid");

        auto specific_fence = Microsoft::WRL::ComPtr<ID3D12Fence1>{};
        basic_fence.As(&specific_fence);
        assert_t::check_bool(specific_fence, "(specific_fence) is not valid");

        m_fence = specific_fence;
    }

    void graphic_hardware_t::shut_fence()
    {
        assert_t::check_bool(m_fence, "(m_fence) is not valid");
        m_fence.Reset();
    }
}
