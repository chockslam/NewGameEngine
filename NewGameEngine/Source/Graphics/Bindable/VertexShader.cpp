/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)
#include "VertexShader.h"
#include "BindableCodex.h"
#include <typeinfo>
using namespace std::string_literals;

VertexShader::VertexShader(Graphics& gfx, const std::string& path)
	:
	path(path)
{
	//read .cso file
	D3DReadFileToBlob(std::wstring{ path.begin(),path.end() }.c_str(), &pBytecodeBlob);
	GetDevice(gfx)->CreateVertexShader(
		pBytecodeBlob->GetBufferPointer(),
		pBytecodeBlob->GetBufferSize(),
		nullptr,
		&pVertexShader
	);
}

void VertexShader::Bind(Graphics& gfx) 
{
	GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetBytecode() const 
{
	return pBytecodeBlob.Get();									// get byte code of the .cso file
}

std::shared_ptr<VertexShader> VertexShader::Resolve(Graphics& gfx, const std::string& path)
{
	return Codex::Resolve<VertexShader>(gfx, path);
}

std::string VertexShader::GenerateUID(const std::string& path)
{
	using namespace std::string_literals;
	return typeid(VertexShader).name() + "#"s + path;
}

std::string VertexShader::GetUID() const  
{
	return GenerateUID(path);
}
