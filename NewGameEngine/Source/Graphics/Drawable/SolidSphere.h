#pragma once
#include "Drawable.h"

class SolidSphere : public Drawable
{
public:
	SolidSphere(Graphics& gfx, float radius, const char* vs, const char* ps, const char* gs = nullptr);
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	DirectX::XMFLOAT3 pos = { 1.0f,1.0f,1.0f };
	/*struct PSColorConstant
	{
		DirectX::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
		float padding;
	} colorConst;
	*/
};