#pragma once
#include "../Engine/GameObject.h"
#include "../Enemy/EnemyManager.h"
#include "WeaponObjectManager.h"

class StateManager;

/// <summary>
/// 武器のベースクラス
/// ボーンにくっつけて使う
/// </summary>
class WeaponBase : public GameObject
{
protected:
	int hModel_;
	int boneIndex_;
	int partIndex_; 
	
	bool isAtkEnd_;			//攻撃が終わったかどうかのフラグ
	bool isCancellable_;	//状態変化していいかのフラグ
	bool isReadyNext_;		//他の行動していいかどうか
	int durance_;			//耐久値

	ENEMY_TYPE type_;
	StateManager* pStateManager_;

public:
	WeaponBase(GameObject* parent, std::string name);
	virtual ~WeaponBase() = 0 {};
	virtual void Initialize() override = 0 {};
	virtual void Update() override = 0 {};
	virtual void Draw() override = 0 {};
	virtual void Release() override = 0 {};
	virtual void ResetState() = 0;
	void UpdateState();
	
	//初期攻撃状態へ推移
	virtual void ChangeAttackState() {};

	//耐久値を減らす
	void Endurance(int val = 1) { durance_ -= val; }

	//--------------------ゲッター・セッター----------------------
	ENEMY_TYPE GetWeaponType() { return type_; }
	
	bool IsCancellable() { return isCancellable_; }
	bool IsAtkEnd() { return isAtkEnd_; }
	bool IsNextReady() { return isReadyNext_; }
	void SetCancellable(bool b) { isCancellable_ = b; }
	void SetAtkEnd(bool b) { isAtkEnd_ = b; }
	void SetNextReady(bool b) { isReadyNext_ = b; }

	bool IsBlockend();
	int GetDurability() { return durance_; }
	void SetDurability(int i) { durance_ = i; }

};

