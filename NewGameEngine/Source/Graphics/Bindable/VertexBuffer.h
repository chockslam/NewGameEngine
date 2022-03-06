/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)

#pragma once
#include "Bindable.h"

/// <summary>
/// Vertex layout contains position and normal.
/// </summary>
struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 n;
};

/// <summary>
/// Class that represents Vertex Buffer D3D resoruce.
/// </summary>
class VertexBuffer : public Bindable
{
public:
	VertexBuffer(Graphics& gfx, const std::string& tag, const std::vector<Vertex>& vertices);
	VertexBuffer(Graphics& gfx, const std::vector<Vertex>& vertices);
	void Bind(Graphics& gfx) noexcept override;
	static std::shared_ptr<VertexBuffer> Resolve(Graphics& gfx, const std::string& tag,
		const std::vector<Vertex>& vertices);
	template<typename...Ignore>
	static std::string GenerateUID(const std::string& tag, Ignore&&...ignore)
	{
		return GenerateUID_(tag);
	}
	std::string GetUID() const noexcept override;
private:
	static std::string GenerateUID_(const std::string& tag);
protected:
	
	std::string tag;											// tag uniquely identifies an object in the Bindable Codex
	UINT stride;												// Size of the elements in the vertex buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};
