#pragma once
#include <DirectXMath.h>
#include <assert.h>
#include <vector>
#include <string>
#include "Fbx.h"
#include "Transform.h"

//-----------------------------------------------------------
//3D���f���iFBX�t�@�C���j���Ǘ�����
//-----------------------------------------------------------
namespace Model
{
	//���f�����
	struct ModelData
	{
		//�t�@�C����
		std::string fileName;

		//���[�h�������f���f�[�^�̃A�h���X
		Fbx*		pFbx;

		//�s��
		Transform 	transform;

		//�A�j���[�V�����̃t���[��
		bool isAimeStop;
		float nowFrame, animSpeed;
		int startFrame, endFrame;

		// �u�����f�B���O�p�t�B�[���h
		bool isBlending;
		float blendFrame;
		float blendWeight;

		//������
		ModelData() : pFbx(nullptr), isAimeStop(false), nowFrame(0), startFrame(0), endFrame(0), animSpeed(0),
			isBlending(false), blendFrame(0.0f), blendWeight(0.0f)
		{
		}

		// �u�����f�B���O�A�j���[�V�����̃p�����[�^��ݒ�
		void SetBlendingAnimFrame(int _startFrame1, int _endFrame1, int _startFrame2, float _blendSpeed, float _blendWeight)
		{
			isBlending = true;

			nowFrame = (float)_startFrame1;
			blendFrame = (float)_startFrame2;
			startFrame = _startFrame1;
			endFrame = _endFrame1;

			animSpeed = _blendSpeed;
			blendWeight = _blendWeight;
		}

		//�A�j���[�V�����̃t���[�������Z�b�g
		//�����FstartFrame	�J�n�t���[��
		//�����FendFrame	�I���t���[��
		//�����FanimSpeed	�A�j���[�V�������x
		void SetAnimFrame(int start, int end, float speed)
		{
			isBlending = false;

			nowFrame = (float)start;
			startFrame = start;
			endFrame = end;
			animSpeed = speed;
		}
	};

	//������
	void Initialize();

	//���f�������[�h
	//�����FfileName�@�t�@�C����
	//�ߒl�F���̃��f���f�[�^�Ɋ��蓖�Ă�ꂽ�ԍ�
	int Load(std::string fileName);

	//�`��
	//�����Fhandle	�`�悵�������f���̔ԍ�
	//�����Fmatrix	���[���h�s��
	void Draw(int handle, int type = 0);

	//�C�ӂ̃��f�����J��
	//�����Fhandle	�J�����������f���̔ԍ�
	void Release(int handle);

	//�S�Ẵ��f�������
	//�i�V�[�����؂�ւ��Ƃ��͕K�����s�j
	void AllRelease();

	//�A�j���[�V�����u�����f�B���O�̒l�Z�b�g�F�P�̃A�j���[�V�����ɂQ�̎w�肵���E�F�C�g�Ńu�����f�B���O������
	//�����Fhandle		�ݒ肵�������f���̔ԍ�
	//�����FstartFrame	�J�n�t���[��1
	//�����FendFrame	�I���t���[��1
	//�����FstartFrame2	�J�n�t���[��2
	//�����FanimSpeed	�A�j���[�V�������x
	//�����FblendWeight �A�j���[�V�����Q�̉e���x
	void SetBlendingAnimFrame(int handle, int startFrame1, int endFrame1, int startFrame2, float animSpeed, float blendWeight);

	//�A�j���[�V�����̃t���[�������Z�b�g
	//�����Fhandle		�ݒ肵�������f���̔ԍ�
	//�����FstartFrame	�J�n�t���[��
	//�����FendFrame	�I���t���[��
	//�����FanimSpeed	�A�j���[�V�������x
	void SetAnimFrame(int handle, int startFrame, int endFrame, float animSpeed);

	void SetBlendFactor(int handle, float weight);

	//�A�j���[�V������~
	void SetAnimeStop(int handle, bool b);
	bool IsAnimeStop(int handle);

	//���݂̃A�j���[�V�����̃t���[�����擾
	int GetAnimFrame(int handle);

	//�C�ӂ̃{�[���̈ʒu���擾
	//�����Fhandle		���ׂ������f���̔ԍ�
	//�����FboneName	���ׂ����{�[���̖��O
	//�ߒl�F�{�[���̈ʒu�i���[���h���W�j
	XMFLOAT3 GetBonePosition(int handle, std::string boneName);

	//�A�j���[�V�������̃{�[���̈ʒu���擾
	XMFLOAT3 GetBoneAnimPosition(int handle, std::string boneName);
	
	//�A�j���[�V�������̃{�[���̉�]���擾
	XMFLOAT3 GetBoneAnimRotate(int handle, std::string boneName);

	//�A�j���[�V�������̃{�[���̉�]Matrix���擾
	XMMATRIX GetBoneAnimRotateMatrix(int handle, std::string boneName);

	//���[���h�s���ݒ�
	//�����Fhandle	�ݒ肵�������f���̔ԍ�
	//�����Fmatrix	���[���h�s��
	void SetTransform(int handle, Transform& transform);

	//���[���h�s��̎擾
	//�����Fhandle	�m�肽�����f���̔ԍ�
	//�ߒl�F���[���h�s��
	XMMATRIX GetMatrix(int handle);

	//�w��ModelData��Fbx�����擾
	Fbx* GetFbx(int handle);

	//���C�L���X�g�i���C���΂��ē����蔻��j�@��������
	//�����Fhandle	���肵�������f���̔ԍ�
	//�����Fdata	�K�v�Ȃ��̂��܂Ƃ߂��f�[�^
	void RayCast(int handle, RayCastData *data);

};