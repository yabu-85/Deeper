#include "Character.h"
#include "CharacterManager.h"
#include "../Engine/Global.h"
#include "../Engine/Easing.h"
#include <vector>

Character::Character(GameObject* parent, std::string name)
	: GameObject(parent, name), movement_(0,0,0), bodyWeight_(1.0f), bodyRange_(0.5f), knockBackTime_(0), knockBackTimeMax_(0), knockBackPower_(0.0f),
	knockBackDirection_{0,0,0}
{
	CharacterManager::AddCharacter(this);
}

Character::~Character() {
	CharacterManager::RemoveCharacter(this);
}

void Character::Update()
{
	if (knockBackTime_ > 0) {
		float speed = 1.0f - ((float)knockBackTime_ / (float)knockBackTimeMax_);
		speed = Easing::OutQuint(speed);
		KnockBack(speed);
		knockBackTime_--;
	}

}

//Y���̍��W�͓������Ȃ��悤�ɂ���
void Character::ReflectCharacter() {
	std::vector<Character*> cList = CharacterManager::GetCharacterList();
	float sY = transform_.position_.y;

	for (Character* c : cList) {
		if (c == this) continue;

		float oY = c->transform_.position_.y;

		//����̒��S�����ւ̃x�N�g�������߂�
		XMFLOAT3 tarPos = c->GetPosition();
		XMVECTOR direction = XMLoadFloat3(&tarPos) - XMLoadFloat3(&transform_.position_);

		//������������̋����������痣�ꂳ����
		float range = bodyRange_ + c->bodyRange_;
		float vecRange = XMVectorGetX(XMVector3Length(direction));
		if (vecRange < range) {
			//�ړ��ʂ��l���������W��ݒ�
			XMVECTOR vTarPos = XMLoadFloat3(&tarPos);
			direction = vTarPos - XMLoadFloat3(&transform_.position_);
			vecRange = XMVectorGetX(XMVector3Length(direction));

			//��̍��W�̐^�񒆂����߂�
			direction *= 0.5f;
			XMVECTOR sPos = XMLoadFloat3(&transform_.position_) + direction;
			XMVECTOR oPos = XMLoadFloat3(&c->transform_.position_) - direction;

			//�����o���x�N�g�������߂�
			float w = this->bodyWeight_ + c->bodyWeight_;
			float sWeight = c->bodyWeight_ / w + 0.001f;
			float oWeight = this->bodyWeight_ / w + 0.001f;
			direction = XMVector3Normalize(direction) * range * 0.5f;
			XMVECTOR push = direction * (range - vecRange);

			//�����o��
			sPos = sPos - direction - (push * sWeight);
			XMStoreFloat3(&transform_.position_, sPos);
			oPos = oPos + direction + (push * oWeight);
			XMStoreFloat3(&c->transform_.position_, oPos);
		}
		
		c->transform_.position_.y = oY;
	}

	transform_.position_.y = sY;
}

void Character::SetKnockBack(KNOCK_TYPE type, int time, float power, XMFLOAT3 pos)
{
	knockBackTime_ = time;
	knockBackTimeMax_ = time;
	knockBackPower_ = power;
	knockBackDirection_ = Float3Multiply(Float3Normalize(Float3Sub(transform_.position_, pos)), knockBackPower_);

	if (type == SMALL) SmallKnockBack();
	else if (type == MEDIUM) MediumKnockBack();
	else if (type == LARGE) LargetKnockBack();
}

void Character::KnockBack(float speed)
{
	transform_.position_.x += (knockBackDirection_.x * speed);
	transform_.position_.z += (knockBackDirection_.z * speed);
	SetMovement(Float3Multiply(knockBackDirection_, speed));
}