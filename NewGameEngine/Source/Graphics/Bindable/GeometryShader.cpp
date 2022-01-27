#include "GeometryShader.h"
#include "BindableCodex.h"

GeometryShader::GeometryShader(Graphics& gfx, const std::string& path)
	:
	path(path)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(std::wstring{ path.begin(),path.end() }.c_str(), &pBlob);
	GetDevice(gfx)->CreateGeometryShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pGeometryShader);
}

void GeometryShader::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->GSSetShader(pGeometryShader.Get(), nullptr, 0u);
}

std::shared_ptr<GeometryShader> GeometryShader::Resolve(Graphics& gfx, const std::string& path)
{
	return Codex::Resolve<GeometryShader>(gfx, path);
}

std::string GeometryShader::GenerateUID(const std::string& path)
{
	using namespace std::string_literals;
	return typeid(GeometryShader).name() + "#"s + path;
}

std::string GeometryShader::GetUID() const noexcept
{
	return GenerateUID(path);
}

