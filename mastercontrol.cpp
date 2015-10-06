// LucKey Productions Urho3D Project Template
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

#include "mastercontrol.h"
#include "templatecam.h"
#include "inputmaster.h"

DEFINE_APPLICATION_MAIN(MasterControl);

MasterControl::MasterControl(Context *context):
    Application(context),
    paused_(false)
{
}


void MasterControl::Setup()
{
    engineParameters_["WindowTitle"] = "LucKey Productions Urho3D Project Template";
    engineParameters_["LogName"] = GetSubsystem<FileSystem>()->GetAppPreferencesDir("urho3d", "logs")+"TestVoxelWidget.log";
    /*engineParameters_["FullScreen"] = true;
    engineParameters_["Headless"] = false;
    engineParameters_["WindowWidth"] = 1980;
    engineParameters_["WindowHeight"] = 1080;*/
}
void MasterControl::Start()
{
    new InputMaster(context_, this);
    cache_ = GetSubsystem<ResourceCache>();
    graphics_ = GetSubsystem<Graphics>();
    renderer_ = GetSubsystem<Renderer>();

    defaultStyle_ = cache_->GetResource<XMLFile>("UI/DefaultStyle.xml");
    CreateConsoleAndDebugHud();
    CreateScene();
    CreateUI();
    SubscribeToEvents();

    /*
    Sound* music = cache_->GetResource<Sound>("Resources/Music/Macroform_-_Root.ogg");
    music->SetLooped(true);
    Node* musicNode = world.scene->CreateChild("Music");
    SoundSource* musicSource = musicNode->CreateComponent<SoundSource>();
    musicSource->SetSoundType(SOUND_MUSIC);
    musicSource->Play(music);
    */
}
void MasterControl::Stop()
{
    engine_->DumpResources(true);
}

void MasterControl::SubscribeToEvents()
{
    SubscribeToEvent(E_UPDATE, HANDLER(MasterControl, HandleUpdate));
    SubscribeToEvent(E_SCENEUPDATE, HANDLER(MasterControl, HandleSceneUpdate));
}

void MasterControl::CreateConsoleAndDebugHud()
{
    Console* console = engine_->CreateConsole();
    console->SetDefaultStyle(defaultStyle_);
    console->GetBackground()->SetOpacity(0.8f);

    DebugHud* debugHud = engine_->CreateDebugHud();
    debugHud->SetDefaultStyle(defaultStyle_);
}

void MasterControl::CreateUI()
{
    cache_ = GetSubsystem<ResourceCache>();
    UI* ui = GetSubsystem<UI>();

    world.cursor.uiCursor = new Cursor(context_);
    world.cursor.uiCursor->SetVisible(false);
    ui->SetCursor(world.cursor.uiCursor);
    world.cursor.uiCursor->SetPosition(graphics_->GetWidth()/2, graphics_->GetHeight()/2);
}

void MasterControl::CreateScene()
{
    world.scene = new Scene(context_);

    world.scene->CreateComponent<Octree>();
    world.scene->CreateComponent<DebugRenderer>();

    PhysicsWorld* physicsWorld = world.scene->CreateComponent<PhysicsWorld>();
//    physicsWorld->SetGravity(Vector3::ZERO);

    //Add a directional light to the world. Enable cascaded shadows on it
    Node* lightNode = world.scene->CreateChild("DirectionalLight");
    lightNode->SetPosition(Vector3(-5.0f, 10.0f, -7.0f));
    lightNode->LookAt(Vector3(0.0f, 0.0f, 0.0f));
    Light* sun = lightNode->CreateComponent<Light>();
    sun->SetLightType(LIGHT_DIRECTIONAL);
    sun->SetBrightness(0.23f);
    sun->SetColor(Color(0.8f, 0.9f, 0.95f));
    sun->SetCastShadows(true);
    sun->SetShadowBias(BiasParameters(0.00025f, 0.5f));
    sun->SetShadowCascade(CascadeParameters(7.0f, 23.0f, 42.0f, 500.0f, 0.8f));

    //Create a point light. Enable cascaded shadows on it
    movingLight_ = world.scene->CreateChild("PointLight");
    movingLight_->SetPosition(Vector3(-23.0f, 10.0f, -7.0f));
    Light* fireFly = movingLight_->CreateComponent<Light>();
    fireFly->SetLightType(LIGHT_POINT);
    fireFly->SetBrightness(0.666f);
    fireFly->SetRange(42.0f);
    fireFly->SetColor(Color(1.23f, 1.0f, 0.5f));
    fireFly->SetCastShadows(true);
    fireFly->SetShadowBias(BiasParameters(0.00025f, 0.5f));
    fireFly->SetShadowCascade(CascadeParameters(7.0f, 23.0f, 42.0f, 500.0f, 0.8f));


    //Create some Kekelplithfs
    for (int k = 0; k < 5; k++){
        Node* objectNode = world.scene->CreateChild("Kekelplithf");
        objectNode->SetPosition((-5.0f + 3.0f*k) * Vector3::RIGHT + 3.0f * Vector3::DOWN + (k%2) * 5.0f * Vector3::FORWARD);
        objectNode->SetRotation(Quaternion(0.0f, Random(360.0f), 0.0f));
        objectNode->SetScale(0.23f+Random(0.0f, 0.1f));

        StaticModel* animatedModel = objectNode->CreateComponent<StaticModel>();
        animatedModel->SetModel(cache_->GetResource<Model>("Resources/Models/Kekelplithf.mdl"));
        animatedModel->SetMaterial(cache_->GetResource<Material>("Resources/Materials/Kekelplithf.xml"));
        animatedModel->SetCastShadows(true);
    }

    //Create camera
    world.camera = new TemplateCam(context_, this);
}

void MasterControl::HandleUpdate(StringHash eventType, VariantMap &eventData)
{
}

void MasterControl::HandleSceneUpdate(StringHash eventType, VariantMap &eventData)
{
    float timeStep = eventData[SceneUpdate::P_TIMESTEP].GetFloat();
    movingLight_->Translate(Vector3::RIGHT*sin(world.scene->GetElapsedTime()*0.23)*0.1f);
}

void MasterControl::HandlePostRenderUpdate(StringHash eventType, VariantMap &eventData)
{
}

void MasterControl::Exit()
{
    engine_->Exit();
}






































