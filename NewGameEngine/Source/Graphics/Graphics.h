/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)

#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <random>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <implot.h>


/// <summary>
/// Class that represents D3D core graphics.
/// </summary>
class Graphics
{
	friend class Bindable;
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	void EndFrame();														
	void BeginFrame(float red, float green, float blue) noexcept;			// Called every frame in the beginning 	
	void DrawIndexed(UINT count) noexcept;									// Wrapper around pContext->DrawIndexed().
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;					
	DirectX::XMMATRIX GetProjection() const noexcept;						
	void SetCamera(DirectX::FXMMATRIX cam) noexcept;						// Set camera transform.
	DirectX::XMMATRIX GetCamera() const noexcept;							// Get camera transform.
private:
	void initImgui(HWND hwnd) noexcept;
private:
	DirectX::XMMATRIX projection;											// Matrix that represents projection.
	DirectX::XMMATRIX camera;												// Matrix that represent camera transform (used to display object relatively to cameras position (See class TransformCBuf)).

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;							// Represents a pointer to ID3D11Device, which is (usually) used to CREATE D3D objects and configure some D3D stuff.
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;							// Swap Chain. Swapping between back buffer and front buffer.
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;					// Context. Used to draw objects and manipulate D3D resources.
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;					// Target. Targets a specific platform. Windows in the case of this implememtaion.
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;					// Stencil view. Used as a Depth Checker, i.e. Z-buffer.
	ImGuiContext* imgCont;
	ImPlotContext* impCont;


};