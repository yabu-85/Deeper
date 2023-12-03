#pragma once
#include <DirectXMath.h>
#include <vector>
using namespace DirectX;

class EnemySpawnCtrl;
class Character;

//•s—v
//‚µ‚å‚¤‚ç‚¢namespace‚É‚µ‚æ‚¤‚Ë
class LineCollider;
class SphereCollider;

class DamageCtrl
{
public:
	enum DamageType {
		DA_Player = 0,
		DA_Enemy,
	};

private:
	EnemySpawnCtrl* pEnemySpawnCtrl_;

	struct CollisionData {
		DamageType type;
		Character* objct;
	};
	std::vector<CollisionData> collisionList_;

public:
	DamageCtrl(EnemySpawnCtrl* p);
	~DamageCtrl();
	void Update();
	void AddCharacter(Character* obj, DamageType _type);
	void RemoveCharacter(Character* obj);
	void AllRemoveCharacter();

	//‚à‚¤Žg‚í‚È‚¢—\’è‚Ì‚â‚Â‚ç
	bool CalcSword(LineCollider* line, int damage);
	bool CalcBullet(SphereCollider* sphere, int damage);
	bool CalcPlyaer(SphereCollider* sphere, int damage);

};