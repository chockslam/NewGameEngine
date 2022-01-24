#include "InputLayout.h"
#include "BindableCodex.h"

InputLayout::InputLayout(Graphics& gfx,
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
	ID3DBlob* pVertexShaderBytecode)
{

	thisDesc = layout;

	GetDevice(gfx)->CreateInputLayout(
		layout.data(), (UINT)layout.size(),
		pVertexShaderBytecode->GetBufferPointer(),
		pVertexShaderBytecode->GetBufferSize(),
		&pInputLayout
	);
}
void InputLayout::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
}

std::shared_ptr<InputLayout> InputLayout::Resolve(Graphics& gfx,
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode)
{
	//std::vector<D3D11_INPUT_ELEMENT_DESC> thisDesc = layout;
	return Codex::Resolve<InputLayout>(gfx, layout, pVertexShaderBytecode);;
}

std::string InputLayout::GenerateUID(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode)
{
	using namespace std::string_literals;

	std::string str = "";
	for (auto elem : layout) {
		str += elem.SemanticName+elem.Format;
	}
	return typeid(InputLayout).name() + "#"s + str;
}

std::string InputLayout::GetUID() const noexcept
{
	return GenerateUID(thisDesc);
}