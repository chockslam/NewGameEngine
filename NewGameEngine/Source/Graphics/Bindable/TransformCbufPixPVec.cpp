#include "TransformCbufPixPVec.h"


TransformCbufDoubleboi::TransformCbufDoubleboi(Graphics& gfx, const Drawable& parent, UINT slotV, UINT slotP)
	:
	TransformCbuf(gfx, parent, slotV)
{
	pPcbuf = std::make_unique<PixelConstantBuffer<Transforms>>(gfx, slotP);
}

void TransformCbufDoubleboi::Bind(Graphics& gfx) noexcept
{
	const auto tf = GetTransforms(gfx);
	TransformCbuf::UpdateBindImpl(gfx, tf);
	UpdateBindImpl(gfx, tf);
}

void TransformCbufDoubleboi::UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcept
{
	pPcbuf->Update(gfx, tf);
	pPcbuf->Bind(gfx);
}

std::unique_ptr<PixelConstantBuffer<TransformCbuf::Transforms>> TransformCbufDoubleboi::pPcbuf;