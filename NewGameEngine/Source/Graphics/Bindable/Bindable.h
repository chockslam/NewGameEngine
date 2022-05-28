/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)
#pragma once
#include "../Graphics.h"
#include <string>
/// <summary>
/// Abstract class represents D3D resource.
/// Needs to be extended by concrete implementation.
/// </summary>
class Bindable
{
public:
	virtual void Bind(Graphics& gfx)  = 0;                // Abstract function which concrete implementation should bind a resource to the graphics pipeline.
	virtual ~Bindable() = default;								  
	virtual std::string GetUID() const  {				  // Needs for Implementation of Bindable Codex.
		assert(false);
		return "";
	}
protected:
	static ID3D11DeviceContext* GetContext(Graphics& gfx) ;			// Returns a pointer to the Context (D3D resource that is responsible for talking to DirectX)
	static ID3D11Device* GetDevice(Graphics& gfx) ;					// Returns a pointer to the representation of the device.
};