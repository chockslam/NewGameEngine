#pragma once
#include "Graphics.h"

namespace dx = DirectX;

class Camera
{
public:
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void Reset() noexcept;
	void MoveF() noexcept;
	void MoveB() noexcept;
	void MoveL() noexcept;
	void MoveR() noexcept;
private:
	float r = 20.0f; //distance from the origin
	float theta = 0.0f; // rotating the camera around the origin (around equator)
	float phi = 0.0f;  // rotating the camera around the northpole and south pole.
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
};