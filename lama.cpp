/* Brixtuff
// Copyright (C) 2016 LucKey Productions (luckeyproductions.nl)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "spawnmaster.h"
#include "player.h"
#include "spit.h"
#include "gravity.h"

#include "lama.h"

void Lama::RegisterObject(Context* context)
{
    context->RegisterFactory<Lama>();
}

Lama::Lama(Context* context) : Controllable(context),
  jumpInterval_{0.01f},
  spitInterval_{0.23f}
{
}

void Lama::OnNodeSet(Node* node)
{ (void)node;

    Controllable::OnNodeSet(node_);
    pitchNode_ = node_->CreateChild("Camera");
    pitchNode_->SetPosition(Vector3(0.0f, 0.35f, 0.1f));

    rigidBody_->SetMass(5.0f);
    rigidBody_->SetAngularFactor(Vector3::UP);
    rigidBody_->SetFriction(0.9f);
    rigidBody_->SetRollingFriction(0.0f);
    rigidBody_->SetAngularDamping(2342.0f);
    rigidBody_->SetAngularRestThreshold(1.0f);
    rigidBody_->SetRestitution(0.0f);
    rigidBody_->SetLinearDamping(0.0f);
    rigidBody_->SetCollisionLayer(1);

    node_->CreateComponent<Gravity>();

    collisionShape_->SetCapsule(0.42f, 1.0f, Vector3::UP * 0.23f);
    model_->SetModel(MC->GetModel("Lama"));
    SharedPtr<Material> mat{ MC->GetMaterial("Basic")->Clone() };
    Color color{};
    color.FromHSV(1.0f / 6.0f * GetSubsystem<SpawnMaster>()->CountActive<Lama>(), 1.0f, 0.23f);
    mat->SetShaderParameter("MatDiffColor", color);
    mat->SetShaderParameter("MatEmissiveColor", color);
    model_->SetMaterial(0, mat);
    model_->SetMaterial(1, MC->GetMaterial("Basic")->Clone());

}

void Lama::Update(float timeStep)
{
    sinceJump_ += timeStep;
    sinceSpit_ += timeStep;

    rigidBody_->ApplyForce(timeStep * 20000.0f * (node_->GetDirection() * move_.z_ +
                                                  node_->GetRight() * move_.x_));

    PODVector<RigidBody*> colliders{};
    rigidBody_->GetCollidingBodies(colliders);

    if (colliders.Size() && sinceJump_ > jumpInterval_){
        if (actions_[0]){
            rigidBody_->ApplyImpulse(42000.0f * node_->GetUp());
            sinceJump_ = 0.0f;
        } /*else if (move_.z_ != 0.0f || move_.x_ != 0){
            rigidBody_->ApplyImpulse(2.3f * (move_.z_ * node_->GetDirection() + move_.x_ * node_->GetRight() + Vector3::UP));
            sinceJump_ = 0.0f;
        }*/
    } else {
        rigidBody_->ApplyForce(timeStep * (235.0f - (GetLinearVelocity() - node_->GetDirection() * GetLinearVelocity().Length()).Length()) * (move_.z_ * node_->GetDirection() + move_.x_ * node_->GetRight() + Vector3::UP * actions_[0]));
    }

    if (GetPlayer() && GetPlayer()->GetPlayerId() == 1){

        node_->Rotate(Quaternion(INPUT->GetMouseMoveX() * 0.23f + 5.0f * aim_.x_, -node_->GetWorldPosition().Normalized()), TS_WORLD);
        pitchNode_->Rotate(Quaternion(INPUT->GetMouseMoveY() * 0.23f - 5.0f * aim_.z_, Vector3::RIGHT));

    }


    if (actions_[1] && sinceSpit_ > spitInterval_)
    {
        Spit* spit{ GetSubsystem<SpawnMaster>()->Create<Spit>() };
        spit->GetNode()->SetDirection(GetPitchNode()->GetWorldDirection() - GetPitchNode()->GetWorldPosition() * 0.01f);
        spit->Set(GetPitchNode()->GetWorldPosition() * 1.01f);

        sinceSpit_ = 0.0f;
    }
}

void Lama::HandleAction(int actionId)
{

    rigidBody_->ApplyImpulse(100.0f * node_->GetUp());
}

void Lama::PostUpdate(float timeStep)
{
    float angleDelta{ node_->GetUp().Angle(-node_->GetWorldPosition())};
    if (angleDelta > 23e-5) {
//        node_->RotateAround(node_->GetWorldPosition() + Vector3::UP * 0.21f, Quaternion::IDENTITY.Slerp(Quaternion(node_->GetUp(), -node_->GetWorldPosition().Normalized()), Clamp(timeStep * 23.0f * angleDelta, 0.0f, 1.0f)), TS_WORLD);
        node_->LookAt(Vector3::ZERO, Quaternion(90.0f, node_->GetRight()) * node_->GetWorldPosition().Normalized());
        node_->Rotate(Quaternion(90.0f, Vector3::RIGHT));
//        Quaternion rot{};
//        rot.FromLookRotation(-node_->GetWorldPosition().Normalized(), -node_->GetDirection());
//        rot = Quaternion(90.0f, node_->GetRight())
//        node_->SetRotation(rot);
    }
}



