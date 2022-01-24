#include "SolidSphere.h"
#include "../Bindable/BindableBase.h"
#include "Sphere.h"


SolidSphere::SolidSphere(Graphics& gfx, float radius, const char* vs, const char* ps)
{
	
	namespace dx = DirectX;

	auto model = Sphere::Make();
	model.Transform(dx::XMMatrixScaling(radius, radius, radius));
	//AddBind(std::make_unique<VertexBuffer>(gfx, model.vertices));
	AddBind(VertexBuffer::Resolve(gfx, "sphere_vertices" , model.vertices));
	//AddBind(std::make_shared<IndexBuffer>(gfx, model.indices));
	AddBind(IndexBuffer::Resolve(gfx,"sphere_indices", model.indices));

	//auto pvs = std::make_shared<VertexShader>(gfx, L"SolidVS.cso");
	auto pvs = VertexShader::Resolve(gfx, vs);
	auto pvsbc = static_cast<VertexShader&>(*pvs).GetBytecode();
	AddBind(std::move(pvs));

	//AddBind(std::make_shared<PixelShader>(gfx, L"SolidPS.cso"));
	AddBind(PixelShader::Resolve(gfx, ps));

	
	//AddBind(std::make_shared<PixelConstantBuffer<PSColorConstant>>(gfx, colorConst));
	//AddBind(PixelConstantBuffer<PSColorConstant>::Resolve(gfx, colorConst));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	AddBind(InputLayout::Resolve(gfx, ied, pvsbc));

	AddBind(Topology::Resolve( gfx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ));

	AddBind(std::make_shared<TransformCbuf>(gfx, *this));
}

void SolidSphere::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	this->pos = pos;
}


DirectX::XMMATRIX SolidSphere::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}
