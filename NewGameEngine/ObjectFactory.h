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

    // Note: Scott Meyers mentions in his Effective Modern
    //       C++ book, that deleted functions should generally
    //       be public as it results in better error messages
    //       due to the compilers behavior to check accessibility
    //       before deleted status
public:
    
    
    
private:
    std::shared_ptr<ObjectManager> om;
};

