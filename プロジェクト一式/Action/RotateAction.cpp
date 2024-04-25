#include "RotateAction.h"
#include "../Character/Character.h"
#include "../GameManager/GameManager.h"
#include "../Player/Player.h"
#include "../Engine/Global.h"

RotateAction::RotateAction(Character* obj) : BaseAction(obj), pTarget_(nullptr), rotateRatio_(1.0f)
{
    pTarget_ = GameManager::GetPlayer();
}

RotateAction::RotateAction(Character* obj, float ratio) : BaseAction(obj), pTarget_(nullptr), rotateRatio_(ratio)
{
}

void RotateAction::Update()
{
    XMFLOAT3 tar = XMFLOAT3();
    XMFLOAT3 pos = pCharacter_->GetPosition();

    //TargetCharacterがいるならその座標を目標地点に
    if (pTarget_) tar = pTarget_->GetPosition();
    else XMStoreFloat3(&tar, XMLoadFloat3(&pos) - pCharacter_->GetMovementVector());

    float rotateY = pCharacter_->GetRotate().y;
    XMFLOAT2 a = XMFLOAT2(sinf(XMConvertToRadians(rotateY)), cosf(XMConvertToRadians(rotateY)));
    XMVECTOR vA = XMVector2Normalize(XMLoadFloat2(&a));
    XMFLOAT2 b(pos.x - tar.x, pos.z - tar.z);
    XMVECTOR vB = XMVector2Normalize(XMLoadFloat2(&b)) * -1.0f;
    XMStoreFloat2(&a, vA);
    XMStoreFloat2(&b, vB);
    float cross = a.x * b.y - a.y * b.x;
    float dot = a.x * b.x + a.y * b.y;
    rotateY += XMConvertToDegrees(-atan2f(cross, dot) * rotateRatio_);
    pCharacter_->SetRotateY(rotateY);
}