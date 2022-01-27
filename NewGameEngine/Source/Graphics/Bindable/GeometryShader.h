#pragma once
#include "Bindable.h"

class GeometryShader : public Bindable
{
public:
	GeometryShader(Graphics& gfx, const std::string& path);
	void Bind(Graphics& gfx) noexcept override;
	static std::shared_ptr<GeometryShader> Resolve(Graphics& gfx, const std::string& path);
	static std::string GenerateUID(const std::string& path);
	std::string GetUID() const noexcept override;
protected:
	std::string path;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> pGeometryShader;
};
