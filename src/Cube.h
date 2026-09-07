#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <wrl/client.h>
#include "Graphics.h"
#include "MathUtils.h"

class Cube {
public:
    struct Vertex {
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT4 color;
    };

    struct ConstantBuffer {
        EngineMath::Matrix4x4 wvp;
    };

public:
    Cube(Graphics& gfx);
    ~Cube() = default;

    void Update(float dt);
    void Draw(Graphics& gfx);

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;

    float m_rotationX = 0.0f;
    float m_rotationY = 0.0f;
    float m_rotationZ = 0.0f;
    UINT m_indexCount = 0;
};
