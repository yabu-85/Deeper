#include "RotateAction.h"
#include "Player.h"

RotateAction::RotateAction(GameObject* obj) : BaseAction(obj), pPlayer_(nullptr), rotateRatio_(1.0f)
{
}

RotateAction::RotateAction(GameObject* obj, float ratio) : BaseAction(obj), pPlayer_(nullptr), rotateRatio_(ratio)
{
}

void RotateAction::Update()
{
    XMFLOAT3 plaPos = pPlayer_->GetPosition();
    XMFLOAT3 pos = pGameObject_->GetPosition();

    float rotateY = pGameObject_->GetRotate().y;
    XMFLOAT2 a = XMFLOAT2(sinf(XMConvertToRadians(rotateY)), cosf(XMConvertToRadians(rotateY)));
    XMVECTOR vA = XMVector2Normalize(XMLoadFloat2(&a));
    XMFLOAT2 b(pos.x - plaPos.x, pos.z - plaPos.z);
    XMVECTOR vB = XMVector2Normalize(XMLoadFloat2(&b)) * -1.0f;
    XMStoreFloat2(&a, vA);
    XMStoreFloat2(&b, vB);
    float cross = a.x * b.y - a.y * b.x;
    float dot = a.x * b.x + a.y * b.y;
    rotateY += XMConvertToDegrees(-atan2f(cross, dot) * rotateRatio_);
    pGameObject_->SetRotateY(rotateY);
}

void RotateAction::Initialize()
{
    pPlayer_ = static_cast<Player*>(pGameObject_->FindObject("Player"));

}

void RotateAction::Terminate()
{
}
