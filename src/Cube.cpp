#include "Cube.h"
#include <d3dcompiler.h>
#include <stdexcept>

const char* shaderCode = R"(
cbuffer ConstantBuffer : register(b0) {
    matrix wvp;
};

struct VS_INPUT {
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct PS_INPUT {
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

PS_INPUT VS(VS_INPUT input) {
    PS_INPUT output;
    output.pos = mul(float4(input.pos, 1.0f), wvp);
    output.color = input.color;
    return output;
}

float4 PS(PS_INPUT input) : SV_TARGET {
    return input.color;
}
)";

Cube::Cube(Graphics& gfx) {
    // 8 Vertices of a Cube with distinct colors for each face/vertex
    const Vertex vertices[] = {
        { DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }, // 0: Red
        { DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // 1: Green
        { DirectX::XMFLOAT3( 1.0f,  1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }, // 2: Blue
        { DirectX::XMFLOAT3( 1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) }, // 3: Yellow
        { DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) }, // 4: Magenta
        { DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }, // 5: Cyan
        { DirectX::XMFLOAT3( 1.0f,  1.0f,  1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }, // 6: White
        { DirectX::XMFLOAT3( 1.0f, -1.0f,  1.0f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f) }, // 7: Gray
    };

    D3D11_BUFFER_DESC vbd = {};
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.Usage = D3D11_USAGE_DEFAULT;
    vbd.CPUAccessFlags = 0u;
    vbd.MiscFlags = 0u;
    vbd.ByteWidth = sizeof(vertices);
    vbd.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA vsd = {};
    vsd.pSysMem = vertices;

    HRESULT hr = gfx.GetDevice()->CreateBuffer(&vbd, &vsd, &m_pVertexBuffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create vertex buffer");
    }

    // 36 Indices for 12 triangles (6 faces)
    const WORD indices[] = {
        // Front Face
        0, 1, 2,
        0, 2, 3,
        // Back Face
        4, 6, 5,
        4, 7, 6,
        // Left Face
        4, 5, 1,
        4, 1, 0,
        // Right Face
        3, 2, 6,
        3, 6, 7,
        // Top Face
        1, 5, 6,
        1, 6, 2,
        // Bottom Face
        4, 0, 3,
        4, 3, 7
    };

    m_indexCount = static_cast<UINT>(sizeof(indices) / sizeof(WORD));

    D3D11_BUFFER_DESC ibd = {};
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.CPUAccessFlags = 0u;
    ibd.MiscFlags = 0u;
    ibd.ByteWidth = sizeof(indices);
    ibd.StructureByteStride = sizeof(WORD);

    D3D11_SUBRESOURCE_DATA isd = {};
    isd.pSysMem = indices;

    hr = gfx.GetDevice()->CreateBuffer(&ibd, &isd, &m_pIndexBuffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create index buffer");
    }

    // Constant Buffer for Matrix
    D3D11_BUFFER_DESC cbd = {};
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbd.MiscFlags = 0u;
    cbd.ByteWidth = sizeof(ConstantBuffer);
    cbd.StructureByteStride = 0u;

    hr = gfx.GetDevice()->CreateBuffer(&cbd, nullptr, &m_pConstantBuffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create constant buffer");
    }

    // Compile Shaders
    Microsoft::WRL::ComPtr<ID3DBlob> pVSBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> pErrBlob;
    hr = D3DCompile(shaderCode, strlen(shaderCode), nullptr, nullptr, nullptr, "VS", "vs_4_0", 0, 0, &pVSBlob, &pErrBlob);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to compile Vertex Shader");
    }

    hr = gfx.GetDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create Vertex Shader");
    }

    Microsoft::WRL::ComPtr<ID3DBlob> pPSBlob;
    hr = D3DCompile(shaderCode, strlen(shaderCode), nullptr, nullptr, nullptr, "PS", "ps_4_0", 0, 0, &pPSBlob, &pErrBlob);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to compile Pixel Shader");
    }

    hr = gfx.GetDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create Pixel Shader");
    }

    // Input Layout
    const D3D11_INPUT_ELEMENT_DESC ied[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = gfx.GetDevice()->CreateInputLayout(
        ied, static_cast<UINT>(sizeof(ied) / sizeof(D3D11_INPUT_ELEMENT_DESC)),
        pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(),
        &m_pInputLayout
    );

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create Input Layout");
    }
}

void Cube::Update(float dt) {
    m_rotationX += dt * 1.0f;
    m_rotationY += dt * 1.2f;
    m_rotationZ += dt * 0.8f;
}

void Cube::Draw(Graphics& gfx) {
    // Calculate World Matrix (Rotation)
    EngineMath::Matrix4x4 world = EngineMath::Matrix4x4::RotationRollPitchYaw(m_rotationX, m_rotationY, m_rotationZ);

    // Calculate View Matrix (Camera positioned at z = -4.0f)
    EngineMath::Vector3 eye(0.0f, 0.0f, -4.0f);
    EngineMath::Vector3 at(0.0f, 0.0f, 0.0f);
    EngineMath::Vector3 up(0.0f, 1.0f, 0.0f);
    EngineMath::Matrix4x4 view = EngineMath::Matrix4x4::LookAtLH(eye, at, up);

    // Calculate Projection Matrix (Perspective)
    float fovRad = 60.0f * (3.1415926535f / 180.0f);
    EngineMath::Matrix4x4 proj = EngineMath::Matrix4x4::PerspectiveFovLH(fovRad, 800.0f / 600.0f, 0.1f, 100.0f);

    // Combine matrices: HLSL expects transposed WVP matrix when using mul(pos, wvp)
    EngineMath::Matrix4x4 wv = EngineMath::Matrix4x4::Multiply(world, view);
    EngineMath::Matrix4x4 wvp = EngineMath::Matrix4x4::Multiply(wv, proj);
    EngineMath::Matrix4x4 wvpTransposed = EngineMath::Matrix4x4::Transpose(wvp);

    // Update Constant Buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    gfx.GetContext()->Map(m_pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &ms);
    ConstantBuffer* pCb = reinterpret_cast<ConstantBuffer*>(ms.pData);
    pCb->wvp = wvpTransposed;
    gfx.GetContext()->Unmap(m_pConstantBuffer.Get(), 0u);

    // Bind Pipeline Components
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    gfx.GetContext()->IASetVertexBuffers(0u, 1u, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
    gfx.GetContext()->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
    gfx.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    gfx.GetContext()->IASetInputLayout(m_pInputLayout.Get());

    gfx.GetContext()->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
    gfx.GetContext()->VSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());

    gfx.GetContext()->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);

    // Draw Indexed Triangles
    gfx.GetContext()->DrawIndexed(m_indexCount, 0u, 0u);
}
