/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)

#pragma once
#include "../Graphics.h"
#include <DirectXMath.h>

class Bindable;

// Abstract Class that represents a drawable object.
class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;				// Transformation matrix of the object. Used by TransformCBuf object.
	void Draw(Graphics& gfx) const noexcept;									// Draw Object -> executed every frame.
	virtual ~Drawable() = default;
	template<class T>															// Query Bindable object from the vector of bindables. Need to modify resources in runtime. Not Used in the program.
	T* QueryBindable() noexcept
	{
		for (auto& pb : binds)
		{
			if (auto pt = dynamic_cast<T*>(pb.get()))
			{
				return pt;
			}
		}
		return nullptr;
	}
protected:
	void AddBind(std::shared_ptr<Bindable> bind) noexcept;						// Add Bindable to the vector of bindables
private:
	const class IndexBuffer* pIndexBuffer = nullptr;							// Index Buffer
	std::vector<std::shared_ptr<Bindable>> binds;
};