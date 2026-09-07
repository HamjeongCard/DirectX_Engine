#pragma once

#include <windows.h>
#include <d3d11.h>
#include <wrl/client.h>

class Graphics {
public:
    Graphics(HWND hWnd, int width, int height);
    ~Graphics() = default;

    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;

    void BeginFrame(float red, float green, float blue);
    void EndFrame();

    ID3D11Device* GetDevice() { return m_pDevice.Get(); }
    ID3D11DeviceContext* GetContext() { return m_pContext.Get(); }

private:
    Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pRasterizerState;
};
