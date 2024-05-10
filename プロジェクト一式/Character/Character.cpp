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
		speed = Easing::QuintOut(speed);
		KnockBack(speed);
		knockBackTime_--;
	}

	ReflectCharacter();

}

//Y軸の座標は動かさないようにする
void Character::ReflectCharacter() {
	std::vector<Character*> cList = CharacterManager::GetCharacterList();
	float sY = transform_.position_.y;

	for (Character* c : cList) {
		//自分は飛ばす
		if (c == this) continue;
		float oY = c->transform_.position_.y;

		XMFLOAT3 targetPos = c->GetPosition();
		XMFLOAT3 direction = Float3Sub(targetPos, transform_.position_);
		
		//当たってないなら次
		float addRange = bodyRange_ + c->bodyRange_;
		float range = CalculationDistance(direction);
		if (range > addRange) continue;

		//押し出しの強さ（0になるの防ぐためにちょい足す）
		float w = this->bodyWeight_ + c->bodyWeight_;
		float sWeight = c->bodyWeight_ / w + 0.001f;
		float oWeight = this->bodyWeight_ / w + 0.001f;

		//押し出しベクトル計算
		XMFLOAT3 extrusion = Float3Multiply(Float3Normalize(direction), addRange - range);

		//押し出す
		XMFLOAT3 outPos = Float3Multiply(extrusion, -sWeight);
		outPos = Float3Add(outPos, transform_.position_);
		transform_.position_ = outPos;

		outPos = Float3Multiply(extrusion, oWeight);
		outPos = Float3Add(outPos, targetPos);
		c->transform_.position_ = outPos;
		
		//y座標は戻す
		c->transform_.position_.y = oY;
	}

	transform_.position_.y = sY;
}

void Character::ApplyDamageDirectly(const DamageInfo& damageinfo)
{
	hp_ -= damageinfo.damage;
	Damage();

	//死亡した場合Dead関数呼ぶ
	if (hp_ <= 0) Dead();
}

void Character::ApplyDamageDirectly(const DamageInfo& damageinfo, const KnockBackInfo& knockinfo)
{
	//時間かパワー情報あるならノックバックさせる
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