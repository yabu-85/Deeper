#pragma once
#include "SceneBase.h"
#include "../Engine/SceneManager.h"
#include <vector>

class Warp;

class StageBase : public SceneBase
{
	bool isCleared_;				//�N���A�������ǂ���
	std::vector<Warp*> warpList_;	//���[�v�̃��X�g
	XMFLOAT3 startPosition_;		//�v���C���[�̃X�^�[�g���W

public:
	StageBase(GameObject* parent, std::string name);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//�����FCSV�ESkyBox�̖��O
	void InitializeStage(std::string csv, std::string sky);

	//Warp�����X�g�ɒǉ�
	void AddWarpList(Warp* w) { warpList_.push_back(w); }
	
	//�S�Ă�Warp��Valid���Z�b�g
	void SetAllWarpValid(bool b);

	void SetWarpStage(SCENE_ID* list);

	//�X�e�[�W���N���A�������ɌĂԊ֐�
	virtual void OnStageCleared() {}
	
	//�X�e�[�W���N���A�������Ɉ�񂾂�true�ɂȂ�
	bool IsClearStage();

	void SetStartPosition(XMFLOAT3 pos) { startPosition_ = pos; }
	XMFLOAT3 GetStartPosition() { return startPosition_; }

};

