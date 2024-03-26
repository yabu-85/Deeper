#pragma once
#include <d3d11.h>
#include <fbxsdk.h>
#include <vector>
#include <string>
#include "Transform.h"
#include "Direct3D.h"

class FbxParts;

//���C�L���X�g�p�\����
struct RayCastData
{
	XMFLOAT3	start;	//���C���ˈʒu
	XMFLOAT3	dir;	//���C�̌����x�N�g��
	float       dist;	//�Փ˓_�܂ł̋���
	BOOL        hit;	//���C������������
	XMFLOAT3 normal;	//�@��

	RayCastData() { start = { 0.0f, 0.0f, 0.0f }, dir = { 0.0f, 0.0f, 0.0f }, 
		normal = {0.0f, 0.0f, 0.0f}, hit = FALSE; dist = 99999.0f; }
};

//-----------------------------------------------------------
//�@FBX�t�@�C���������N���X
//�@�قƂ�ǂ̏����͊e�p�[�c���Ƃ�FbxParts�N���X�ōs��
//-----------------------------------------------------------
class Fbx
{
	//FbxPart�N���X���t�����h�N���X�ɂ���
	//FbxPart��private�Ȋ֐��ɂ��A�N�Z�X��
	friend class FbxParts;



	//���f���̊e�p�[�c�i�������邩���j
	std::vector<FbxParts*>	parts_;

	//FBX�t�@�C���������@�\�̖{�́iFbxImport��FbxScene���쐬���邽�߂ɕK�v�ȊǗ��N���X�ł��B
	FbxManager* pFbxManager_;

	//FbxImporter
	//Fbx�t�@�C����Import���邽�߂̃p�[�T�[�N���X�ł��B
	//�t�@�C���̎��f�[�^�����b�V����J�����Ȃǂ̏��ɕ������Ă���܂��B

	//FBX�t�@�C���̃V�[���iMaya�ō�������ׂĂ̕��́j������
	//Importer�ɂ���āA�������ꂽFbx�̃f�[�^��ۑ����邽�߂̃N���X�ł��B
	FbxScene*	pFbxScene_;


	// �A�j���[�V�����̃t���[�����[�g
	FbxTime::EMode	_frameRate;

	//�A�j���[�V�������x
	float			_animSpeed;

	//�A�j���[�V�����̍ŏ��ƍŌ�̃t���[��
	int _startFrame, _endFrame;





	//�m�[�h�̒��g�𒲂ׂ�
	//�����FpNode		���ׂ�m�[�h
	//�����FpPartsList	�p�[�c�̃��X�g
	void CheckNode(FbxNode* pNode, std::vector<FbxParts*> *pPartsList);




public:
	Fbx();
	~Fbx();

	//���[�h
	//�����FfileName	�t�@�C����
	//�ߒl�F�����������ǂ���
	virtual HRESULT Load(std::string fileName);

	//�`��
	//�����FWorld	���[���h�s��
	void    Draw(Transform& transform, Direct3D::SHADER_TYPE shader, int frame1, int frame2, float blendFactor);

	//�`��
	//�����FWorld	���[���h�s��
	void    Draw(Transform& transform, Direct3D::SHADER_TYPE shader, int frame);

	//���
	void    Release();

	bool GetBoneIndex(std::string boneName, int* index, int* partIndex);

	//�C�ӂ̃{�[���̈ʒu���擾
	//�����FboneName	�擾�������{�[���̈ʒu
	//�ߒl�F�{�[���̈ʒu
	XMFLOAT3 GetBonePosition(int index, int partIndex);

	//�C�ӂ̃A�j���[�V�������̃{�[���̈ʒu���擾
	XMFLOAT3 GetBoneAnimBlendPosition(int index, int partIndex, int frame1, int frame2, float blendFactor);

	//�C�ӂ̃A�j���[�V�������̃{�[���̈ʒu���擾
	XMFLOAT3 GetBoneAnimPosition(int index, int partIndex, int frame);

	//�C�ӂ̃A�j���[�V�������̃{�[���̉�]���擾
	XMFLOAT3 GetBoneAnimRotate(int index, int partIndex, int frame);

	//�C�ӂ̃A�j���[�V�������̃{�[���̉�]Matrix���擾
	XMFLOAT3 GetBoneAniBlendRotate(int index, int partIndex, int frame1, int frame2, float blendFactor);

	//���C�L���X�g�i���C���΂��ē����蔻��j
	//�����Fdata	�K�v�Ȃ��̂��܂Ƃ߂��f�[�^
	void RayCast(RayCastData *data);

	std::vector<FbxParts*> GetFbxParts() { return parts_; };

};

