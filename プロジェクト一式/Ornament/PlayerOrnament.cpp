#include "PlayerOrnament.h"
#include "../Engine/Model.h"
#include "../Engine/Input.h"

namespace {

}

PlayerOrnament::PlayerOrnament(GameObject* parent)
    : GameObject(parent, "PlayerOrnament"), hModel_(-1)
{
}

PlayerOrnament::~PlayerOrnament()
{
}

void PlayerOrnament::Initialize()
{
    hModel_ = Model::Load("Model/Fiter.fbx");
    assert(hModel_ >= 0);
}

void PlayerOrnament::Update()
{
}

void PlayerOrnament::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
   
}

void PlayerOrnament::Release()
{
}