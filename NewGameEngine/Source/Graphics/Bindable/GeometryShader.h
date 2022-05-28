/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)

#pragma once
#include "Bindable.h"
/// <summary>
/// Class represents Geometry Shader resource.
/// </summary>
class GeometryShader : public Bindable
{
public:
	GeometryShader(Graphics& gfx, const std::string& path);
	// Bind function. Invoked every frame.
	void Bind(Graphics& gfx)  override;
	// Bindable Codex stuff
	static std::shared_ptr<GeometryShader> Resolve(Graphics& gfx, const std::string& path);
	static std::string GenerateUID(const std::string& path);
	std::string GetUID() const  override;
protected:
	std::string path;														// Path to the shader (.cso)
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> pGeometryShader;			// pointer to D3D resource
};
