#pragma once
#include <vector>
#include <Windows.h>

class EnemyManager;
class NavigationAI;
class WeaponObjectManager;
class CollisionMap;
class StageBase;
class Player;
class CreateStage;
class SceneManager;

namespace GameManager
{
	void Initialize();
	void Update();
	void Draw();
	void SceneTransitionInitialize();

	//�|�[�Y���j���[�֐�
	void StartPause();
	void EndPause();

	void PauseClose();

	//�}�E�X�̐�����؂�ւ���Ƃ��̃t���O
	bool MouseLimitedChange();

	//���}�E�X�������|����ׂ���
	bool IsMouseLimitedScene();

	EnemyManager* GetEnemyManager();
	NavigationAI* GetNavigationAI();
	WeaponObjectManager* GetWeaponObjectManager();
	CreateStage* GetCreateStage();
	SceneManager* GetSceneManager();
	void SetSceneManager(SceneManager* ma);
	CollisionMap* GetCollisionMap();
	void SetCollisionMap(CollisionMap* map);
	Player* GetPlayer();
	void SetPlayer(Player* player);
	StageBase* GetStage();
	void SetStage(StageBase* stage);
};