#include "TransformCbuf.h"


TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot)
	:parent(parent)
{
	if (!pGcbuf)
	{
		pGcbuf = std::make_unique<GeometryConstantBuffer<Transforms>>(gfx);
	}
}

void TransformCbuf::Bind(Graphics& gfx) noexcept
{
	UpdateBindImpl(gfx, GetTransforms(gfx));
}

void TransformCbuf::UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcept
{
	pGcbuf->Update(gfx, tf);
	pGcbuf->Bind(gfx);
}

TransformCbuf::Transforms TransformCbuf::GetTransforms(Graphics& gfx) noexcept
{
	const auto modelView = parent.GetTransformXM() * gfx.GetCamera();
	return {
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(
			modelView *
			gfx.GetProjection()
		)
	};
}


std::unique_ptr<GeometryConstantBuffer<TransformCbuf::Transforms>> TransformCbuf::pGcbuf;