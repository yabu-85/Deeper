#include "BoxCollider.h"
#include "SphereCollider.h"
#include "SegmentCollider.h"
#include "GameObject.h"
#include "Model.h"
#include "Transform.h"

//�R���X�g���N�^
Collider::Collider():
	pGameObject_(nullptr), center_(0.0f, 0.0f, 0.0f), hDebugModel_(-1), size_(0.0f, 0.0f, 0.0f), type_(COLLIDER_MAX)
{
}

//�f�X�g���N�^
Collider::~Collider()
{
}

//���^���m�̏Փ˔���
//�����FboxA	�P�ڂ̔��^����
//�����FboxB	�Q�ڂ̔��^����
//�ߒl�F�ڐG���Ă����true
bool Collider::IsHitBoxVsBox(BoxCollider* boxA, BoxCollider* boxB)
{

	XMFLOAT3 boxPosA = Transform::Float3Add(boxA->pGameObject_->GetWorldPosition(), boxA->center_);
	XMFLOAT3 boxPosB = Transform::Float3Add(boxB->pGameObject_->GetWorldPosition(), boxB->center_);

	if ((boxPosA.x + boxA->size_.x / 2) > (boxPosB.x - boxB->size_.x / 2) &&
		(boxPosA.x - boxA->size_.x / 2) < (boxPosB.x + boxB->size_.x / 2) &&
		(boxPosA.y + boxA->size_.y / 2) > (boxPosB.y - boxB->size_.y / 2) &&
		(boxPosA.y - boxA->size_.y / 2) < (boxPosB.y + boxB->size_.y / 2) &&
		(boxPosA.z + boxA->size_.z / 2) > (boxPosB.z - boxB->size_.z / 2) &&
		(boxPosA.z - boxA->size_.z / 2) < (boxPosB.z + boxB->size_.z / 2))
	{
		return true;
	}
	return false;
}

//���^�Ƌ��̂̏Փ˔���
//�����Fbox	���^����
//�����Fsphere	�Q�ڂ̔��^����
//�ߒl�F�ڐG���Ă����true
bool Collider::IsHitBoxVsCircle(BoxCollider* box, SphereCollider* sphere)
{
	XMFLOAT3 circlePos = Transform::Float3Add(sphere->pGameObject_->GetWorldPosition(), sphere->center_);
	XMFLOAT3 boxPos = Transform::Float3Add(box->pGameObject_->GetWorldPosition(), box->center_);

	if (circlePos.x > boxPos.x - box->size_.x - sphere->size_.x &&
		circlePos.x < boxPos.x + box->size_.x + sphere->size_.x &&
		circlePos.y > boxPos.y - box->size_.y - sphere->size_.x &&
		circlePos.y < boxPos.y + box->size_.y + sphere->size_.x &&
		circlePos.z > boxPos.z - box->size_.z - sphere->size_.x &&
		circlePos.z < boxPos.z + box->size_.z + sphere->size_.x )
	{
		return true;
	}

	return false;
}

//���^�Ɛ����̏Փ˔���
//�����Fbox	 ���^����
//�����Fseg  ����
//�ߒl�F�ڐG���Ă����true
bool Collider::IsHitBoxVsSegment(BoxCollider* box, SegmentCollider* seg)
{

	return false;
}

//���̓��m�̏Փ˔���
//�����FcircleA	�P�ڂ̋��̔���
//�����FcircleB	�Q�ڂ̋��̔���
//�ߒl�F�ڐG���Ă����true
bool Collider::IsHitCircleVsCircle(SphereCollider* circleA, SphereCollider* circleB)
{
	XMFLOAT3 centerA = circleA->center_;
	XMFLOAT3 positionA = circleA->pGameObject_->GetWorldPosition();
	XMFLOAT3 centerB = circleB->center_;
	XMFLOAT3 positionB = circleB->pGameObject_->GetWorldPosition();

	XMVECTOR v = (XMLoadFloat3(&centerA) + XMLoadFloat3(&positionA))
				- (XMLoadFloat3(&centerB) + XMLoadFloat3(&positionB));

	if (XMVector3Length(v).m128_f32[0] <= circleA->size_.x + circleB->size_.x)
	{
		return true;
	}

	return false;
}

//���̂Ɛ����̏Փ˔���
//�����Fcircle ���̔���
//�����Fseg    ����
//�ߒl�F�ڐG���Ă����true
bool Collider::IsHitCircleVsSegment(SphereCollider* circle, SegmentCollider* seg)
{
	//���̂̒��S���W
	XMFLOAT3 f = circle->pGameObject_->GetWorldPosition();
	XMVECTOR cCenter = XMLoadFloat3(&f) + XMLoadFloat3(&circle->center_);

	//�����̎n�_�ƏI�_���W
	f = seg->pGameObject_->GetWorldPosition();
	XMVECTOR sStart = XMLoadFloat3(&f) + XMLoadFloat3(&seg->center_);
	XMVECTOR sEnd = sStart + seg->vec_ * seg->size_.x;
	
	//�����̏I�_�Ƌ��̋������~�͈͓̔��Ȃ瓖�����Ă�
	XMVECTOR vP = cCenter - sEnd;
	if (circle->size_.x > XMVectorGetX(XMVector3Length(vP))) return true;

	//�����̎n�_�Ƌ��̋������~�͈͓̔��Ȃ瓖�����Ă�
	vP = cCenter - sStart;
	if (circle->size_.x > XMVectorGetX(XMVector3Length(vP))) return true;

	//���ς�0���傫���A�����x�N�g���̑傫����菬�����Ȃ�
	float dot = XMVectorGetX(XMVector3Dot(seg->vec_, vP));
	if (dot > 0 && dot < seg->size_.x) {
		//dot�̒����̃x�N�g��
		XMVECTOR vec = seg->vec_ * dot;

		//���̂����ԋ߂�segment�̓_�Ƌ��̂܂ł̋��������̂̔��a�̂Q���菬������Γ������Ă�
		float range = XMVectorGetX(XMVector3Length(vP - vec));
		if (circle->size_.x > range) return true;
	}
	return false;
	
}


//�������m�̏Փ˔���
//�����FSegA	�P�ڂ̐���
//�����FSegmB	�Q�ڂ̐���
//�ߒl�F�ڐG���Ă����true
bool Collider::IsHitSegmentVsSegment(SegmentCollider* segA, SegmentCollider* segB)
{
	return false;
}

//�e�X�g�\���p�̘g��`��
//�����Fposition	�I�u�W�F�N�g�̈ʒu
void Collider::Draw(XMFLOAT3 position)
{
	Transform transform;
	transform.position_ = XMFLOAT3(position.x + center_.x, position.y + center_.y, position.z + center_.z);
	transform.scale_ = size_;
	transform.Calclation();
	Model::SetTransform(hDebugModel_, transform);
	Model::Draw(hDebugModel_, 4);
}
