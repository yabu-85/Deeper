#pragma once
#include "SceneBase.h"
#include "../Engine/SceneManager.h"

class Warp;

class StageBase : public SceneBase
{
	bool isCleared_;				//�N���A�������ǂ���
	Warp* pWarp_;					//���[�v�̃��X�g
	XMFLOAT3 startPosition_;		//�v���C���[�̃X�^�[�g���W
	XMFLOAT3 bossPosition_;			//�{�X�̃X�^�[�g�|�W�V����

public:
	StageBase(GameObject* parent, std::string name);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//�����FCSV�ESkyBox�̖��O
	void InitializeStage(std::string csv, std::string sky);
	
	//Warp�����X�g�ɒǉ�
	void SetWarp(Warp* w) { pWarp_ = w; }
	Warp* GetWarp() { return pWarp_; }

	//Warp��Valid���Z�b�g
	void SetWarpValid(bool b);

	//Warp��̐ݒ�
	void SetWarpStage(SCENE_ID id);

	//�X�e�[�W���N���A�������ɌĂԊ֐�
	virtual void OnStageCleared() {}
	
	//�X�e�[�W���N���A�������Ɉ�񂾂�true�ɂȂ�
	bool IsClearStage();
	
	//�N���A��񂵂���펞true��Ԃ�
	bool IsAlwaysClearStage() { return isCleared_; }

	void SetStartPosition(XMFLOAT3 pos) { startPosition_ = pos; }
	XMFLOAT3 GetStartPosition() { return startPosition_; }
	void SetBossPosition(XMFLOAT3 pos) { bossPosition_ = pos; }
	XMFLOAT3 GetBossPosition() { return bossPosition_; }

};

