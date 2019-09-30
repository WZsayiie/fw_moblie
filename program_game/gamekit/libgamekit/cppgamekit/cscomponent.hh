#pragma once

#include "csnode.hh"

_CSGAMEKIT_BEGIN_VERSION_NS

cq_declare(csGameObject);
cq_declare(csTransform);

cq_class(csComponent, csNode) {
    
    //csGameObject.addComponent() will invoke this function, developer should not call it.
    virtual void resetGameObjectIfNeeded(csGameObjectRef gameObject);
    
    virtual csGameObjectRef gameObject();
    
    //get brother components:
    virtual csComponentRef getComponent(cqClass *clazz);
    
    template<class T> typename cqRef<T>::Strong getComponent() {
        return cqObject::cast<T>(getComponent(T::clazz()));
    }
    
    virtual csTransformRef transform();
};

_CSGAMEKIT_END_VERSION_NS
