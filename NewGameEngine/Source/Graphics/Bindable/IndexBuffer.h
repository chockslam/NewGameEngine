/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)

#pragma once
#include "Bindable.h"

/// <summary>
/// Class represents Index Buffer resource.
/// </summary>
class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices);
	IndexBuffer(Graphics& gfx, std::string tag, const std::vector<unsigned short>& indices);
	void Bind(Graphics& gfx) noexcept override;
	UINT GetCount() const noexcept;

	static std::shared_ptr<IndexBuffer> Resolve(Graphics& gfx, const std::string& tag,
		const std::vector<unsigned short>& indices);
	template<typename...Ignore>
	static std::string GenerateUID(const std::string& tag, Ignore&&...ignore)
	{
		return GenerateUID_(tag);
	}
	std::string GetUID() const noexcept override;
private:
	static std::string GenerateUID_(const std::string& tag);
protected:
	std::string tag;												// tag is used to uniquely identify a resource in the Bindable Codex.
	UINT count;														// Number of indices.
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
};