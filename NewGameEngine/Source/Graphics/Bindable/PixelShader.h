/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)
#pragma once
#include "Bindable.h"
/// <summary>
/// Class that represents Pixel Shader.
/// </summary>
class PixelShader : public Bindable
{
public:
	PixelShader(Graphics& gfx, const std::string& path);
	void Bind(Graphics& gfx)  override;
	static std::shared_ptr<PixelShader> Resolve(Graphics& gfx, const std::string& path);
	static std::string GenerateUID(const std::string& path);
	std::string GetUID() const  override;
protected:
	std::string path;													// path to .cso file
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};