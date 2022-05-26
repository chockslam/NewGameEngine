#pragma once

#include <string>
#include <list>
#include <memory>
#include "Source/Graphics/Graphics.h"
#include "Source/Graphics/Drawable/SolidSphere.h"
#include "Source/Graphics/Bindable/ConstantBuffer.h"

class SceneObject
{
public:
	SceneObject(Graphics& gfx, float radius, int latDiv, int longDiv, const char* vs, const char* ps, float pos[3], const char* gs = nullptr);
	virtual ~SceneObject();
public:
	SceneObject(Graphics& gfx, float radius, int latDiv, int longDiv, const char* vs, const char* ps, float pos[3], const char* gs = nullptr);
	virtual ~SceneObject();
	void Reset() noexcept;
	void Draw(Graphics& gfx) const noexcept;
	void Bind(Graphics& gfx, DirectX::FXMMATRIX view, float musParams[3]) const noexcept;
private:
	/// <summary>
	/// Pixel shader constant buffer.
	/// /color/ - contains music data from FFT (x = Bass, y = Mid-Range, z = Treble) 
	/// </summary>
	struct SolidSphereCBuf
	{
		alignas(16) DirectX::XMFLOAT3 color;
	};

private:
	float initPos[3];

	DirectX::XMFLOAT3 pos;											// position of the object
	SolidSphereCBuf cbData;											// Struct object declared above.
	mutable SolidSphere mesh;
	mutable PixelConstantBuffer<SolidSphereCBuf> cbuf;				// Pixel Constant buffer that uses structure declared above as a data.
	mutable GeometryConstantBuffer<SolidSphereCBuf> cbufG;			// Geometry Constant buffer that uses structure declared above as a data.


};

