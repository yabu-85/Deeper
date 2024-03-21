#include "Character.h"
#include "CharacterManager.h"
#include "../Engine/Global.h"
#include "../Engine/Easing.h"
#include <vector>

Character::Character(GameObject* parent, std::string name)
	: GameObject(parent, name), movement_(0,0,0), bodyWeight_(1.0f), bodyRange_(0.5f), knockBackTime_(0), knockBackTimeMax_(0), knockBackDirection_{0,0,0}
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
		speed = Easing::OutQuint(speed);
		KnockBack(speed);
		knockBackTime_--;
	}

}

//Y軸の座標は動かさないようにする
void Character::ReflectCharacter() {
	std::vector<Character*> cList = CharacterManager::GetCharacterList();
	float sY = transform_.position_.y;

	for (Character* c : cList) {
		if (c == this) continue;

		float oY = c->transform_.position_.y;

		//相手の中心方向へのベクトルを求める
		XMFLOAT3 tarPos = c->GetPosition();
		XMVECTOR direction = XMLoadFloat3(&tarPos) - XMLoadFloat3(&transform_.position_);

		//距離が判定内の距離だったら離れさせる
		float range = bodyRange_ + c->bodyRange_;
		float vecRange = XMVectorGetX(XMVector3Length(direction));
		if (vecRange < range) {
			//移動量を考慮した座標を設定
			XMVECTOR vTarPos = XMLoadFloat3(&tarPos);
			direction = vTarPos - XMLoadFloat3(&transform_.position_);
			vecRange = XMVectorGetX(XMVector3Length(direction));

			//二つの座標の真ん中を求める
			direction *= 0.5f;
			XMVECTOR sPos = XMLoadFloat3(&transform_.position_) + direction;
			XMVECTOR oPos = XMLoadFloat3(&c->transform_.position_) - direction;

			//押し出しベクトルを求める
			float w = this->bodyWeight_ + c->bodyWeight_;
			float sWeight = c->bodyWeight_ / w + 0.001f;
			float oWeight = this->bodyWeight_ / w + 0.001f;
			direction = XMVector3Normalize(direction) * range * 0.5f;
			XMVECTOR push = direction * (range - vecRange);

			//押し出す
			sPos = sPos - direction - (push * sWeight);
			XMStoreFloat3(&transform_.position_, sPos);
			oPos = oPos + direction + (push * oWeight);
			XMStoreFloat3(&c->transform_.position_, oPos);
		}
		
		c->transform_.position_.y = oY;
	}

	transform_.position_.y = sY;
}

void Character::ApplyDamageDirectly(const DamageInfo& damageinfo)
{
	hp_ -= damageinfo.damage;

	Damage();
	if (hp_ <= 0) {
		Dead();
	}
}

void Character::ApplyDamageDirectly(const DamageInfo& damageinfo, const KnockBackInfo& knockinfo)
{
	ApplyDamageDirectly(damageinfo);

	//時間とパワーがあるならノックバックさせる
	if (knockinfo.time == 0 || knockinfo.power == 0.0f) return;
	knockBackTime_ = knockinfo.time;
	knockBackTimeMax_ = knockinfo.time;
	knockBackDirection_ = Float3Multiply(Float3Normalize(Float3Sub(transform_.position_, knockinfo.pos)), knockinfo.power);

 	if (knockinfo.type == KNOCK_TYPE::SMALL) SmallKnockBack();
	else if (knockinfo.type == KNOCK_TYPE::MEDIUM) MediumKnockBack();
	else if (knockinfo.type == KNOCK_TYPE::LARGE) LargetKnockBack();
}

bool Character::ApplyDamageWithList(const DamageInfo& daamgeinfo)
{
	//List似ないなら追加してDamageを与える・すでにあるなら終了
	for (auto it = damageInfoList_.begin(); it != damageInfoList_.end(); ++it) {
		if (it->owner == daamgeinfo.owner && it->name == daamgeinfo.name) return false;
	}
	AddDamageInfo(daamgeinfo);
	ApplyDamageDirectly(daamgeinfo);
	
	return true;
}

bool Character::ApplyDamageWithList(const DamageInfo& daamgeinfo, const KnockBackInfo& knockinfo)
{
	//List似ないなら追加してDamageを与える・すでにあるなら終了
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