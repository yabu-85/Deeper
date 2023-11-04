#include "BoxCollider.h"
#include "SphereCollider.h"
#include "LineCollider.h"
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

//���^�ƒ����̏Փ˔���
//�����Fbox	���^����
//�����Fline	�Q�ڂ̒�������
//�ߒl�F�ڐG���Ă����true
bool Collider::IsHitBoxVsLine(BoxCollider* box, LineCollider* line)
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

//���̂ƒ����̏Փ˔���
//�����Fcircle	�P�ڂ̋��̔���
//�����Fline	�Q�ڂ̒�������
//�ߒl�F�ڐG���Ă����true
bool Collider::IsHitCircleVsLine(SphereCollider* circle, LineCollider* line)
{
	//���̒��S���W
	XMFLOAT3 cCenter = circle->center_;
	XMFLOAT3 cWorldPos = circle->pGameObject_->GetWorldPosition();
	cCenter.x += cWorldPos.x;
	cCenter.y += cWorldPos.y;
	cCenter.z += cWorldPos.z;

	//�����̎n�_
	XMFLOAT3 lStart = line->center_;
	XMFLOAT3 lWorldPos = line->pGameObject_->GetWorldPosition();
	lStart.x += lWorldPos.x;
	lStart.y += lWorldPos.y;
	lStart.z += lWorldPos.z;

	float r = circle->size_.x; //�~�̔��a

	//����Start��P�̔��a���ɓ����Ă����疳�����œ������Ă�Ƃ݂Ȃ�
	double distSquared = pow(cCenter.x - lStart.x, 2) + pow(cCenter.y - lStart.y, 2) + pow(cCenter.z - lStart.z, 2);
	double radSquared = pow(r, 2);
	if (distSquared < radSquared) {
		return true;
	}

	// ���̒��S���璼����̍ŒZ�������v�Z
	cCenter.x = cCenter.x - lStart.x;
	cCenter.y = cCenter.y - lStart.y;
	cCenter.z = cCenter.z - lStart.z;

	float A = line->vec_.x * line->vec_.x + line->vec_.y * line->vec_.y + line->vec_.z * line->vec_.z;
	float B = line->vec_.x * cCenter.x + line->vec_.y * cCenter.y + line->vec_.z * cCenter.z;
	float C = cCenter.x * cCenter.x + cCenter.y * cCenter.y + cCenter.z * cCenter.z - r * r;

	if (A == 0.0f)
		return false;  //���C�̒�����0

	float s = B * B - A * C;
	if (s < 0.0f) return false; // �Փ˂��Ă��Ȃ�

	s = sqrtf(s);
	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	if (a1 < 0.0f || a2 < 0.0f) return false; // ���C�̔��΂ŏՓ�

	XMFLOAT3 q1; //�ՓˊJ�n�n�_
	q1.x = lStart.x + a1 * line->vec_.x;
	q1.y = lStart.y + a1 * line->vec_.y;
	q1.z = lStart.z + a1 * line->vec_.z;

	// �x�N�g���̍������v�Z
	XMVECTOR difference = XMVectorSubtract(XMLoadFloat3(&lStart), XMLoadFloat3(&q1));

	// �x�N�g���̒����i�����j���v�Z
	XMVECTOR distance = XMVector3Length(difference);
	float distanceFloat;
	XMStoreFloat(&distanceFloat, distance);

	//range�͈̔͊O�������ꍇ false
	if (line->size_.x < distanceFloat) return false;

	return true;
}

//�������m�̏Փ˔���
//�����FlineA	�P�ڂ̒�������
//�����FlineB	�Q�ڂ̒�������
//�ߒl�F�ڐG���Ă����true
bool Collider::IsHitLineVsLine(LineCollider* lineA, LineCollider* lineB)
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
