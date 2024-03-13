#pragma once
#include "../Engine/GameObject.h"

class BulletBase : public GameObject
{
	GameObject* pParent_;	//撃った人のポインタ

protected:
	int hModel_;
	int lifeTime_;			//生存時間
	int damage_;			//ダメージ
	float velocity_;		//弾速
	XMFLOAT3 moveVec_;		//移動量

public:
	BulletBase(GameObject* parent, std::string name);
	virtual void Shot(XMFLOAT3 pos, XMFLOAT3 target);	//発射
	virtual void Move();								//移動させる
	virtual void Hit() = 0;								//当たった時の処理

	void LifeTime();							//生存時間の計算
	void LifeDead();							//生存時間が終わったら呼ぶ関数	
	int GetDamage() { return damage_; }			//ダメージ量を取得
	
	void SetShotParent(GameObject* p) { pParent_ = p; }
	GameObject* GetShotParent() { return pParent_; }

};

