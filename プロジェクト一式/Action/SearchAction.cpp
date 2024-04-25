#include "SearchAction.h"
#include "../Character/Character.h"
#include "../GameManager/GameManager.h"
#include "../Stage/CollisionMap.h"
#include "../Player/Player.h"
#include "../Engine/Global.h"

VisionSearchAction::VisionSearchAction(Character* obj, float range, float fov) : BaseAction(obj), range_(range), isFoundTarget_(false)
{
    pTarget_ = GameManager::GetPlayer();
    fovRadian_ = XMConvertToRadians(fov) / 2;
}

void VisionSearchAction::Update()
{
    XMFLOAT3 charaPos = pCharacter_->GetPosition();
    XMFLOAT3 targetPos = pTarget_->GetPosition();

    //�����Ă�����v�Z
    float charaRotateY = pCharacter_->GetRotate().y - 180.0f;
    XMFLOAT3 charaForward = XMFLOAT3();
    charaForward.x = (float)sin(XMConvertToRadians(charaRotateY));
    charaForward.z = (float)cos(XMConvertToRadians(charaRotateY));
    XMVECTOR vCharaForward = XMLoadFloat3(&charaForward);

    //�^�[�Q�b�g�ւ̃x�N�g��
    XMFLOAT3 toTarget = XMFLOAT3(charaPos.x - targetPos.x, 0.0f, charaPos.z - targetPos.z);
    XMVECTOR toTargetNorm = XMVector3Normalize(XMLoadFloat3(&toTarget));

    // ����p���v�Z
    float dotProduct = XMVectorGetX(XMVector3Dot(toTargetNorm, vCharaForward));
    float angle = acosf(dotProduct);

    // �p�x���r���ă^�[�Q�b�g���͈͊O�ɂ��邩�ǂ������m�F�E�͈͊O�Ȃ�I��
    if (angle > fovRadian_) return;

    // �����x�N�g�����͈͓��ɓ�����Cell�� Segment/CollisionMap ���g���Ă����蔻������Ă���
    RayCastData data;
    data.start = charaPos;
    XMStoreFloat3(&data.dir, (toTargetNorm * -1.0f));
    GameManager::GetCollisionMap()->RaySelectCellVsSegment(data, targetPos);

    //rayDist��toTarget���l���傫���Ȃ�Target��ڎ��o����
    float leng = XMVectorGetX(XMVector3Length(XMLoadFloat3(&toTarget)));

    isFoundTarget_ = false;
    if (data.hit) {
        if (data.dist > leng) {
            isFoundTarget_ = true;
        }
    }
    else {
        //�ǂ��ɂ�������Ȃ��������猩���Ă�
        isFoundTarget_ = true;
    }

}