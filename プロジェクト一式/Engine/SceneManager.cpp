#include "sceneManager.h"
#include "Model.h"
#include "Image.h"
#include "Audio.h"
#include "../Scene/Stage1.h"
#include "../Scene/Stage2.h"
#include "../Scene/Stage3.h"
#include "../GameManager/GameManager.h"
#include "../Player/PlayerData.h"
#include "../Scene/TitleScene.h"
#include "../Scene/ResultScene.h"

//�R���X�g���N�^
SceneManager::SceneManager(GameObject * parent)
	: GameObject(parent, "SceneManager"), currentSceneID_(SCENE_ID_TITLE), nextSceneID_(currentSceneID_)
{
}

//������
void SceneManager::Initialize()
{
	//�ŏ��̃V�[��������
#if _DEBUG
	currentSceneID_ = SCENE_ID_STAGE3;
	nextSceneID_ = currentSceneID_;
	Instantiate<Stage3>(this);
#else
	currentSceneID_ = SCENE_ID_TITLE;
	nextSceneID_ = currentSceneID_;
	Instantiate<TitleScene>(this);
#endif

	GameManager::SetSceneManager(this);

}

//�X�V
void SceneManager::Update()
{
	//���̃V�[�������݂̃V�[���ƈႤ�@���@�V�[����؂�ւ��Ȃ���΂Ȃ�Ȃ�
	if (currentSceneID_ != nextSceneID_)
	{
		OutputDebugString("SceneManager : ChangeInitialize\n");
		
		//���̃v���C���[�̃f�[�^��ۑ�����
		PlayerData::SavePlayerData();

		//�w���Stage�Ȃ�StageClear���𑝂₷
		PlayerData::AddClearStageCount(currentSceneID_);

		//���̃V�[���̃I�u�W�F�N�g��S�폜
		KillAllChildren();

		//���[�h�����f�[�^��S�폜
		Audio::Release();
		Model::AllRelease();
		Image::AllRelease();

		//�V�[���J�ڎ��̏�����
		GameManager::SceneTransitionInitialize();

		//���̃V�[�����쐬
		switch (nextSceneID_)
		{
			case SCENE_ID_TITLE: Instantiate<TitleScene>(this); break;
			case SCENE_ID_RESULT: Instantiate<ResultScene>(this); break;
			case SCENE_ID_STAGE1: Instantiate<Stage1>(this); break;
			case SCENE_ID_STAGE2: Instantiate<Stage2>(this); break;
			case SCENE_ID_STAGE3: Instantiate<Stage3>(this); break;
		}

		OutputDebugString("SceneManager : ChangeScene\n");

		Audio::Initialize();
		currentSceneID_ = nextSceneID_;
	}
}

//�`��
void SceneManager::Draw()
{
}

//�J��
void SceneManager::Release()
{
}

//�V�[���؂�ւ��i���ۂɐ؂�ւ��̂͂��̎��̃t���[���j
void SceneManager::ChangeScene(SCENE_ID next)
{
	nextSceneID_ = next;
}