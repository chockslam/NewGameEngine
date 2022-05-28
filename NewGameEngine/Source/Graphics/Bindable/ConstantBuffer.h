/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)

#pragma once
#include "Bindable.h"
#include "BindableCodex.h"

/// <summary>
/// Class that represent constant Buffer (not used directly code but acts as an abstract class)
/// </summary>
/// <typeparam name="C"> Constant Buffer parameters - struct </typeparam>
template<typename C>
class ConstantBuffer : public Bindable
{
public:
	void Update(Graphics& gfx, const C& consts)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		GetContext(gfx)->Map(
			pConstantBuffer.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&msr
		);
		memcpy(msr.pData, &consts, sizeof(consts));
		GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0u);
	}
	ConstantBuffer()
	{
	}
	ConstantBuffer(Graphics& gfx, UINT slot = 0u)
		:slot(slot)
	{
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;
		GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &pConstantBuffer);
	}
	ConstantBuffer(Graphics& gfx, const C& consts, UINT slot = 0u)
		:slot(slot)
	{

		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		GetDevice(gfx)->CreateBuffer(&cbd, &csd, &pConstantBuffer);
	}
	

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	UINT slot;
};

/// <summary>
/// Class that represents vertex constant buffer
/// </summary>
/// <typeparam name="C"> Constant Buffer parameters - struct </typeparam>
template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
	using Bindable::GetContext;

public:
	using ConstantBuffer<C>::ConstantBuffer;
	// Binding to the pipeline
	void Bind(Graphics& gfx)  override
	{
		GetContext(gfx)->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
	}
	// Bindable codex implementation stuff... 
	static std::shared_ptr<VertexConstantBuffer> Resolve(Graphics& gfx, const C& consts, UINT slot = 0)
	{
		return Codex::Resolve<VertexConstantBuffer>(gfx, consts, slot);
	}
	static std::shared_ptr<VertexConstantBuffer> Resolve(Graphics& gfx, UINT slot = 0)
	{
		return Codex::Resolve<VertexConstantBuffer>(gfx, slot);
	}
	static std::string GenerateUID(const C&, UINT slot)
	{
		return GenerateUID(slot);
	}
	static std::string GenerateUID(UINT slot = 0)
	{
		using namespace std::string_literals;
		return typeid(VertexConstantBuffer).name() + "#"s + std::to_string(slot);
	}
	std::string GetUID() const  override
	{
		return GenerateUID(slot);
	}
};


/// <summary>
/// Class that represents geometry constant buffer. Similar to vertex constant buffer
/// </summary>
/// <typeparam name="C"> Constant Buffer parameters - struct </typeparam>
template<typename C>
class GeometryConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
	using Bindable::GetContext;

public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gfx)  override
	{
		GetContext(gfx)->GSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
	}
	static std::shared_ptr<GeometryConstantBuffer> Resolve(Graphics& gfx, const C& consts, UINT slot = 0)
	{
		return Codex::Resolve<GeometryConstantBuffer>(gfx, consts, slot);
	}
	static std::shared_ptr<GeometryConstantBuffer> Resolve(Graphics& gfx, UINT slot = 0)
	{
		return Codex::Resolve<GeometryConstantBuffer>(gfx, slot);
	}
	static std::string GenerateUID(const C&, UINT slot)
	{
		return GenerateUID(slot);
	}
	static std::string GenerateUID(UINT slot = 0)
	{
		using namespace std::string_literals;
		return typeid(GeometryConstantBuffer).name() + "#"s + std::to_string(slot);
	}
	std::string GetUID() const  override
	{
		return GenerateUID(slot);
	}
};



/// <summary>
/// Class that represents pixel constant buffer. Similar to vertex constant buffer
/// </summary>
/// <typeparam name="C"> Constant Buffer parameters - struct </typeparam>

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gfx)  override
	{
		GetContext(gfx)->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
	}
	static std::shared_ptr<PixelConstantBuffer> Resolve(Graphics& gfx, const C& consts, UINT slot = 0)
	{
		return Codex::Resolve<PixelConstantBuffer>(gfx, consts, slot);
	}
	static std::shared_ptr<PixelConstantBuffer> Resolve(Graphics& gfx, UINT slot = 0)
	{
		return Codex::Resolve<PixelConstantBuffer>(gfx, slot);
	}
	static std::string GenerateUID(const C&, UINT slot)
	{
		return GenerateUID(slot);
	}
	static std::string GenerateUID(UINT slot = 0)
	{
		using namespace std::string_literals;
		return typeid(PixelConstantBuffer).name() + "#"s + std::to_string(slot);
	}
	std::string GetUID() const  override
	{
		return GenerateUID(slot);
	}
};