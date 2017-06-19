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

#ifndef LAMA_H
#define LAMA_H

#include <controllable.h>


class Lama : public Controllable
{
    URHO3D_OBJECT(Lama, Controllable);
public:
    Lama(Context* context);
    static void RegisterObject(Context* context);
    virtual void Update(float timeStep);
    virtual void OnNodeSet(Node* node);

    Node* GetPitchNode() const { return pitchNode_; }
    void PostUpdate(float tiemStep);
private:
    Node* pitchNode_;
    float sinceJump_;
    float jumpInterval_;

    float sinceSpit_;
    float spitInterval_;
};

#endif // LAMA_H
