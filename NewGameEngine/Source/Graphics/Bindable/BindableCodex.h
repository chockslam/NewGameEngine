/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)
#pragma once
#include "Bindable.h"
#include "BindableCodex.h"
#include <type_traits>
#include <memory>
#include <unordered_map>


/// <summary>
/// Bindable Codex implementation. A pool of all D3D resource that can be identified by the unique Identifier (See Bindable::GetUID()).
/// Uses Singleton Design Pattern.
/// </summary>
class Codex
{
public:
	static void ResetAll() {
		auto& c = Get();
		for (auto& bind : c.binds) {
			bind.second.reset();
		}
	}
	// Public Template function that resolves Bindable using its ID
	template<class T, typename...Params>
	static std::shared_ptr<T> Resolve(Graphics& gfx, Params&&...p)
	{
		static_assert(std::is_base_of<Bindable, T>::value, "Can only resolve classes derived from Bindable");
		return Get().Resolve_<T>(gfx, std::forward<Params>(p)...);
	}
private:
	// Private Implementation of Resolve function
	template<class T, typename...Params>
	std::shared_ptr<T> Resolve_(Graphics& gfx, Params&&...p)
	{
		const auto key = T::GenerateUID(std::forward<Params>(p)...);					// Generates ID based on Construct parameters
		const auto i = binds.find(key);													// Check whether the resource is in the pool (map).
		if (i == binds.end())															// If it is not there, make one, push it to the map and return
		{
			auto bind = std::make_shared<T>(gfx, std::forward<Params>(p)...);
			binds[key] = bind;
			return bind;
		}
		else																			// If it is there, return resource
		{
			return std::static_pointer_cast<T>(i->second);
		}
	}
	static Codex& Get()																	// Return singleton
	{
		static Codex codex;
		return codex;
	}
private:
	std::unordered_map<std::string, std::shared_ptr<Bindable>> binds;					// Map of all bindables
};