#include "D3D11GPUBuffer.h"

namespace Soul
{
	D3D11GPUBuffer::D3D11GPUBuffer(D3D11Device& device, enum class GPU_BUFFER_TYPE gpuBufferType, const void* mem, unsigned byteSize)
		:
		mDevice(device)
	{
		D3D11_BUFFER_DESC dbd;
		ZeroMemory(&dbd, sizeof(dbd));
		switch (gpuBufferType)
		{
		case GPU_BUFFER_TYPE::GBT_VERTEX:
			//Create VertexBuffer
			dbd.Usage = D3D11_USAGE_IMMUTABLE;
			dbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			dbd.ByteWidth = byteSize;
			break;
		case GPU_BUFFER_TYPE::GBT_INDEX:
			//Create IndexBuffer
			dbd.Usage = D3D11_USAGE_IMMUTABLE;
			dbd.ByteWidth = byteSize;
			dbd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			break;
		case GPU_BUFFER_TYPE::GBT_CONST:
			dbd.Usage = D3D11_USAGE_DYNAMIC;
			dbd.ByteWidth = byteSize;
			dbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			dbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		default:
			break;
		}
		HRESULT hr;
		if (mem)
		{
			D3D11_SUBRESOURCE_DATA initData;
			ZeroMemory(&initData, sizeof(initData));
			initData.pSysMem = mem;
			hr = mDevice->CreateBuffer(&dbd, &initData, mD3D11Buffer.ReleaseAndGetAddressOf());
		}
		else
		{
			hr = mDevice->CreateBuffer(&dbd, nullptr, mD3D11Buffer.ReleaseAndGetAddressOf());
		}
		if (FAILED(hr))
		{

		}
	}
}