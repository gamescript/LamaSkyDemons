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

#include "mastercontrol.h"
#include "inputmaster.h"
#include "spawnmaster.h"
#include "brixtuffcam.h"
#include "gravity.h"
#include "platform.h"
#include "lama.h"
#include "spit.h"
#include "player.h"

URHO3D_DEFINE_APPLICATION_MAIN(MasterControl);

MasterControl* MasterControl::instance_ = NULL;

MasterControl* MasterControl::GetInstance()
{
    return MasterControl::instance_;
}

MasterControl::MasterControl(Context *context):
    Application(context)
{
    instance_ = this;
}

void MasterControl::Setup()
{
    SetRandomSeed(TIME->GetSystemTime());

    engineParameters_["WindowTitle"] = "Brixtuff";
    engineParameters_["LogName"] = GetSubsystem<FileSystem>()->GetAppPreferencesDir("urho3d", "logs")+"LucKeyTemplate.log";
    engineParameters_["ResourcePaths"] = "Data;CoreData;Resources";
    engineParameters_["WindowIcon"] = "icon.png";
//    engineParameters_["FullScreen"] = false;
//    engineParameters_["WindowWidth"] = 960;
//    engineParameters_["WindowHeight"] = 540;
}
void MasterControl::Start()
{
    Gravity::RegisterObject(context_);
    Platform::RegisterObject(context_);
    Lama::RegisterObject(context_);
    Spit::RegisterObject(context_);

    context_->RegisterSubsystem(new InputMaster(context_));
    context_->RegisterSubsystem(new SpawnMaster(context_));

    CreateScene();

    //Play music
    Sound* music{ CACHE->GetResource<Sound>("Resources/Music/Urho - Disciples of Urho.ogg") };
    music->SetLooped(true);
    Node* musicNode{ scene_->CreateChild("Music") };
    SoundSource* musicSource{ musicNode->CreateComponent<SoundSource>() };
    musicSource->SetSoundType(SOUND_MUSIC);
//    musicSource->Play(music);
}
void MasterControl::Stop()
{
    engine_->DumpResources(true);
}
void MasterControl::Exit()
{
    engine_->Exit();
}


void MasterControl::CreateScene()
{
    scene_ = new Scene(context_);
    scene_->CreateComponent<Octree>();
    PhysicsWorld* physicsWorld{ scene_->CreateComponent<PhysicsWorld>() };
    physicsWorld->SetGravity(Vector3::ZERO);
    physicsWorld->SetFps(50);
//    world.camera = new BrixtuffCam(context_, this);
    CreateLights();


    Node* platformNode{ scene_->CreateChild("Platform") };
    platformNode->CreateComponent<Platform>();

    for (int p : {1}){//, 2, 3, 4}){
        players_.Push(SharedPtr<Player>(new Player(p, context_)));

        Lama* lama{ GetSubsystem<SpawnMaster>()->Create<Lama>() };
        lama->Set(Vector3::DOWN * 5.0f);

        GetSubsystem<InputMaster>()->SetPlayerControl(GetPlayer(p), lama);

        BrixtuffCam* cam{ new BrixtuffCam(context_) };
        cam->playerId_ = p;
        cam->rootNode_->SetParent(lama->GetPitchNode());
        cam->rootNode_->SetPosition(Vector3::ZERO);
        cameras_.Push(SharedPtr<BrixtuffCam>(cam));
    }

    for (BrixtuffCam* c : cameras_){
        c->SetupViewport();
    }

//    world.camera->rootNode_->LookAt(lama->GetNode()->GetPosition());
}

