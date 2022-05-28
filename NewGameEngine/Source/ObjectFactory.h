#pragma once
#include <memory>
#include "ObjectManager.h"
class ObjectFactory
{
public:
    static ObjectFactory& getInstance()
    {
        static ObjectFactory    instance; // Guaranteed to be destroyed.
                              // Instantiated on first use.
        return instance;
    }
private:
    ObjectFactory() {}                // Constructor? (the {} brackets) are needed here.

    // C++ 11
    // =======
    // We can use the better technique of deleting the methods
    // we don't want.
public:
    ObjectFactory(ObjectFactory const&) = delete;
    void operator=(ObjectFactory const&) = delete;

    void SetUpObjectManager(std::shared_ptr<ObjectManager> om);
    void SetUpLevelManager();
    void AddSphere(Graphics& gfx, float pos[3], float radius, float latDiv, float longDiv, const char* vs, const char* ps, const char* gs = nullptr);
public:
    
    
    
private:
    std::shared_ptr<ObjectManager> om;
};

