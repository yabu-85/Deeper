#include "SearchAction.h"
#include "Character.h"

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

    // 角度を比較してターゲットが範囲内にあるかどうかを確認
    isFoundTarget_ = false;
    if (angle <= fovRadian_) {
        //視野内だよ
        isFoundTarget_ = true;
    }


    //とりあえずここにCollisionMapとの判定を作ろう
    //視線ベクトルが範囲内に入ったCellと Segment/CollisionMap を使ってあたり判定をしていく


    // line ベクトルに垂直なベクトルを計算
    XMVECTOR upVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // もし他のベクトルが必要なら変更
    XMVECTOR lineNormal = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&charaForward), upVector));

    // 四角形の四隅の点を求める
    XMVECTOR c1 = XMVectorSet(20, 0, 20, 0);
    XMVECTOR c2 = XMVectorSet(20, 0, 30, 0);
    XMVECTOR c3 = XMVectorSet(30, 0, 30, 0);
    XMVECTOR c4 = XMVectorSet(30, 0, 20, 0);

    // 四隅の点から線のベースとなる点の距離を求める
    XMVECTOR lineBase = XMLoadFloat3(&charaPos);
    c1 = c1 - lineBase;
    c2 = c2 - lineBase;
    c3 = c3 - lineBase;
    c4 = c4 - lineBase;

    // 内積により線の方向を求める
    float dp1 = XMVectorGetY(XMVector3Dot(lineNormal, c1));
    float dp2 = XMVectorGetY(XMVector3Dot(lineNormal, c2));
    float dp3 = XMVectorGetY(XMVector3Dot(lineNormal, c3));
    float dp4 = XMVectorGetY(XMVector3Dot(lineNormal, c4));

    // 全部同じ方向にあれば、線と四角形が当たっていることはない
    if ((dp1 * dp2 <= 0) || (dp2 * dp3 <= 0) || (dp3 * dp4 <= 0)) {
        OutputDebugString("Hit\n");
    }
    else {
        OutputDebugString("\n");
    }


    //CellのTrianglesに対して

    


}

//------------------------------Audio---------------------------------

AuditorySearchAction::AuditorySearchAction(Character* obj) : BaseAction(obj)
{
}

void AuditorySearchAction::Update()
{
}