void MasterControl::CreateLights()
{
    Node* sunLightNode{ scene_->CreateChild("CentralLight") };
    Light* sunLight{ sunLightNode->CreateComponent<Light>() };
    sunLight->SetLightType(LIGHT_DIRECTIONAL);
    sunLight->SetRange(200.0f);
    sunLight->SetBrightness(0.5f);
    sunLight->SetCastShadows(true);
    sunLight->SetShadowDistance(10.0f);
    sunLight->SetFadeDistance(1.0f);
    sunLight->SetShadowIntensity(0.7f);
            /*
    //Add a directional light to the world. Enable cascaded shadows on it
    Node* downardsLightNode = scene_->CreateChild("DirectionalLight");
    downardsLightNode->SetPosition(Vector3(-2.0f, 10.0f, -5.0f));
    downardsLightNode->LookAt(Vector3(0.0f, 0.0f, 0.0f));
    Light* downwardsLight = downardsLightNode->CreateComponent<Light>();
    downwardsLight->SetLightType(LIGHT_DIRECTIONAL);
    downwardsLight->SetBrightness(0.666f);
    downwardsLight->SetColor(Color(0.8f, 0.9f, 0.95f));
    downwardsLight->SetCastShadows(true);
    downwardsLight->SetShadowIntensity(0.23f);
    downwardsLight->SetShadowBias(BiasParameters(0.000025f, 0.5f));
    downwardsLight->SetShadowCascade(CascadeParameters(1.0f, 5.0f, 23.0f, 100.0f, 0.8f));
*/
    //Add a directional light to the world. Enable cascaded shadows on it
    /*Node* upwardsLightNode = scene_->CreateChild("DirectionalLight");
    upwardsLightNode->SetPosition(Vector3(3.0f, 2.0f, 5.0f));
    upwardsLightNode->LookAt(Vector3(0.0f, 0.0f, 0.0f));
    Light* upwardsLight = upwardsLightNode->CreateComponent<Light>();
    upwardsLight->SetCastShadows(true);
    upwardsLight->SetLightType(LIGHT_DIRECTIONAL);
    upwardsLight->SetBrightness(0.34);
    upwardsLight->SetColor(Color(0.23f, 0.666f, 1.0f));

    //Create a point light. Enable cascaded shadows on it
    Node* pointLightNode_ = scene_->CreateChild("MovingLight");
    pointLightNode_->SetPosition(Vector3(0.0f, 23.0f, 0.0f));
    Light* pointLight = pointLightNode_->CreateComponent<Light>();
    pointLight->SetCastShadows(true);
    pointLight->SetLightType(LIGHT_POINT);
    pointLight->SetBrightness(0.1f);
    pointLight->SetRange(42.0f);
    pointLight->SetColor(Color(1.0f, 0.0f, 0.0f));*/
}

Vector<SharedPtr<Player> > MasterControl::GetPlayers() const {
    return players_;
}
Player* MasterControl::GetPlayer(int playerId) const
{
    for (Player* p : players_) {

        if (p->GetPlayerId() == playerId)
            return p;
    }
    return nullptr;
}

Vector<SharedPtr<BrixtuffCam> > MasterControl::GetCameras() const {
    return cameras_;
}
BrixtuffCam* MasterControl::GetCamera(int playerId) const
{
    for (BrixtuffCam* c : cameras_) {

        if (c->GetPlayerId() == playerId)
            return c;
    }
    return nullptr;
}

Material* MasterControl::GetMaterial(String name) const
{
    return CACHE->GetResource<Material>("Materials/" + name + ".xml");
}
Model* MasterControl::GetModel(String name) const
{
    return CACHE->GetResource<Model>("Models/" + name + ".mdl");
}
Texture* MasterControl::GetTexture(String name) const
{
    return CACHE->GetResource<Texture>("Textures/" + name + ".png");
}
Sound* MasterControl::GetMusic(String name) const {
    Sound* song{ CACHE->GetResource<Sound>("Music/" + name + ".ogg") };
    song->SetLooped(true);
    return song;
}
Sound* MasterControl::GetSample(String name) const {
    Sound* sample{ CACHE->GetResource<Sound>("Samples/" + name + ".ogg") };
    sample->SetLooped(false);
    return sample;
}

float MasterControl::Sine(const float freq, const float min, const float max, const float shift)
{
    float phase{SinePhase(freq, shift)};
    float add{0.5f * (min + max)};
    return LucKey::Sine(phase) * 0.5f * (max - min) + add;
}
float MasterControl::Cosine(const float freq, const float min, const float max, const float shift)
{
    return Sine(freq, min, max, shift + 0.25f);
}
float MasterControl::SinePhase(float freq, float shift)
{
    return M_PI * 2.0f * (freq * scene_->GetElapsedTime() + shift);
}
