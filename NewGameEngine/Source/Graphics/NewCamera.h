// CODE was written with help by Jpres (https://www.youtube.com/watch?v=sROnNl_7Ex8)

#pragma once
#include <DirectXMath.h>
namespace dx = DirectX;
/// <summary>
/// Class that represents camera in the scene.
/// </summary>
class Camera
{
public:
	Camera();

	// Getters of View/Projection Matrices.
	const dx::XMMATRIX& GetViewMatrix() const;
	const dx::XMMATRIX& GetProjectionMatrix() const;

	// Getters for position and rotation vectors/arrays
	const dx::XMVECTOR& GetPositionVector() const;
	const dx::XMFLOAT3& GetPositionFloat3() const;
	const dx::XMVECTOR& GetRotationVector() const;
	const dx::XMFLOAT3& GetRotationFloat3() const;

	// Change parameters of the camera via public function.
	void SetPosition(const dx::XMVECTOR& pos);
	void SetPosition(float x, float y, float z);
	void AdjustPosition(const dx::XMVECTOR& pos);
	void AdjustPosition(float x, float y, float z);
	void SetRotation(const dx::XMVECTOR& rot);
	void SetRotation(float x, float y, float z);
	void AdjustRotation(const dx::XMVECTOR& rot);
	void AdjustRotation(float x, float y, float z);
	void SetLookAtPos(dx::XMFLOAT3 lookAtPos);
	const dx::XMVECTOR& GetForwardVector();
	const dx::XMVECTOR& GetRightVector();
	const dx::XMVECTOR& GetBackwardVector();
	const dx::XMVECTOR& GetLeftVector();
private:
	void UpdateViewMatrix();
	// member variables pos/rot in different representation 
	dx::XMVECTOR posVector;
	dx::XMVECTOR rotVector;
	dx::XMFLOAT3 pos;
	dx::XMFLOAT3 rot;
	// view and projection matrices.
	dx::XMMATRIX viewMatrix;
	dx::XMMATRIX projectionMatrix;


	// Directional vectors constant
	const dx::XMVECTOR DEFAULT_FORWARD_VECTOR = dx::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const dx::XMVECTOR DEFAULT_UP_VECTOR = dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const dx::XMVECTOR DEFAULT_BACKWARD_VECTOR = dx::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const dx::XMVECTOR DEFAULT_LEFT_VECTOR = dx::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const dx::XMVECTOR DEFAULT_RIGHT_VECTOR = dx::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);



	// Position of the directional vectors
	dx::XMVECTOR vec_forward;
	dx::XMVECTOR vec_left;
	dx::XMVECTOR vec_right;
	dx::XMVECTOR vec_backward;
};