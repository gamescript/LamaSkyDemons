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

#include "brixtuffcam.h"

BrixtuffCam::BrixtuffCam(Context *context):
    Object(context)
{
    rootNode_ = MC->scene_->CreateChild("Camera");
    camera_ = rootNode_->CreateComponent<Camera>();
    camera_->SetFarClip(1024.0f);
    camera_->SetNearClip(0.05f);
    camera_->SetFov(120);

//    rootNode_->CreateComponent<Light>()->SetRadius(256.0f);
}

void BrixtuffCam::SetupViewport()
{

    //Set up a viewport to the Renderer subsystem so that the 3D scene can be seen
    SharedPtr<Viewport> viewport(new Viewport(context_, MC->scene_, camera_));
    viewport_ = viewport;
//    int width{ GRAPHICS->GetWidth() / 2 };
//    int height{ GRAPHICS->GetHeight() / 2 };
//    viewport_->SetRect(IntRect(((playerId_ - 1) % 2) * width,
//                               ((playerId_ - 1) / 2) * height,
//                               (((playerId_ - 1) % 2) * width) + width,
//                               (((playerId_ - 1) / 2) * height) + height));

    //Add anti-asliasing and bloom
    effectRenderPath_ = viewport_->GetRenderPath()->Clone();
    effectRenderPath_->Append(CACHE->GetResource<XMLFile>("PostProcess/FXAA3.xml"));
    effectRenderPath_->SetEnabled("FXAA3", true);
    effectRenderPath_->Append(CACHE->GetResource<XMLFile>("PostProcess/BloomHDR.xml"));
    effectRenderPath_->SetShaderParameter("BloomHDRThreshold", 0.23f);
    effectRenderPath_->SetShaderParameter("BloomHDRMix", Vector2(0.75f, 0.5f));
    effectRenderPath_->SetEnabled("BloomHDR", true);

    Renderer* renderer{ GetSubsystem<Renderer>() };
    renderer->SetNumViewports(MC->GetCameras().Size());
    viewport_->SetRenderPath(effectRenderPath_);
    renderer->SetViewport(playerId_ - 1, viewport_);
}

Vector3 BrixtuffCam::GetWorldPosition()
{
    return rootNode_->GetWorldPosition();
}

Quaternion BrixtuffCam::GetRotation()
{
    return rootNode_->GetRotation();
}
