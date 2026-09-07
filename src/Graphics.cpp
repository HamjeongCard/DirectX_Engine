#include "Graphics.h"
#include <stdexcept>

Graphics::Graphics(HWND hWnd, int width, int height) {
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = hWnd;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    UINT createFlags = 0;
#if defined(_DEBUG)
    createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createFlags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &m_pSwapChain,
        &m_pDevice,
        nullptr,
        &m_pContext
    );

    if (FAILED(hr)) {
        // Fallback to WARP or Software if HW device creation fails
        hr = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_WARP,
            nullptr,
            createFlags,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &sd,
            &m_pSwapChain,
            &m_pDevice,
            nullptr,
            &m_pContext
        );
    }

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create DirectX 11 Device and Swap Chain");
    }

    // Create Render Target View
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackBuffer);
    m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pTargetView);

    // Create Depth Stencil State & Texture & View
    D3D11_DEPTH_STENCIL_DESC dsd = {};
    dsd.DepthEnable = TRUE;
    dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsd.DepthFunc = D3D11_COMPARISON_LESS;

    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
    m_pDevice->CreateDepthStencilState(&dsd, &pDSState);
    m_pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

    Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1u;
    descDepth.ArraySize = 1u;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1u;
    descDepth.SampleDesc.Quality = 0u;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    m_pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0u;
    m_pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &m_pDepthStencilView);

    // Bind Render Target and Depth Stencil
    m_pContext->OMSetRenderTargets(1u, m_pTargetView.GetAddressOf(), m_pDepthStencilView.Get());

    // Configure Viewport
    D3D11_VIEWPORT vp = {};
    vp.Width = static_cast<FLOAT>(width);
    vp.Height = static_cast<FLOAT>(height);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    m_pContext->RSSetViewports(1u, &vp);

    // Create Rasterizer State (Solid rendering with Culling)
    D3D11_RASTERIZER_DESC rd = {};
    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_BACK;
    rd.FrontCounterClockwise = FALSE;
    rd.DepthBias = 0;
    rd.SlopeScaledDepthBias = 0.0f;
    rd.DepthBiasClamp = 0.0f;
    rd.DepthClipEnable = TRUE;
    rd.ScissorEnable = FALSE;
    rd.MultisampleEnable = FALSE;
    rd.AntialiasedLineEnable = FALSE;

    m_pDevice->CreateRasterizerState(&rd, &m_pRasterizerState);
    m_pContext->RSSetState(m_pRasterizerState.Get());
}

void Graphics::BeginFrame(float red, float green, float blue) {
    const float color[] = { red, green, blue, 1.0f };
    m_pContext->ClearRenderTargetView(m_pTargetView.Get(), color);
    m_pContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Graphics::EndFrame() {
    m_pSwapChain->Present(1u, 0u);
}
