#pragma once
#include "Collider.h"

//-----------------------------------------------------------
//�����̓����蔻��
//-----------------------------------------------------------
class LineCollider : public Collider
{
	XMFLOAT3 vec_; //�����̌���
	
	//Collider�N���X��private�����o�ɃA�N�Z�X�ł���悤�ɂ���
	friend class Collider;

	//�ڐG����
	//�����Ftarget	����̓����蔻��
	//�ߒl�F�ڐG���Ă��true
	bool IsHit(Collider* target) override;

public:
	//�R���X�g���N�^�i�����蔻��̍쐬�j
	//�����FbasePos	�����蔻��̒��S�ʒu�i�Q�[���I�u�W�F�N�g�̌��_���猩���ʒu�j
	//�����Fvec 	�����蔻��̌���
	//�����Frange	�����蔻��̃T�C�Y
	LineCollider(XMFLOAT3 center, XMFLOAT3 vec, float range);

	void SetVec(XMFLOAT3 _vec) { vec_ = _vec; }
	void SetSize(float _size) { size_ = XMFLOAT3(_size, _size, _size); }

};