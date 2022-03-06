/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)

#pragma once
#include "Bindable.h"
/// <summary>
/// Class that represents Input Layout resource. Input to shaders...
/// </summary>
class InputLayout : public Bindable
{
public:
	InputLayout(Graphics& gfx,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
		ID3DBlob* pVertexShaderBytecode);
	void Bind(Graphics& gfx) noexcept override;
	static std::shared_ptr<InputLayout> Resolve(Graphics& gfx,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode);
	static std::string GenerateUID(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode = nullptr);
	std::string GetUID() const noexcept override;
protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;									
	std::vector<D3D11_INPUT_ELEMENT_DESC> thisDesc;											// (D3D) Description of what parameters are passed to the shader pipeline.
};