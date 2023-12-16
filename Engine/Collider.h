#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

//�N���X�̑O���錾
class GameObject;
class BoxCollider;
class SphereCollider;
class SegmentCollider;

//�����蔻��̃^�C�v
enum ColliderType
{
	COLLIDER_BOX,		//���^
	COLLIDER_CIRCLE,	//����
	COLLIDER_SEGMENT,	//����
	COLLIDER_MAX,		
};

//-----------------------------------------------------------
//�����蔻����Ǘ�����N���X
//-----------------------------------------------------------
class Collider
{
	//���ꂼ��̃N���X��private�����o�ɃA�N�Z�X�ł���悤�ɂ���
	friend class BoxCollider;
	friend class SphereCollider;
	friend class SegmentCollider;

protected:
	GameObject*		pGameObject_;	//���̔���������Q�[���I�u�W�F�N�g
	ColliderType	type_;			//���
	XMFLOAT3		center_;		//���S�ʒu�i�Q�[���I�u�W�F�N�g�̌��_���猩���ʒu�j
	XMFLOAT3		size_;			//����T�C�Y�i���A�����A���s���j
	int				hDebugModel_;	//�f�o�b�O�\���p�̃��f����ID

public:
	//�R���X�g���N�^
	Collider();

	//�f�X�g���N�^
	virtual ~Collider();

	//�ڐG����i�p�����SphereCollider��BoxCollider�ŃI�[�o�[���C�h�j
	//�����Ftarget	����̓����蔻��
	//�ߒl�F�ڐG���Ă��true
	virtual bool IsHit(Collider* target) = 0;

	//-------------------------------------------------------

	//���^���m�̏Փ˔���
	//�����FboxA	�P�ڂ̔��^����
	//�����FboxB	�Q�ڂ̔��^����
	//�ߒl�F�ڐG���Ă����true
	bool IsHitBoxVsBox(BoxCollider* boxA, BoxCollider* boxB);

	//���^�Ƌ��̂̏Փ˔���
	//�����Fbox	    1�ڂ̔��^����
	//�����Fsphere  �Q�ڂ̐�������
	//�ߒl�F�ڐG���Ă����true
	bool IsHitBoxVsCircle(BoxCollider* box, SphereCollider* sphere);

	//���^�ƒ����̏Փ˔���
	//�����Fbox	���^����
	//�����Fseg ��������
	//�ߒl�F�ڐG���Ă����true
	bool IsHitBoxVsSegment(BoxCollider* box, SegmentCollider* seg);

	//-------------------------------------------------------

	//���̓��m�̏Փ˔���
	//�����FcircleA	�P�ڂ̋��̔���
	//�����FcircleB	�Q�ڂ̋��̔���
	//�ߒl�F�ڐG���Ă����true
	bool IsHitCircleVsCircle(SphereCollider* circleA, SphereCollider* circleB);

	//���̂Ɛ����Փ˔���
	//�����Fcircle	�P�ڂ̋��̔���
	//�����Fseg	    �Q�ڂ̒�������
	//�ߒl�F�ڐG���Ă����true
	bool IsHitCircleVsSegment(SphereCollider* circle, SegmentCollider* seg);

	//-------------------------------------------------------

	//�������m�̏Փ˔���
	//�����FsegA	�P�ڂ̒�������
	//�����FsegB	�Q�ڂ̒�������
	//�ߒl�F�ڐG���Ă����true
	bool IsHitSegmentVsSegment(SegmentCollider* segA, SegmentCollider* segB);

	//-------------------------------------------------------

	//�e�X�g�\���p�̘g��`��
	//�����Fposition	�I�u�W�F�N�g�̈ʒu
	void Draw(XMFLOAT3 position);

	void SetGameObject(GameObject* gameObject) { pGameObject_ = gameObject; }
	GameObject* GetGameObject() { return pGameObject_; }

	XMFLOAT3 GetCenter() { return center_; };
	void SetCenter(XMFLOAT3 pos) { center_ = pos; }

	void SetSize(float _size) { size_ = XMFLOAT3(_size, _size, _size); }
	XMFLOAT3 GetSize() { return size_; }
};

