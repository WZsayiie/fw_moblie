#include "csbaseobject.hh"
#include "csgameobject.hh"

cq_member(csBaseObject) {
};

void csBaseObject::dontDestroyOnLoad(csBaseObjectRef object) {
    if (object == nullptr) {
        return;
    }
    
    if (object->isKindOfClass(csComponent::getClass())) {
        
        auto component = cqObject::cast<csComponent>(object);
        auto gameObject = component->gameObject();
        csGameObject::dontDestoryOnLoad(gameObject);
        
    } else if (object->isKindOfClass(csGameObject::getClass())) {
        
        auto gameObject = cqObject::cast<csGameObject>(object);
        csGameObject::dontDestoryOnLoad(gameObject);
    }
}

void csBaseObject::destroy(csBaseObjectRef object) {
    if (object == nullptr) {
        return;
    }
    
    if (object->isKindOfClass(csComponent::getClass())) {
        
        auto component = cqObject::cast<csComponent>(object);
        auto gameObject = component->gameObject();
        if (gameObject != nullptr) {
            gameObject->removeComponent(object->clazz());
        }
        
    } else if (object->isKindOfClass(csGameObject::getClass())) {
        
        auto gameObject = cqObject::cast<csGameObject>(object);
        csGameObject::destroy(gameObject);
    }
}
