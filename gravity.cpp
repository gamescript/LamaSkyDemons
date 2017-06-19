
#include "gravity.h"



void Gravity::RegisterObject(Context* context)
{
    context->RegisterFactory<Gravity>();
}

Gravity::Gravity(Context* context) : LogicComponent(context)
{
}

void Gravity::OnNodeSet(Node* node)
{ if(!node) return;
}

void Gravity::Update(float timeStep)
{
    if (RigidBody* rigidBody = node_->GetComponent<RigidBody>()) {
        rigidBody->SetGravityOverride(23.0f * node_->GetWorldPosition().Normalized());
    }
}




