/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)

#pragma once
#include "Bindable.h"
/// <summary>
/// Class represents Topology D3D resource. Topology means how D3D processes geometry. (Points/Line/Triangles/Quads)
/// </summary>
class Topology : public Bindable
{
public:
	Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
	void Bind(Graphics& gfx)  override;
	static std::shared_ptr<Topology> Resolve(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
	static std::string GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type);
	std::string GetUID() const  override;
protected:
	D3D11_PRIMITIVE_TOPOLOGY type;
};