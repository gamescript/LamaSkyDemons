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

#include "platform.h"


Platform::Platform(Context* context) : LogicComponent(context)
{
}

void Platform::RegisterObject(Context* context)
{
    context->RegisterFactory<Platform>();
}

void Platform::OnNodeSet(Node* node)
{ (void)node;

    Zone* zone = node_->CreateComponent<Zone>();
    zone->SetFogStart(23.0f);
    zone->SetFogEnd(128.0f);
    zone->SetFogColor(Color(0.8f, 0.9f, 1.0f));
    zone->SetBoundingBox(BoundingBox(Vector3::ONE * -1000.0f, Vector3::ONE * 1000.0f));
    zone->SetAmbientColor(Color(0.4f, 0.45f, 0.5f));

//    node_->CreateComponent<Skybox>()->SetMaterial();

    StaticModel* terrain{ node_->CreateComponent<StaticModel>() };
    terrain->SetModel(MC->GetModel("Level1"));
    terrain->SetMaterial(MC->GetMaterial("VCol"));
    terrain->SetOccludee(false);
    terrain->SetCastShadows(true);

    node_->CreateComponent<RigidBody>()->SetFriction(1.0f);

    CollisionShape* terrainCollider{ node_->CreateComponent<CollisionShape>() };
    terrainCollider->SetTriangleMesh(MC->GetModel("Level1"));
//    node_->SetScale(Vector3(5.0f, 0.23f, 5.0f));
/*
    for (int i{-23}; i < 23; ++i){
        for (int j{-23}; j < 23; ++j){
            for (int k{0}; k < Random(5, 2 * Abs(i + j) + 5) + !Random(5) * Random(5); ++k){
                if (!Random(5) || k < 2 && k < Abs(i + j)){
                    Node* blockNode{ node_->CreateChild("Block") };
                    blockNode->SetPosition(Vector3(4.2f * i, k * 0.1f + Random(Abs(j * k) * 0.1f), 4.2f * j));
                    Vector3 scale{ 2.3f * Vector3((2.3f - Random(0.023f, 0.05f) * k * k) + Random(0.23f, 0.5f),
                                          Random(0.42f, 0.5f + Abs(j * k) * 0.023f),
                                                (2.3f - Random(0.023f, 0.05f) * k * k) + Random(0.23f, 0.5f))};
                    LucKey::Scale(scale, scale);
                    blockNode->SetScale(scale);

                    blockNode->Rotate(Quaternion(Random(2.3f) * Random(2.3f),
                                                 Random(360.0f),
                                                 Random(2.3f) * Random(2.3f)));

                    StaticModel* staticModel{ blockNode->CreateComponent<StaticModel>() };
                    staticModel->SetModel(MC->GetModel("Tooth"));
                    staticModel->SetMaterial(MC->GetMaterial("Tooth"));
                    staticModel->SetCastShadows(true);

                    blockNode->CreateComponent<RigidBody>();
                    blockNode->CreateComponent<CollisionShape>()->SetTriangleMesh(MC->GetModel("Tooth"));
                }
            }
        }
    }
    */
}

void Platform::Update(float timeStep)
{
//    node_->GetComponent<Zone>()->SetFogColor((Color(0.23f, 0.23f, 0.23f) + LucKey::RandomColor()) * 0.05f * MC->Sine);
}





