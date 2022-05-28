/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)
#include "TransformCbuf.h"


TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot)
	:parent(parent)
{
	if (!pGcbuf)
	{
		pGcbuf = std::make_unique<GeometryConstantBuffer<Transforms>>(gfx);
	}
}

void TransformCbuf::Bind(Graphics& gfx) 
{
	UpdateBindImpl(gfx, GetTransforms(gfx));
}

void TransformCbuf::UpdateBindImpl(Graphics& gfx, const Transforms& tf) 
{
	//Update and bind transformCBuf every frame.
	pGcbuf->Update(gfx, tf);
	pGcbuf->Bind(gfx);
}

TransformCbuf::Transforms TransformCbuf::GetTransforms(Graphics& gfx) 
{
	const auto modelView = parent.GetTransformXM() * gfx.GetCamera();// Calculate modelView matrix
	// Return modelView and modelViewProjection matrices.
	return {
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(
			modelView *
			gfx.GetProjection()
		)
	};
}


//std::unique_ptr<GeometryConstantBuffer<TransformCbuf::Transforms>> TransformCbuf::pGcbuf;