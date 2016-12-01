#ifndef SPIT_H
#define SPIT_H

#include <Urho3D/Urho3D.h>
#include "sceneobject.h"

class Spit : public SceneObject
{
    URHO3D_OBJECT(Spit, SceneObject);
public:
    Spit(Context* context);
    static void RegisterObject(Context* context);
    virtual void OnNodeSet(Node* node);
    virtual void Update(float timeStep);
    virtual void Set(Vector3 position);
};

#endif // SPIT_H
