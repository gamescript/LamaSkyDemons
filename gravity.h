
#ifndef GRAVITY_H
#define GRAVITY_H

#include <Urho3D/Urho3D.h>
#include "luckey.h"

class Gravity : public LogicComponent
{
    URHO3D_OBJECT(Gravity, LogicComponent);
public:
    Gravity(Context* context);
    static void RegisterObject(Context* context);
    virtual void OnNodeSet(Node* node);
    virtual void Update(float timeStep);
};

#endif // GRAVITY_H
