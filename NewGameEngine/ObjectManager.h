#pragma once

#include <list>
#include <memory>
#include "SceneManager.h"

class SceneObject;

class ObjectManager
{
	public:
		ObjectManager() {};
		// Level Manager getter.
		//std::shared_ptr<LevelManager> getLevelManager() const { return this->lm; };

		void AddObject(std::shared_ptr<SceneObject> obj); // ObjectFactory related method
		void UpdateAll(float frameTime);						// Update all objects in the pObjectList
		void RenderAll();										// Render all objects in the pObjectList
		void DeleteAll();										// Delete all objects in the pObjectList && pObjectCollidable
		void DeleteInactive();									// Delete all objects that became 'inactive'
		void InactivateAll();									// Inactivate all objects.
		//void CheckCollisions();
		//void checkOtherInteraction(std::wstring objType1, std::wstring objType2, float threshold);
		//template<class T> std::shared_ptr<T> QueryObject();
		//template<class T> std::list<std::shared_ptr<T>> QueryObjectList();
		~ObjectManager();
	private:
		// shared pointer to LevelManager
		// std::shared_ptr<LevelManager> lm;
		// List of ALL gameObjects to be updated and rendered.
		std::list<std::shared_ptr<SceneObject>> pObjectList;
		std::list<SceneManager> sm;
};

