#include "Character.h"
#include "CharacterManager.h"
#include "../Engine/Global.h"
#include "../Engine/Easing.h"
#include <vector>

Character::Character(GameObject* parent, std::string name)
	: GameObject(parent, name), hp_(0), maxHp_(0), movement_(0,0,0), bodyWeight_(1.0f), bodyRange_(0.5f), knockBackTime_(0), knockBackTimeMax_(0), 
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
		float speed = ((float)knockBackTime_ / (float)knockBackTimeMax_);
		speed = Easing::EaseOutQuint(speed);
		KnockBack(speed);
		knockBackTime_--;
	}

	ReflectCharacter();

}

//Y���̍��W�͓������Ȃ��悤�ɂ���
void Character::ReflectCharacter() {
	std::vector<Character*> cList = CharacterManager::GetCharacterList();
	float sY = transform_.position_.y;

	for (Character* c : cList) {
		//�����͔�΂�
		if (c == this) continue;
		float oY = c->transform_.position_.y;

		XMFLOAT3 targetPos = c->GetPosition();
		XMFLOAT3 direction = Float3Sub(targetPos, transform_.position_);
		
		//�������ĂȂ��Ȃ玟
		float addRange = bodyRange_ + c->bodyRange_;
		float range = CalculationDistance(direction);
		if (range > addRange) continue;

		//�����o���̋����i0�ɂȂ�̖h�����߂ɂ��傢�����j
		float w = this->bodyWeight_ + c->bodyWeight_;
		float sWeight = c->bodyWeight_ / w + 0.001f;
		float oWeight = this->bodyWeight_ / w + 0.001f;

		//�����o���x�N�g���v�Z
		XMFLOAT3 extrusion = Float3Multiply(Float3Normalize(direction), addRange - range);

		//�����o��
		XMFLOAT3 outPos = Float3Multiply(extrusion, -sWeight);
		outPos = Float3Add(outPos, transform_.position_);
		transform_.position_ = outPos;

		outPos = Float3Multiply(extrusion, oWeight);
		outPos = Float3Add(outPos, targetPos);
		c->transform_.position_ = outPos;
		
		//y���W�͖߂�
		c->transform_.position_.y = oY;
	}

	transform_.position_.y = sY;
}

void Character::ApplyDamageDirectly(const DamageInfo& damageinfo)
{
	hp_ -= damageinfo.damage;
	Damage();

	//���S�����ꍇDead�֐��Ă�
	if (hp_ <= 0) Dead();
}

void Character::ApplyDamageDirectly(const DamageInfo& damageinfo, const KnockBackInfo& knockinfo)
{
	//���Ԃ��p���[��񂠂�Ȃ�m�b�N�o�b�N������
	if (knockinfo.time != 0 || knockinfo.power != 0.0f) {
		knockBackTime_ = knockinfo.time;
		knockBackTimeMax_ = knockinfo.time;
		knockBackDirection_ = Float3Multiply(Float3Normalize(Float3Sub(transform_.position_, knockinfo.pos)), knockinfo.power);

		if (knockinfo.type == KNOCK_TYPE::SMALL) SmallKnockBack();
		else if (knockinfo.type == KNOCK_TYPE::MEDIUM) MediumKnockBack();
		else if (knockinfo.type == KNOCK_TYPE::LARGE) LargeKnockBack();
	}
	
	ApplyDamageDirectly(damageinfo);
}

bool Character::ApplyDamageWithList(const DamageInfo& daamgeinfo)
{
	//List���Ȃ��Ȃ�ǉ�����Damage��^����E���łɂ���Ȃ�I��
	for (auto it = damageInfoList_.begin(); it != damageInfoList_.end(); ++it) {
		if (it->owner == daamgeinfo.owner && it->name == daamgeinfo.name) return false;
	}
	AddDamageInfo(daamgeinfo);
	ApplyDamageDirectly(daamgeinfo);
	
	return true;
}

bool Character::ApplyDamageWithList(const DamageInfo& daamgeinfo, const KnockBackInfo& knockinfo)
{
	//List���Ȃ��Ȃ�ǉ�����Damage��^����E���łɂ���Ȃ�I��
	for (auto it = damageInfoList_.begin(); it != damageInfoList_.end(); ++it) {
		if (it->owner == daamgeinfo.owner && it->name == daamgeinfo.name) return false;
	}
	AddDamageInfo(daamgeinfo);
	ApplyDamageDirectly(daamgeinfo, knockinfo);
	
	return true;
}

void Character::AddDamageInfo(const DamageInfo& info)
{
	damageInfoList_.push_back(info);
}

void Character::RemoveDamageInfo(const DamageInfo& info)
{
	for (auto it = damageInfoList_.begin(); it != damageInfoList_.end(); ++it) {
		if (it->owner == info.owner && it->name == info.name) {
			damageInfoList_.erase(it);
			break;
		}
	}
}

void Character::KnockBack(float speed)
{
	transform_.position_.x += (knockBackDirection_.x * speed);
	transform_.position_.z += (knockBackDirection_.z * speed);
	SetMovement(Float3Multiply(knockBackDirection_, speed));
}

void DamageController::AddAttackList(Character* chara)
{ 
	attackList.push_back(chara); 
}

void DamageController::ResetAttackList() 
{ 
	for (auto i : attackList) {
		i->RemoveDamageInfo(currentDamage);
	}
	attackList.clear();
}