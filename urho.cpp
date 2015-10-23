/* LucKey Productions Urho3D Project Template
//
// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <http://unlicense.org/>
*/

#include "urho.h"

Urho::Urho(Context* context, MasterControl* masterControl):
    Object(context),
    masterControl_{masterControl},
    velocity_{Vector3::ZERO},
    maxVelocity_{12.0f},
    target_{Vector3::LEFT},
    seenTarget_{false}
{
    rootNode_ = masterControl_->world.scene->CreateChild("Urho");
    rootNode_->SetPosition(Vector3(7.0f, -1.0f, 16.0f));
    rootNode_->SetRotation(Quaternion(0.0f, 90.0f, 0.0f));
    rootNode_->SetScale(1.0f);

    animatedModel_ = rootNode_->CreateComponent<AnimatedModel>();
    animatedModel_->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Urho.mdl"));
    animatedModel_->SetMaterial(masterControl_->cache_->GetResource<Material>("Resources/Materials/VCol.xml"));
    animatedModel_->SetCastShadows(true);

    animCtrl_ = rootNode_->CreateComponent<AnimationController>();
    animCtrl_->PlayExclusive("Resources/Models/Swim.ani", 0, true, 0.23f);


    SubscribeToEvent(E_UPDATE, HANDLER(Urho, HandleUpdate));
}

void Urho::HandleUpdate(StringHash eventType, VariantMap &eventData)
{
    float timeStep{eventData[Update::P_TIMESTEP].GetFloat()};
    Swim(timeStep);
}

void Urho::Swim(float timeStep)
{
    Vector3 targetDelta = target_ - rootNode_->GetPosition();

    //Accellerate
    bool limit = velocity_.Angle(targetDelta) < 90.0f && velocity_.Length() > maxVelocity_;
    if (!limit) {
        float swimFactor = pow( sin(0.42 * masterControl_->world.scene->GetElapsedTime()), 4.0 );
        swimFactor *= swimFactor;
        float angleFactor = rootNode_->GetDirection().Angle(targetDelta)*0.0023f;
        swimFactor = 3.0f * Clamp(5.0f * swimFactor - angleFactor + 0.23f * targetDelta.Length(), 0.23f, 2.0f);
        velocity_ += swimFactor * timeStep * targetDelta.Normalized();
    }
    //Check target visibility and distance
    if (targetDelta.Angle(rootNode_->GetDirection()) > 90.0f && seenTarget_ || targetDelta.Length() < 0.5f){
        target_ = SwimTarget();
        seenTarget_ = false;
    }
    else if (targetDelta.Angle(rootNode_->GetDirection()) < 90.0f){
        seenTarget_ = true;
    }
    velocity_ *= 0.96f;
    rootNode_->Translate(velocity_*timeStep, TS_WORLD);
    //Update rotation in accordance with Urho's movement.
    if (velocity_.Length() > 0.01f){
        Quaternion rotation = rootNode_->GetWorldRotation();
        Quaternion aimRotation = rotation;
        aimRotation.FromLookRotation(velocity_);
        rootNode_->SetRotation(rotation.Slerp(aimRotation, timeStep * velocity_.Length()));
    }
    //Update animation speed
    animCtrl_->SetSpeed("Resources/Models/Swim.ani", 4.2f*velocity_.Length()/maxVelocity_);
}
Vector3 Urho::SwimTarget()
{
    float extraAngle = 5.0f * rootNode_->GetPosition().Length();
    float randomAngle = LucKey::RandomSign() * Random(extraAngle, 5.0f+extraAngle);
    Vector3 planarDirection = (rootNode_->GetDirection() * (Vector3::ONE-Vector3::UP)).Normalized();
    Vector3 newTarget = rootNode_->GetPosition() + (Random(2.0f, 3.0f) * (Quaternion(randomAngle, Vector3::UP) * planarDirection));
    newTarget.y_ = Clamp(target_.y_ + Random(-0.23f, 0.23f), -2.0, 2.0f);
    newTarget *= 0.75f;
    return newTarget;
}
