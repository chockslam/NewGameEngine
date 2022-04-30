/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)

#pragma once
#include "../Graphics.h"
#include "SolidSphere.h"
#include "../Bindable/ConstantBuffer.h"
#include "..\..\Window\AudioIO.h"

/// <summary>
/// Wrapper around a standard SolidSphere to make manipulation with the object easier.
/// </summary>
class WrapperSolidSphere
{
public:
	WrapperSolidSphere(Graphics& gfx, float radius,int latDiv,int longDiv, const char* vs, const char* ps, float pos[3], const char* gs = nullptr);
	~WrapperSolidSphere();
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