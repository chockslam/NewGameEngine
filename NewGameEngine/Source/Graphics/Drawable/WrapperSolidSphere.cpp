#include "WrapperSolidSphere.h"
#include <imgui.h>
#include <implot.h>

WrapperSolidSphere::WrapperSolidSphere(Graphics& gfx, float radius, int latDiv, int longDiv, const char* vs, const char* ps, float pos[3], const char* gs)
	:
	mesh(gfx, radius,latDiv,longDiv, vs, ps, gs),
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
	// Set new position and draw
	mesh.SetPos(this->pos);
	mesh.Draw(gfx);
}



void WrapperSolidSphere::Bind(Graphics& gfx, DirectX::FXMMATRIX view, float musParams[3]) const noexcept
{
	auto dataCopy = cbData;
	DirectX::XMVECTOR color = { musParams[0], musParams[1], musParams[2] };
	//const auto pos = DirectX::XMLoadFloat3(pos);
	DirectX::XMStoreFloat3(&dataCopy.color, color);

	// Update pixel constant buffer amd geometry constant buffer with audio data.
	cbuf.Update(gfx, dataCopy);
	cbufG.Update(gfx, dataCopy);

	// Bind constant buffers to the pipeline
	cbuf.Bind(gfx);
	cbufG.Bind(gfx);
}
