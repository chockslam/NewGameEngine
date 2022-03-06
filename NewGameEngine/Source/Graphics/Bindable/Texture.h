/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)

#pragma once
#include "Bindable.h"

class Surface;
/// <summary>
/// Class that represents Texture D3D resource. Used bi GUI to draw .png picture in there.
/// </summary>
class Texture : public Bindable
{
public:
	Texture(Graphics& gfx, const std::string& path, unsigned int slot = 0);
	void Bind(Graphics& gfx) noexcept override;
	static std::shared_ptr<Texture> Resolve(Graphics& gfx, const std::string& path, UINT slot = 0);
	static std::string GenerateUID(const std::string& path, UINT slot = 0);
	std::string GetUID() const noexcept override;
	bool HasAlpha() const noexcept;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTexture() const noexcept;
private:
	unsigned int slot;
protected:
	bool hasAlpha = false;													// Checks whether the array of pixels (loaded pictures) has alpha value in the A channel (RGBA format) 
	std::string path;														// Path to the picture.
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;			
};