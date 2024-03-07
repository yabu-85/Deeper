#include "SearchAction.h"
#include "../Character/Character.h"
#include "../Stage/CollisionMap.h"

VisionSearchAction::VisionSearchAction(Character* obj, float range, float fov) : BaseAction(obj), range_(range), isFoundTarget_(false)
{
    pTarget_ = static_cast<Character*>(pCharacter_->FindObject("Player"));
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
    if (angle > fovRadian_) {
        return;
    }

    // ここからCollisionMapとの当たり判定をしてPlayerが見えていれば isFoundTarget = true
    // 視線ベクトルが範囲内に入ったCellと Segment/CollisionMap を使ってあたり判定をしていく

    RayCastData data;
    data.start = charaPos;
    XMStoreFloat3(&data.dir, (toTargetNorm * -1.0f));

    CollisionMap* pCollisionMap = static_cast<CollisionMap*>(pCharacter_->FindObject("CollisionMap"));
    pCollisionMap->RaySelectCellVsSegment(data, targetPos);

    //rayDistがtoTargetより値が大きいならTargetを目視出来た
    float leng = XMVectorGetX(XMVector3Length(XMLoadFloat3(&toTarget)));

    isFoundTarget_ = false;
    if (data.hit) {
        if (data.dist > leng) {
            isFoundTarget_ = true;
        }
    }
    else {
        //どこにも当たらなかったから見えてる
        isFoundTarget_ = true;
    }

}