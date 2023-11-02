#include "LineCollider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Model.h"

//�R���X�g���N�^�i�����蔻��̍쐬�j
//�����FbasePos	�����蔻��̒��S�ʒu�i�Q�[���I�u�W�F�N�g�̌��_���猩���ʒu�j
//�����Fsize	�����蔻��̃T�C�Y
LineCollider::LineCollider(XMFLOAT3 center, XMFLOAT3 vec, float range)
{
	center_ = center;
	size_ = XMFLOAT3(range, range, range);
	vec_ = vec;
	type_ = COLLIDER_LINE;

	//�����[�X���͔���g�͕\�����Ȃ�
#ifdef _DEBUG
	//�e�X�g�\���p����g
	hDebugModel_ = Model::Load("DebugCollision/sphereCollider.fbx");
#endif
}

//�ڐG����
//�����Ftarget	����̓����蔻��
//�ߒl�F�ڐG���Ă��true
bool LineCollider::IsHit(Collider* target)
{
	if (target->type_ == COLLIDER_BOX)
		return IsHitBoxVsLine((BoxCollider*)target, this);
	else if (target->type_ == COLLIDER_CIRCLE)
		return IsHitCircleVsLine((SphereCollider*)target, this);
	else 
		return IsHitLineVsLine((LineCollider*)target, this);
}