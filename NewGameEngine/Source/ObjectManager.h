#pragma once

#include <list>
#include <memory>
#include "SceneManager.h"
#include "SceneObject.h"


class ObjectManager
{
public:
	ObjectManager() {};
	void AddObject(std::shared_ptr<SceneObject> obj);		// ObjectFactory related method
	void SetLevelManager(std::shared_ptr<SceneManager> sm) { if(!sm) this->sm = std::move(sm); };
	void UpdateAll(Graphics& gfx, DirectX::XMMATRIX viewMatrix, float musParams[3]);						// Update all objects in the pObjectList
	void RenderAll(Graphics& gfx);										// Render all objects in the pObjectList
	void DeleteAll();										// Delete all objects in the pObjectList && pObjectCollidable
	void DeleteInactive();									// Delete all objects that became 'inactive'
	void InactivateAll();									// Inactivate all objects.
	virtual ~ObjectManager() {};
	void startScene(Graphics& gfx, std::string scene);
private:
	std::list<std::shared_ptr<SceneObject>> pObjectList;
	std::shared_ptr<SceneManager> sm;
};

