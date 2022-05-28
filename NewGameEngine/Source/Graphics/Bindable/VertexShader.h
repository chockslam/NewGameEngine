/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)

#pragma once
#include "Bindable.h"

/// <summary>
/// Class that represnts Vertex Shader
/// </summary>
class VertexShader : public Bindable
{
public:
	VertexShader(Graphics& gfx, const std::string& path);
	void Bind(Graphics& gfx)  override;
	ID3DBlob* GetBytecode() const ;
	static std::shared_ptr<VertexShader> Resolve(Graphics& gfx, const std::string& path);
	static std::string GenerateUID(const std::string& path);
	std::string GetUID() const  override;
protected:
	std::string path;
	Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;					// Bytecode of the shader needed in the constructor of Input layout.
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
};