/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)

#pragma once
#include "Drawable.h"

/// <summary>
/// Concrete implementation of Drawable - Sphere.
/// </summary>
class SolidSphere : public Drawable
{
public:
	SolidSphere(Graphics& gfx, float radius, int latDiv, int longDiv, const char* vs, const char* ps, const char* gs = nullptr);
	//void SetPos(DirectX::XMFLOAT3 pos) ;																						// set position for the sphere
	DirectX::XMMATRIX GetTransformXM() const  override;																			// Used by TransformCBuf to get Transformation Matrix.
	//virtual void SetPos(DirectX::XMFLOAT3 pos)  override;
};
