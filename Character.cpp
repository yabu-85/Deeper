#include "Character.h"

Character::Character(GameObject* parent) 
	: GameObject(parent), movement_(0,0,0), bodyWeight_(1.0f), bodyRange_(0.5f)
{
}

void Character::ReflectCharacter(Character* pCharacter) {
	//����̒��S�����ւ̃x�N�g�������߂�
	XMFLOAT3 tarPos = pCharacter->GetPosition();
	XMVECTOR direction = XMLoadFloat3(&tarPos) - XMLoadFloat3(&transform_.position_);
	
	//������������̋����������痣�ꂳ����
	float range = bodyRange_ + pCharacter->bodyRange_;
	if ( XMVectorGetX(XMVector3Length(direction)) < range ) {
		
		//��̍��W�̐^�񒆂����߂�
		direction *= 0.5f;
		XMVECTOR sPos = XMLoadFloat3(&transform_.position_) + direction;
		XMVECTOR oPos = XMLoadFloat3(&pCharacter->transform_.position_) - direction;
		
		//�����o���x�N�g�������߂�
		direction = XMVector3Normalize(direction) * range * 0.5f;

		float w = this->bodyWeight_ + pCharacter->bodyWeight_;
		float sWeight = pCharacter->bodyWeight_ / w + 0.001f;
		float oWeight = this->bodyWeight_ / w + 0.001f;

		sWeight = 1.0f;
		oWeight = 1.0f;

		//�����o��
		sPos = sPos - direction * oWeight;
		XMStoreFloat3(&transform_.position_, sPos);
		oPos = oPos + direction * sWeight;
		XMStoreFloat3(&pCharacter->transform_.position_, oPos);

		return;

	}
}