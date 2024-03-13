#pragma once
#include "../Engine/GameObject.h"

class BulletBase : public GameObject
{
	GameObject* pParent_;	//�������l�̃|�C���^

protected:
	int hModel_;
	int lifeTime_;			//��������
	int damage_;			//�_���[�W
	float velocity_;		//�e��
	XMFLOAT3 moveVec_;		//�ړ���

public:
	BulletBase(GameObject* parent, std::string name);
	virtual void Shot(XMFLOAT3 pos, XMFLOAT3 target);	//����
	virtual void Move();								//�ړ�������
	virtual void Hit() = 0;								//�����������̏���

	void LifeTime();							//�������Ԃ̌v�Z
	void LifeDead();							//�������Ԃ��I�������ĂԊ֐�	
	int GetDamage() { return damage_; }			//�_���[�W�ʂ��擾
	
	void SetShotParent(GameObject* p) { pParent_ = p; }
	GameObject* GetShotParent() { return pParent_; }

};

