/* Brixtuff
// Copyright (C) 2016 LucKey Productions (luckeyproductions.nl)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// Commercial licenses are available through frode@lindeijer.nl
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

#include "spit.h"

void Spit::RegisterObject(Context* context)
{
    context->RegisterFactory<Spit>();
}

Spit::Spit(Context* context) : SceneObject(context)
{

}

void Spit::OnNodeSet(Node* node)
{ (void)node;
    StaticModel* model{ node_->CreateComponent<StaticModel>() };
    model->SetModel(MC->GetModel("Spit"));
    model->SetMaterial(MC->GetMaterial("Spit"));
    node_->CreateComponent<RigidBody>()->SetMass(0.5f);
}

void Spit::Set(Vector3 position)
{
    SceneObject::Set(position);

    RigidBody* rigidBody{ node_->GetComponent<RigidBody>() };
    rigidBody->ResetForces();
    rigidBody->ApplyImpulse(node_->GetDirection() * 5.0f);
}

void Spit::Update(float timeStep)
{
    node_->SetDirection(node_->GetComponent<RigidBody>()->GetLinearVelocity().Normalized());
}