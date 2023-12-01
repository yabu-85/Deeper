#pragma once
#include <DirectXMath.h>
#include <vector>
using namespace DirectX;

class EnemySpawnCtrl;
class GameObject;

//•s—v
//‚µ‚å‚¤‚ç‚¢namespace‚É‚µ‚æ‚¤‚Ë
class LineCollider;
class SphereCollider;

class DamageCtrl
{
	EnemySpawnCtrl* pEnemySpawnCtrl_;

	enum DamageType {

	};
	struct Character {
		DamageType type;
		GameObject* objct;
	};
	std::vector<Character> calcCharacters_;

public:
	DamageCtrl(EnemySpawnCtrl* p);
	~DamageCtrl();
	void Update();

	//‚à‚¤Žg‚í‚È‚¢—\’è‚Ì‚â‚Â‚ç
	bool CalcSword(LineCollider* line, int damage);
	bool CalcBullet(SphereCollider* sphere, int damage);
	bool CalcPlyaer(SphereCollider* sphere, int damage);

};