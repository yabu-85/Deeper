#include "SearchAction.h"
#include "Character.h"
#include "CollisionMap.h"

#include "Engine/Input.h"
#include <math.h>

VisionSearchAction::VisionSearchAction(Character* obj, float range, float fov) : BaseAction(obj), range_(range), isFoundTarget_(false)
{
    pTarget_ = (Character*)pCharacter_->FindObject("Player");
    fovRadian_ = XMConvertToRadians(fov) / 2;
}

void VisionSearchAction::Update()
{
    // Characterの視線方向を計算
    XMFLOAT3 charaPos = pCharacter_->GetPosition();
    XMFLOAT3 targetPos = pTarget_->GetPosition();

    //ターゲットへのベクトルを計算（逆ベクトル）
    XMFLOAT3 toTarget = XMFLOAT3(charaPos.x - targetPos.x, 0.0f, charaPos.z - targetPos.z);

    float charaRotateY = pCharacter_->GetRotate().y - 180.0f;
    XMFLOAT3 charaForward = XMFLOAT3();
    charaForward.x = (float)sin(XMConvertToRadians(charaRotateY));
    charaForward.z = (float)cos(XMConvertToRadians(charaRotateY));

    // 方向ベクトルを正規化
    XMVECTOR toTargetNorm = XMVector3Normalize(XMLoadFloat3(&toTarget));
    XMVECTOR charaForwardNorm = XMLoadFloat3(&charaForward);

    // 視野角を計算
    float dotProduct = XMVectorGetX(XMVector3Dot(toTargetNorm, charaForwardNorm));
    float angle = acosf(dotProduct);

    // 角度を比較してターゲットが範囲外にあるかどうかを確認・範囲外なら終了
    
    pCharacter_->SetScale(XMFLOAT3(2.0f, 2.0f, 2.0f));
    
    if (angle > fovRadian_) {
        
        pCharacter_->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));

        return;
    }

    //とりあえずここにCollisionMapとの判定を作ろう
    //視線ベクトルが範囲内に入ったCellと Segment/CollisionMap を使ってあたり判定をしていく
    
    RayCastData data;
    data.start = charaPos;
    XMStoreFloat3(&data.dir, toTargetNorm);
    
    CollisionMap* pCollisionMap = (CollisionMap*)pCharacter_->FindObject("CollisionMap");
    pCollisionMap->RaySelectCellVsSegment(data, targetPos);

    //rayDistがtoTargetより値が大きいならTargetを目視出来た
    float leng = XMVectorGetX(XMVector3Length(XMLoadFloat3(&toTarget)));
    isFoundTarget_ = false;
    if (data.hit && data.dist > leng) {
    //    isFoundTarget_ = true;
    //    OutputDebugString("on : Hit\n");
        pCharacter_->SetScale(XMFLOAT3(2.0f, 2.0f, 2.0f));
    }
    else {
    //    OutputDebugString("on : no  ");
    //    OutputDebugStringA(std::to_string(data.dist).c_str() );
    //    OutputDebugString("\n");
    }

}

//------------------------------Audio---------------------------------

AuditorySearchAction::AuditorySearchAction(Character* obj) : BaseAction(obj)
{
}

void AuditorySearchAction::Update()
{
}

