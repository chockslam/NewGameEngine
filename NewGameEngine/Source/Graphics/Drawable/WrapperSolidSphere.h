#pragma once
#include "../Graphics.h"
#include "SolidSphere.h"
#include "../Bindable/ConstantBuffer.h"
#include "..\..\Window\AudioIO.h"

class WrapperSolidSphere
{
public:
	WrapperSolidSphere(Graphics& gfx, float radius, const char* vs, const char* ps, float pos[3]);
	void Reset() noexcept;
	void Draw(Graphics& gfx) const noexcept;
	void SetColor(DirectX::XMFLOAT3 color) noexcept;
	void SpawnControlWindow() noexcept;
	void SpawnTestFreqPlot() noexcept;
	void Bind(Graphics& gfx, DirectX::FXMMATRIX view, float musParams[3]) const noexcept;
private:
	struct SolidSphereCBuf
	{
		alignas(16) DirectX::XMFLOAT3 color;
	};

private:
	float initPos[3];

	DirectX::XMFLOAT3 pos;
	SolidSphereCBuf cbData;
	mutable SolidSphere mesh;
	mutable PixelConstantBuffer<SolidSphereCBuf> cbuf;
	mutable PixelConstantBuffer<SolidSphereCBuf> cbufV;
};