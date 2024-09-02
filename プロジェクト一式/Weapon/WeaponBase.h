#pragma once
#include "../Engine/GameObject.h"
#include "../Enemy/EnemyManager.h"
#include "WeaponObjectManager.h"

class StateManager;

/// <summary>
/// ����̃x�[�X�N���X
/// �{�[���ɂ������Ďg��
/// </summary>
class WeaponBase : public GameObject
{
protected:
	int hModel_;
	int boneIndex_;
	int partIndex_; 
	
	bool isAtkEnd_;			//�U�����I��������ǂ����̃t���O
	bool isCancellable_;	//��ԕω����Ă������̃t���O
	bool isReadyNext_;		//���̍s�����Ă������ǂ���
	int durance_;			//�ϋv�l

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
	
	//�����U����Ԃ֐���
	virtual void ChangeAttackState() {};

	//�ϋv�l�����炷
	void Endurance(int val = 1) { durance_ -= val; }

	//--------------------�Q�b�^�[�E�Z�b�^�[----------------------
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

