/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)
#include "Bindable.h"
/// <summary>
///  Get raw pointer to the content from graphics reference
/// </summary>
/// <param name="gfx">reference to graphics object</param>
ID3D11DeviceContext* Bindable::GetContext(Graphics& gfx) 
{
	return gfx.pContext.Get(); 
}
/// <summary>
///  Get raw pointer to the device from graphics reference
/// </summary>
/// <param name="gfx">reference to graphics object</param>
ID3D11Device* Bindable::GetDevice(Graphics& gfx) 
{
	return gfx.pDevice.Get(); 
}
