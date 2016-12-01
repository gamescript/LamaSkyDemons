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

    rigidBody_->SetMass(1.0f);
    rigidBody_->SetAngularFactor(Vector3::UP);
    rigidBody_->SetFriction(0.5f);
    rigidBody_->SetAngularDamping(5.0f);
    rigidBody_->SetRestitution(0.1f);

    collisionShape_->SetCapsule(0.23f, 0.32f, Vector3::UP * 0.16f);
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

    PODVector<RigidBody*> colliders{};
    rigidBody_->GetCollidingBodies(colliders);

    if (colliders.Size() && sinceJump_ > jumpInterval_){
        if (actions_[0]){
            rigidBody_->ApplyImpulse(4.2f * Vector3::UP);
            sinceJump_ = 0.0f;
        } else if (move_.z_ != 0.0f || move_.x_ != 0){
            rigidBody_->ApplyImpulse(0.5f * (move_.z_ * node_->GetDirection() + move_.x_ * node_->GetRight() + Vector3::UP));
            sinceJump_ = 0.0f;
        }
    } else {
        rigidBody_->ApplyForce(timeStep * 23.0f * (move_.z_ * node_->GetDirection() + move_.x_ * node_->GetRight() + Vector3::UP * actions_[0]));
    }

    if (GetPlayer() && GetPlayer()->GetPlayerId() == 1){

        node_->Rotate(Quaternion(0.0f, INPUT->GetMouseMoveX() + 5.0f * aim_.x_, 0.0f), TS_WORLD);
        node_->Rotate(Quaternion(INPUT->GetMouseMoveY() - 5.0f * aim_.z_, 0.0f, 0.0f));
    }

    if (actions_[1] && sinceSpit_ > spitInterval_)
    {
        Spit* spit{ GetSubsystem<SpawnMaster>()->Create<Spit>() };
        spit->GetNode()->SetDirection(node_->GetDirection());
        spit->Set(node_->GetPosition() + Vector3::UP * 0.32f);

        sinceSpit_ = 0.0f;
    }
}





