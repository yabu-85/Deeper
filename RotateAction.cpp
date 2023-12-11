#include "RotateAction.h"
#include "Engine/GameObject.h"

RotateAction::RotateAction(GameObject* obj) : BaseAction(obj), pTarget_(nullptr), rotateRatio_(1.0f)
{
}

RotateAction::RotateAction(GameObject* obj, float ratio) : BaseAction(obj), pTarget_(nullptr), rotateRatio_(ratio)
{
}

void RotateAction::Update()
{
    XMFLOAT3 tar = XMFLOAT3();
    
    if(pTarget_) pTarget_->GetPosition();
    

    XMFLOAT3 pos = pGameObject_->GetPosition();

    float rotateY = pGameObject_->GetRotate().y;
    XMFLOAT2 a = XMFLOAT2(sinf(XMConvertToRadians(rotateY)), cosf(XMConvertToRadians(rotateY)));
    XMVECTOR vA = XMVector2Normalize(XMLoadFloat2(&a));
    XMFLOAT2 b(pos.x - tar.x, pos.z - tar.z);
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
    pTarget_ = static_cast<GameObject*>(pGameObject_->FindObject("Player"));

}

void RotateAction::Terminate()
{
}
