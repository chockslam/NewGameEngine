/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)

#pragma once
#include "ConstantBuffer.h"
#include "../Drawable/Drawable.h"
#include <DirectXMath.h>

/// <summary>
/// Class that represnts a special case of Constant Buffer resource. 
/// </summary>
class TransformCbuf : public Bindable
{
protected:
	/// <summary>
	/// Struct that updated and passed to the GS every frame.
	/// model view and modelViewProj transform are passed to every geometry shader.
	/// </summary>
	struct Transforms
	{
		DirectX::XMMATRIX modelViewProj;
		DirectX::XMMATRIX model;
	};
public:
	TransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot = 0);
	void Bind(Graphics& gfx) noexcept override;
protected:
	void UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcept;						// Updates Buffer and bind it to the pipeline.
	Transforms GetTransforms(Graphics& gfx) noexcept;
private:
	std::unique_ptr<GeometryConstantBuffer<Transforms>> pGcbuf;						
	const Drawable& parent;																	// Drawable that the shader is used by.
};
