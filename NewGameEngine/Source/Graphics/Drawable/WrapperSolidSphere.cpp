#include "WrapperSolidSphere.h"
#include <imgui.h>
#include <implot.h>

WrapperSolidSphere::WrapperSolidSphere(Graphics& gfx, float radius, const char* vs, const char* ps, float pos[3], const char* gs)
	:
	mesh(gfx, radius, vs, ps, gs),
	cbuf(gfx),
	cbufG(gfx, 1u)
{
	//cbufG = GeometryConstantBuffer::Reso
	this->initPos[0] = pos[0];
	this->initPos[1] = pos[1];
	this->initPos[2] = pos[2];
	Reset();
}

void WrapperSolidSphere::Reset() noexcept
{

	this->pos = { this->initPos[0], this->initPos[1], this->initPos[2] };
	cbData = {
		{ 0.4f,0.4f,0.4f },
	};
}


void WrapperSolidSphere::Draw(Graphics& gfx) const noexcept
{
	mesh.SetPos(this->pos);
	mesh.Draw(gfx);
}

void WrapperSolidSphere::SetColor(DirectX::XMFLOAT3 color) noexcept
{
}

void WrapperSolidSphere::SpawnControlWindow() noexcept
{

	SpawnTestFreqPlot();
}

void WrapperSolidSphere::SpawnTestFreqPlot() noexcept
{
	
}


void WrapperSolidSphere::Bind(Graphics& gfx, DirectX::FXMMATRIX view, float musParams[3]) const noexcept
{
	auto dataCopy = cbData;
	DirectX::XMVECTOR color = { musParams[0], musParams[1], musParams[2] };
	//const auto pos = DirectX::XMLoadFloat3(pos);
	DirectX::XMStoreFloat3(&dataCopy.color, color);
	//DirectX::XMStoreFloat3(&dataCopy.pos, DirectX::XMVector3Transform(pos, view));
	cbuf.Update(gfx, dataCopy);
	cbufG.Update(gfx, dataCopy);

	cbuf.Bind(gfx);
	cbufG.Bind(gfx);
}
